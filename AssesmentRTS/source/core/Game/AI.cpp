#include "../ent/ent.h"
#include "../math.h"
#include "../ui/ui.h"
#include "../Game/game.h"

enum UnitTask
{
	IDLE = 0,
	FIGHT_UNITS = 1,
	FIGHT_BUILDS = 2
};

bool canBuildTower = false;
bool canBuildBarracks = false;
bool firstRun = true;
bool targetingBuilds = false;

int tempCount = 0;
int money;
int playerUnits;
int playerUnitsAttackingBuilds;
int playerUnitsAttackingUnits;
int units;
int idleUnits;
int unitAttackers;
int minUnits = 10;
int barracks;
int barrackSlots;
int towers;
int ai_bg[gridY][gridX];
int tmr_Count = 0;
int unitCountSave = 0;

float ai_TimerStart = 0.0f;
float tmr_Update = ai_TimerStart;

//Counts all AI units
int ai_CU()
{
	int count = 0;
	for (int a = 0; a < unitSpawnIndex; a++)
	{
		if (u_Current == u_Invader) { count++; }
	}

	return count;
}

//Count units doing whatever task
int ai_CUDT(UnitTask ut)
{
	int count = 0;
	switch (ut)
	{
	case IDLE:
		for (int a = 0; a < unitSpawnIndex; a++) { if (u_Current == u_Invader && u_Current.getTarget() == NOTHING) { count++; } } break;
	case FIGHT_UNITS:
		for (int a = 0; a < unitSpawnIndex; a++) { if (u_Current == u_Invader && u_Current.getTarget() == UNITS) { count++; } } break;
	case FIGHT_BUILDS:
		for (int a = 0; a < unitSpawnIndex; a++) { if (u_Current == u_Invader && u_Current.getTarget() == BUILDINGS) { count++; } } break;
	}

	return count;
}

//Count number of player units
int ai_CPU()
{
	int count = 0;
	for (int a = 0; a < unitSpawnIndex; a++)
	{
		if (u_Current == u_Human)
		{
			count++;
		}
	}

	return count;
}

//Counts the number of player units actively attacking its units
int ai_CPUAU()
{
	int count = 0;
	for (int a = 0; a < unitSpawnIndex; a++)
	{
		if (u_Current == u_Human && u_Current.getTarget() == UNITS && u_Current.isTargetInRange())
		{
			count++;
		}
	}
	return count;
}

//Counts the number of player units actively attacking it's buildings 
int ai_CPUNB()
{
	int count = 0;
	for (int a = 0; a < unitSpawnIndex; a++)
	{
		if (u_Current == u_Human && u_Current.getTarget() == BUILDINGS && u_Current.isTargetInRange())
		{
			count++;
		}
	}

	return count;
}

//Counts the number of player units that are or aren't in an area
int ai_CPUIA(float x, float y, float h, float w, bool countOut = false)
{
	int count = 0;
	PosDim pd;
	for (int a = 0; a < unitSpawnIndex; a++)
	{
		if (u_Current == u_Human)
		{
			pd = u_Current.getPosDim();

			if (pd.x > x && pd.x < x + w && pd.y > y && pd.y < y + h)
			{
				if (!countOut)
				{
					count++;
				}
			}
			else if (countOut)
			{
				count++;
			}
		}
	}

	return count;
}

//Counts player units in the area doing whatever task
int ai_CPUIADT(float x, float y, float h, float w, Target tar)
{
	int count = 0;
	PosDim pd;
	for (int a = 0; a < unitSpawnIndex; a++)
	{
		if (u_Current == u_Human && u_Current.getTarget() == tar)
		{
			pd = u_Current.getPosDim();

			if (pd.x > x && pd.x < x + w && pd.y > y && pd.y < y + h)
			{
				count++;
			}
		}
	}

	return count;
}

//Counts the number of AI units that are or aren't in an area
int ai_CMUIA(float x, float y, float h, float w, bool countOut = false)
{
	int count = 0;
	PosDim pd;
	for (int a = 0; a < unitSpawnIndex; a++)
	{
		if (u_Current == u_Invader)
		{
			pd = u_Current.getPosDim();

			if (pd.x > x && pd.x < x + w && pd.y > y && pd.y < y + h && !countOut)
			{
				count++;
			}
			else if (countOut)
			{
				count++;
			}
		}
	}

	return count;
}

//Counts its barracks
int ai_CB()
{
	int count = 0;
	for (int a = 0; a < buildSpawnIndex; a++) { if (b_Current == b_InvaderBarracks) { count++; } }
	return count;
}

//Counts its towers
int ai_CT()
{
	int count = 0;
	for (int a = 0; a < buildSpawnIndex; a++) { if (b_Current == b_InvaderTower) { count++; } }
	return count;
}

//Returns the size of the biggest group of AI units doing a given task
int ai_CBG(Target tar)
{
	int biggestGroup = 0;

	for (int a = 0; a < unitSpawnIndex; a++)
	{
		//Finds the biggest group of units attacking their target (tar)
		if (u_Current == u_Invader && u_Current.getTarget() == tar)
		{
			int group = ai_CPUIADT(u_Current.getPosDim().x, u_Current.getPosDim().y, u_Current.getAtkRad(), u_Current.getAtkRad(), tar);
			if (group > biggestGroup) { biggestGroup = group; }
		}
	}

	return biggestGroup;
}

//Are there player buildings in a given area?
bool ai_CPBIA(float x, float y, float h, float w)
{
	for (int a = 0; a < buildSpawnIndex; a++)
	{
		if (b_Current == b_HumanBarracks || b_Current == b_HumanTC || b_Current == b_HumanTower)
		{
			PosDim pd = b_Current.getPosDim();

			if (pd.x > x && pd.x < x + w)
			{
				if (pd.y > y && pd.y < y + h)
				{
					return true;
				}
			}
		}
	}

	return false;
}

//Find the biggest group of enemy units actively attacking a given target
PosDim ai_FBEG(Target tar)
{
	int biggestGroup = 0;
	PosDim location;

	for (int a = 0; a < unitSpawnIndex; a++)
	{
		//Finds the biggest group of units attacking their target (tar)
		if (u_Current == u_Human && u_Current.getTarget() == tar && u_Current.isTargetInRange())
		{
			int group = ai_CPUIADT(u_Current.getPosDim().x, u_Current.getPosDim().y, u_Current.getAtkRad(), u_Current.getAtkRad(), tar);
			if (group > biggestGroup) { biggestGroup = group; location = u_Current.getPosDim(); }
		}
	}
	return location;
}

//Find the biggest group of AI units doing whatever task
PosDim ai_FBAIG(Target tar)
{
	int biggestGroup = 0;
	PosDim location = { xSpace(101), ySpace(101), 1, 1 };

	for (int a = 0; a < unitSpawnIndex; a++)
	{
		//Finds the biggest group of units attacking their target (tar)
		if (u_Current == u_Invader && u_Current.getTarget() == tar)
		{
			int group = ai_CPUIADT(u_Current.getPosDim().x, u_Current.getPosDim().y, u_Current.getAtkRad(), u_Current.getAtkRad(), tar);
			if (group > biggestGroup) { biggestGroup = group; location = u_Current.getPosDim(); }
		}
	}
	return location;
}

//sets number of units to a task, regardless of current task.
void ai_SUT(const int numUnits, UnitTask task)
{
	for (int a = 0, b = numUnits; a < unitSpawnIndex; a++)
	{
		if (b == 0) { a = unitSpawnIndex; continue; }

		if (u_Current == u_Invader)
		{
			switch (task)
			{
			case FIGHT_UNITS:
				u_Current.setTarget(UNITS);
				break;
			case FIGHT_BUILDS:
				u_Current.setTarget(BUILDINGS);
				break;
			default:
				u_Current.setTarget(NOTHING);
				break;
			}
			b--;
		}
	}
}

//Removes a task from number of units if they have one
void ai_RUFM(const int numUnits, UnitTask task)
{
	for (int a = 0, b = numUnits; a < unitSpawnIndex; a++)
	{
		if (b == 0) { a = unitSpawnIndex; continue; }

		if (u_Current == u_Invader)
		{
			if (u_Current.getTarget() == NOTHING) { continue; }
			else { u_Current.setTarget(NOTHING); b--; }
		}
	}
}

//Takes number of units doing one task, and sets them to another
void ai_SUTFUT(const int numUnits, UnitTask doThis, UnitTask notThis)
{
	Target task;
	Target task2;
	switch (notThis)
	{
	case FIGHT_UNITS: task = UNITS; break;
	case FIGHT_BUILDS: task = BUILDINGS; break;
	default: task = NOTHING; break;
	}

	switch (doThis)
	{
	case FIGHT_UNITS: task2 = UNITS; break;
	case FIGHT_BUILDS: task2 = BUILDINGS; break;
	default: task2 = NOTHING; break;
	}

	for (int a = 0, b = numUnits; a < unitSpawnIndex; a++)
	{
		
		if (b == 0) { a = unitSpawnIndex; continue; }

		if (u_Current == u_Invader)
		{
			if (u_Current.getTarget() == task)
			{
				u_Current.setTarget(task2);
				b--;
			}
		}
	}
}

//Sets all or a number of its units wihtin the passed area to the passed target,
//Note: this forces a retarget on all affected units.
void ai_SUTIA(float x, float y, float h, float w, Target tar, int cap = units + 1)
{
	int count = 0;
	for (int a = 0; a < unitSpawnIndex; a++)
	{
		if (u_Current == u_Invader)
		{
			PosDim pd = u_Current.getPosDim();
			if (pd.x >= x && pd.x <= x + w)
			{
				if (pd.y >= y && pd.y < y + h)
				{
					if (count < cap)
					{
						u_Current.setTarget(NOTHING);
						u_Current.setTarget(tar);
						count++;
					}
				}
			}
		}
	}
}

//Moves number of units doing nothing and moves them to the passed location
void ai_MU(const int numUnits, float tx, float ty)
{
	for (int a = 0, b = numUnits; a < unitSpawnIndex; a++)
	{
		if (b == 0) { a = unitSpawnIndex; continue; }
		if (u_Current == u_Invader && u_Current.getTarget() == NOTHING)
		{
			u_Current.setTargetCoords(tx, ty);
			b--;
		}
	}
}

//Determines where it can and can't build
void ai_CBS()
{
	// All: -1, 1 == RED
	// Towers: 3 == RED; 0, 2 == GREEN
	// Barracks: 0, 2 == RED; 3 == GREEN

	towers = 0;
	barrackSlots = 0;
	canBuildTower = false;
	canBuildBarracks = false;

	//Sets all values to -1, effectively defaulting the grid.
	//The positions with this value are "invisible" to the AI
	//if they are not ovewritten by the otehr buildings, similar
	//to how it works for the player.
	for (int a = 0; a < gridY; a++)
	{
		for (int b = 0; b < gridX; b++)
		{
			ai_bg[a][b] = -1;
		}
	}

	//Finds all its towers, then sets positions next to it to 
	//Green. This happens first so it can be over written by 
	//Buildings later down
	for (int a = 0; a < buildSpawnIndex; a++)
	{
		int x = b_Current.gridPos[1];
		int y = b_Current.gridPos[0];

		if (b_Current == b_InvaderTower)
		{
			if (y + 1 <  8) { ai_bg[y + 1][x] = 2; }
			if (y - 1 > -1) { ai_bg[y - 1][x] = 2; }
			if (x + 1 < 10) { ai_bg[y][x + 1] = 2; }
			if (x - 1 > -1) { ai_bg[y][x - 1] = 2; }
		}
	}

	//finds barracks. sets its grid pos to green. 
	for (int a = 0; a < buildSpawnIndex; a++)
	{
		int x = b_Current.gridPos[1];
		int y = b_Current.gridPos[0];

		if (b_Current == b_InvaderBarracks)
		{
			ai_bg[y][x] = 0;
		}
	}

	//Finds all towers again. 
	//Sets its current grid pos to blue if by its self, 
	//Or to red if accompanied.
	for (int a = 0; a < buildSpawnIndex; a++)
	{
		int x = b_Current.gridPos[1];
		int y = b_Current.gridPos[0];

		if (b_Current == b_InvaderTower)
		{
			if (ai_bg[y][x] == 0) { ai_bg[y][x] = 1; }
			else { ai_bg[y][x] = 3; }
		}
	}

	//Town center sets its pos to red.
	for (int a = 0; a < buildSpawnIndex; a++)
	{
		int x = b_Current.gridPos[1];
		int y = b_Current.gridPos[0];

		if (b_Current == b_InvaderTC)
		{
			ai_bg[y][x] = 1;
		}
	}

	//Tells the AI if it can build or not. 
	for (int a = 0; a < gridY; a++)
	{
		for (int b = 0; b < gridX; b++)
		{

			switch (ai_bg[a][b])
			{
			case 0: case 2: towers++; if (p_AI.getMoney() >= 200) { canBuildTower = true; } break;
			case 3: if (p_AI.getMoney() >= 350) { canBuildBarracks = true; } barrackSlots++; break;
			}
		}
	}

	if (barrackSlots >= 2) { canBuildTower = false; }
}

//Start training a certain number of units, will change based on availble room.
void ai_QU(int numUnits)
{
	int targetBarrack;
	int comp = 11;

	//This loop finds the barrack with the least units in its queue
	for (int a = 0; a < buildSpawnIndex; a++)
	{
		int unitsInQ = 0;

		if (b_Current == b_InvaderBarracks)
		{
			for (int b = 0; b < 10; b++)
			{
				if (b_Current.queue[b] != -1) { unitsInQ++; }
			}

			if (unitsInQ < comp)
			{
				comp = unitsInQ;
				targetBarrack = a;
			}
		}
	}

	for (; comp + numUnits > 10; numUnits--); //This reduces the added units to fit the queue

	for (int a = comp; a < comp + numUnits; a++)
	{
		b_AllDynam[targetBarrack].queue[a] = u_Invader.getID();
	}
}

//Build a tower
void ai_PT()
{
	bool frst = false;
	bool built = false;
	int rand;
	int variate = std::rand() % 3;

	bool ud = std::rand() % 2;

	rand = std::rand();
	rand %= towers;

	cout << "Tower slots: " << towers << " Probability: " << towers / 3 << " Side: " << ud << endl;

	
	if (ud)
	{
		for (int a = 0; a < gridY; a++)
		{
			for (int b = 0; b < gridX; b++)
			{
				int y = a;
				int x = b;

				if (built) { a = gridY, b = gridX; }

				if (ai_bg[a][b] == 2 || ai_bg[a][b] == 0)
				{

					cout << "rand = " << rand << endl;

					if (rand < towers / 2.5 && !built)
					{
						int variate = std::rand() % 3;
						spawnBuild(b_InvaderTower, xSpace(buildGrid[y][x] * 10 + 5 + variate), ySpace((buildGrid[y + 1][x] + y + 3 - x) * 10 - 5 - variate));
						built = true;
						p_AI.addMoney(-b_InvaderTower.getCost());
						cout << "Built at: (" << xSpace(buildGrid[y][x] * 10 + 5 + variate) << ", " << ySpace((buildGrid[y + 1][x] + y + 3 - x) * 10 - 5 - variate) << ")\n";
					}
					else
					{
						rand = std::rand();
						rand %= towers;

					}
				}
			}
		}
	}
	else
	{
		variate *= -1;
		for (int a = gridY - 1; a > 0; a--)
		{
			for (int b = gridX - 1; b > 0; b--)
			{
				int y = a;
				int x = b;

				if (built) { a = 0, b = 0; }

				if (ai_bg[a][b] == 2 || ai_bg[a][b] == 0)
				{

					cout << "rand = " << rand << endl;

					if (rand < towers / 4 && !built)
					{
						spawnBuild(b_InvaderTower, xSpace(buildGrid[y][x] * 10 + 5 + variate), ySpace((buildGrid[y + 1][x] + y + 3 - x) * 10 - 5 - variate));
						built = true;
						p_AI.addMoney(-b_InvaderTower.getCost());
						cout << "Built at: (" << xSpace(buildGrid[y][x] * 10 + 5 + variate) << ", " << ySpace((buildGrid[y + 1][x] + y + 3 - x) * 10 - 5 - variate) << ")\n";
					}
					else
					{
						rand = std::rand();
						rand %= towers;

					}
				}
			}
		}
	}
}

//Build a barracks
void ai_PB()
{
	bool ud = std::rand() % 2;

	if (ud)
	{
		for (int a = 0; a < gridY; a++)
		{
			for (int b = 0; b < gridX; b++)
			{
				int y = a;
				int x = b;

				if (ai_bg[a][b] == 3)
				{
					spawnBuild(b_InvaderBarracks, xSpace(buildGrid[y][x] * 10 + 5), ySpace((buildGrid[y + 1][x] + y + 3 - x) * 10 - 7));
					p_AI.addMoney(-b_InvaderBarracks.getCost());
					return;
				}
			}
		}
	}
	else
	{
		for (int a = gridY - 1; a > 0; a--)
		{
			for (int b = gridX - 1; b > 0; b--)
			{
				int y = a;
				int x = b;

				if (ai_bg[a][b] == 3)
				{
					spawnBuild(b_InvaderBarracks, xSpace(buildGrid[y][x] * 10 + 5), ySpace((buildGrid[y + 1][x] + y + 3 - x) * 10 - 7));
					p_AI.addMoney(-b_InvaderBarracks.getCost());
					return;
				}
			}
		}
	}
}

void ai_UU()
{

	//Refill the queues
	for (int a = 0; a < barracks; a++) { ai_QU(10); }

	//Should I send streams?
	PosDim closestUnit;
	PosDim ib;
	int count = 0;
	for (int a = 0; a < buildSpawnIndex; a++) { if (b_Current == b_InvaderTC) { ib = b_Current.getPosDim(); a = buildSpawnIndex; } }
	for (int a = 0; a < unitSpawnIndex; a++)
	{
		if (u_Current == u_Human)
		{
			closestUnit = u_Current.getPosDim();
			if (dist(ib, closestUnit) < sqrt(expo<float>(xSpace(25), 2) + expo<float>(ySpace(25), 2)))
			{
				count++;
			}
		}
	}

	if (count >= 10) { ai_SUTFUT(idleUnits, FIGHT_UNITS, IDLE); }
	else
	{
		if (firstRun && idleUnits >= 10) { ai_SUTFUT(idleUnits, FIGHT_UNITS, IDLE); firstRun = false; }
		else if (!firstRun && idleUnits >= 5) { ai_SUTFUT(idleUnits, FIGHT_UNITS, IDLE); }
	}


	//Should I attack buildings?
	if (units > playerUnits * 1.5)
	{
		if (unitCountSave < units) { targetingBuilds = false; }
		if (!targetingBuilds)
		{
			ai_SUTFUT(frac<int>(unitAttackers, 2, 3), FIGHT_BUILDS, FIGHT_UNITS);
			targetingBuilds = true;
		}

		unitCountSave = units;
	}
	else
	{
		targetingBuilds = false;
		for (int a = 0; a < unitSpawnIndex; a++)
		{
			int count = 0;
			if (u_Current == u_Invader && !u_Current.groupFlag)
			{
				for (int b = 0; b < buildSpawnIndex; b++)
				{
					if (b_AllDynam[b] == b_HumanBarracks || b_AllDynam[b] == b_HumanTC || b_AllDynam[b] == b_HumanTower)
					{
						if (dist(u_Current.getPosDim(), b_AllDynam[b].getPosDim()) <= u_Current.getAtkRad() * 2)
						{
							u_Current.groupFlag = true;
							b = buildSpawnIndex;
						}
					}
				}

				if (!u_Current.groupFlag) { continue; }

				//Searches for all units in its AtkRad.
				//If they weren't grouped, it adds them to its group.
				for (int b = 0; b < unitSpawnIndex; b++)
				{
					if (u_AllDynam[b] == u_Invader && !u_AllDynam[b].groupFlag)
					{
						if (dist(u_Current.getPosDim(), u_AllDynam[b].getPosDim()) <= u_Current.getAtkRad())
						{
							count++;
							u_AllDynam[b].groupFlag = true;
						}
					}
				}

				//If the group reaches or surpasses the specified cap,
				//Split the group.
				if (count >= 10)
				{
					ai_SUTIA(u_Current.getPosDim().x, u_Current.getPosDim().y, u_Current.getAtkRad(), u_Current.getAtkRad(), BUILDINGS, count / 3);
				}
			}
		}
	}


	//Should I defend my buildings?
	if (playerUnitsAttackingBuilds > 5)
	{
		float baseRange = u_Invader.getAtkRad() * 3;
		float rangeScale = playerUnitsAttackingBuilds / 5;
		PosDim group = ai_FBEG(BUILDINGS);
		ai_SUTIA(group.x, group.y, baseRange * rangeScale, baseRange * rangeScale, UNITS);
	}

	//Is there a group of units  I need to eliminate?
	if (playerUnitsAttackingUnits > 15)
	{
		float baseRange = u_Invader.getAtkRad() * 2;
		float rangeScale = playerUnitsAttackingBuilds / 7.5;
		PosDim group = ai_FBEG(UNITS);
		ai_SUTIA(group.x, group.y, baseRange * rangeScale, baseRange * rangeScale, UNITS);
	}


}

void ai_Run(float updateTime)
{
	if (tmr_Update >= updateTime)
	{
		money = p_AI.getMoney();
		cout << "AI Money: " << money << endl;

		tmr_Update = 0.0f;

		playerUnits = ai_CPU();
		playerUnitsAttackingBuilds = ai_CPUNB();
		playerUnitsAttackingUnits = ai_CPUAU();
		units = ai_CU();
		idleUnits = ai_CUDT(IDLE);
		unitAttackers = ai_CUDT(FIGHT_UNITS);
		barracks = ai_CB();

		ai_UU();
		ai_CBS();
		if (canBuildBarracks) { ai_PB(); }
		else if (canBuildTower) { ai_PT(); }

		for (int a = 0; a < unitSpawnIndex; a++) { u_Current.groupFlag = false; }
	}
	else { tmr_Update += getDeltaTime(); }
}