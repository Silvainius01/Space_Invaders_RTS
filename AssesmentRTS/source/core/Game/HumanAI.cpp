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

bool canBuildTowerh = false;
bool canBuildBarracksh = false;

int tempCounth = 0;
int moneyh;
int playerUnitsh;
int playerUnitsNearMeh;
int attackingPlayerUnitsh;
int unitsh;
int idleUnitsh;
int unitAttackersh;
int buildAttackersh;
int trainingUnitsh;
int minUnitsh = 10;
int barracksh;
int barrackSlotsh;
int towersh;
int aih_bg[gridY][gridX];
int tmrh_Count = 0;

float aih_TimerStart = 0.0f;
float tmr_Updateh = aih_TimerStart;

//Counts all player units
int aih_CU()
{
	int count = 0;
	for (int a = 0; a < unitSpawnIndex; a++)
	{
		if (u_Current == u_Human) { count++; }
	}

	return count;
}

//Count units doing whatever task
int aih_CUDT(UnitTask ut)
{
	int count = 0;
	switch (ut)
	{
	case IDLE:
		for (int a = 0; a < unitSpawnIndex; a++) { if (u_Current == u_Human && u_Current.getTarget() == NOTHING) { count++; } } break;
	case FIGHT_UNITS:
		for (int a = 0; a < unitSpawnIndex; a++) { if (u_Current == u_Human && u_Current.getTarget() == UNITS) { count++; } } break;
	case FIGHT_BUILDS:
		for (int a = 0; a < unitSpawnIndex; a++) { if (u_Current == u_Human && u_Current.getTarget() == BUILDINGS) { count++; } } break;
	}

	return count;
}

//Count number of AI units
int aih_CPU()
{
	int count = 0;
	for (int a = 0; a < unitSpawnIndex; a++)
	{
		if (u_Current == u_Invader)
		{
			count++;
		}
	}

	return count;
}

//Counts the number of player units near/in the base.
//It will know how many units are attacking its buildings in this area if told to count them
int aih_CPUNB(bool countBuildingAttackers)
{
	int count = 0;
	PosDim pd;
	PosDim bpd;

	for (int a = 0; a < unitSpawnIndex; a++)
	{
		if (u_Current == u_Invader && u_Current.getTarget() == BUILDINGS)
		{
			pd = u_Current.getPosDim();
			bpd = b_AllDynam[u_Current.getTargetedBuild()].getPosDim();

			int dist = sqrt(expo<float>(abs(pd.x - bpd.x), 2) + expo<float>(abs(pd.y - bpd.y), 2));

			if (dist <= u_Current.getAtkRad()) { count++; }
		}
	}

	return count;
}

//Counts the number of player units that are or aren't in an area
int aih_CPUIA(float x, float y, float h, float w, bool countOut = false)
{
	int count = 0;
	PosDim pd;
	for (int a = 0; a < unitSpawnIndex; a++)
	{
		if (u_Current == u_Invader)
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

//Counts the number of AI units that are or aren't in an area
int aih_CMUIA(float x, float y, float h, float w, bool countOut = false)
{
	int count = 0;
	PosDim pd;
	for (int a = 0; a < unitSpawnIndex; a++)
	{
		if (u_Current == u_Human)
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

//Counts how many units are being trained
int aih_CUIT()
{
	int count = 0;
	for (int a = 0; a < buildSpawnIndex; a++)
	{
		if (b_Current == b_HumanBarracks)
		{
			for (int b = 0; b < 10; b++)
			{
				if (b_Current.queue[b] != -1) { count++; }
				else { b = 10; }
			}
		}
	}

	return count;
}

//Counts its barracks
int aih_CB()
{
	int count = 0;
	for (int a = 0; a < buildSpawnIndex; a++) { if (b_Current == b_HumanBarracks) { count++; } }
	return count;
}

//Counts its towers
int aih_CT()
{
	int count = 0;
	for (int a = 0; a < buildSpawnIndex; a++) { if (b_Current == b_HumanTower) { count++; } }
	return count;
}

//sets number of units to a task, regardless of current task.
void aih_SUT(const int numUnits, UnitTask task)
{
	for (int a = 0, b = numUnits; a < unitSpawnIndex; a++)
	{
		if (b == 0) { a = unitSpawnIndex; continue; }

		if (u_Current == u_Human)
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
void aih_RUFM(const int numUnits, UnitTask task)
{
	for (int a = 0, b = numUnits; a < unitSpawnIndex; a++)
	{
		if (b == 0) { a = unitSpawnIndex; continue; }

		if (u_Current == u_Human)
		{
			if (u_Current.getTarget() == NOTHING) { continue; }
			else { u_Current.setTarget(NOTHING); b--; }
		}
	}
}

//Takes number of units doing one task, and sets them to another
void aih_SUTFUT(const int numUnits, UnitTask doThis, UnitTask notThis)
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

		if (u_Current == u_Human)
		{
			if (u_Current.getTarget() == task)
			{
				u_Current.setTarget(task2);
				b--;
			}
		}
	}
}

//Moves number of units doing nothing and moves them to the passed location
void aih_MU(const int numUnits, float tx, float ty)
{
	for (int a = 0, b = numUnits; a < unitSpawnIndex; a++)
	{
		if (b == 0) { a = unitSpawnIndex; continue; }
		if (u_Current == u_Human && u_Current.getTarget() == NOTHING)
		{
			u_Current.setTargetCoords(tx, ty);
			b--;
		}
	}
}

//Determines where it can and can't build
void aih_CBS()
{
	// All: -1, 1 == RED
	// Towers: 3 == RED; 0, 2 == GREEN
	// Barracks: 0, 2 == RED; 3 == GREEN

	towersh = 0;
	barrackSlotsh = 0;
	canBuildTowerh = false;
	canBuildBarracksh = false;

	//Sets all values to -1, effectively defaulting the grid.
	//The positions with this value are "invisible" to the AI
	//if they are not ovewritten by the otehr buildings, similar
	//to how it works for the player.
	for (int a = 0; a < gridY; a++)
	{
		for (int b = 0; b < gridX; b++)
		{
			aih_bg[a][b] = -1;
		}
	}

	//Finds all its towers, then sets positions next to it to 
	//Green. This happens first so it can be over written by 
	//Buildings later down
	for (int a = 0; a < buildSpawnIndex; a++)
	{
		int x = b_Current.gridPos[1];
		int y = b_Current.gridPos[0];

		if (b_Current == b_HumanTower)
		{
			if (y + 1 <  8) { aih_bg[y + 1][x] = 2; }
			if (y - 1 > -1) { aih_bg[y - 1][x] = 2; }
			if (x + 1 < 10) { aih_bg[y][x + 1] = 2; }
			if (x - 1 > -1) { aih_bg[y][x - 1] = 2; }
		}
	}

	//finds barracks. sets its grid pos to green. 
	for (int a = 0; a < buildSpawnIndex; a++)
	{
		int x = b_Current.gridPos[1];
		int y = b_Current.gridPos[0];

		if (b_Current == b_HumanBarracks)
		{
			aih_bg[y][x] = 0;
		}
	}

	//Finds all towers again. 
	//Sets its current grid pos to blue if by its self, 
	//Or to red if accompanied.
	for (int a = 0; a < buildSpawnIndex; a++)
	{
		int x = b_Current.gridPos[1];
		int y = b_Current.gridPos[0];

		if (b_Current == b_HumanTower)
		{
			if (aih_bg[y][x] == 0) { aih_bg[y][x] = 1; }
			else { aih_bg[y][x] = 3; }
		}
	}

	//Town center sets its pos to red.
	for (int a = 0; a < buildSpawnIndex; a++)
	{
		int x = b_Current.gridPos[1];
		int y = b_Current.gridPos[0];

		if (b_Current == b_HumanBarracks)
		{
			aih_bg[y][x] = 1;
		}
	}

	//Tells the AI if it can build or not. 
	for (int a = 0; a < gridY; a++)
	{
		for (int b = 0; b < gridX; b++)
		{

			switch (aih_bg[a][b])
			{
			case 0: case 2: towersh++; if (p_Player.getMoney() >= 200) { canBuildTowerh = true; } break;
			case 3: if (p_Player.getMoney() >= 350) { canBuildBarracksh = true; } barrackSlotsh++; break;
			}
		}
	}

	if (barrackSlotsh >= 2) { canBuildTowerh = false; }
}

//Start training a certain number of units, will change based on availble room.
void aih_QU(int numUnits)
{
	int targetBarrack;
	int comp = 11;

	//This loop finds the barrack with the least units in its queue
	for (int a = 0; a < buildSpawnIndex; a++)
	{
		int unitsInQ = 0;

		if (b_Current == b_HumanBarracks)
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
		b_AllDynam[targetBarrack].queue[a] = u_Human.getID();
	}
}

//Build a tower
void aih_PT()
{
	bool frst = false;
	bool built = false;
	int rand;
	int variate = std::rand() % 3;

	bool ud = std::rand() % 2;

	rand = std::rand();
	rand %= towersh;

	if (ud)
	{
		for (int a = 0; a < gridY; a++)
		{
			for (int b = 0; b < gridX; b++)
			{
				int y = a;
				int x = b;

				if (built) { a = gridY, b = gridX; }

				if (aih_bg[a][b] == 2 || aih_bg[a][b] == 0)
				{
					if (rand < towersh / 2.5 && !built)
					{
						int variate = std::rand() % 3;
						spawnBuild(b_HumanTower, xSpace(buildGrid[y][x] * 10 + 5 + variate), ySpace((buildGrid[y + 1][x] + y + 3 - x) * 10 - 5 - variate));
						built = true;
						p_Player.addMoney(-b_HumanTower.getCost());
					}
					else
					{
						rand = std::rand();
						rand %= towersh;

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

				if (aih_bg[a][b] == 2 || aih_bg[a][b] == 0)
				{
					if (rand < towersh / 4 && !built)
					{
						spawnBuild(b_HumanTower, xSpace(buildGrid[y][x] * 10 + 5 + variate), ySpace((buildGrid[y + 1][x] + y + 3 - x) * 10 - 5 - variate));
						built = true;
						p_Player.addMoney(-b_HumanTower.getCost());
					}
					else
					{
						rand = std::rand();
						rand %= towersh;

					}
				}
			}
		}
	}
}

//Build a barracks
void aih_PB()
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

				if (aih_bg[a][b] == 3)
				{
					spawnBuild(b_HumanBarracks, xSpace(buildGrid[y][x] * 10 + 5), ySpace((buildGrid[y + 1][x] + y + 3 - x) * 10 - 7));
					p_Player.addMoney(-b_HumanBarracks.getCost());
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

				if (aih_bg[a][b] == 3)
				{
					spawnBuild(b_HumanBarracks, xSpace(buildGrid[y][x] * 10 + 5), ySpace((buildGrid[y + 1][x] + y + 3 - x) * 10 - 7));
					p_Player.addMoney(-b_HumanBarracks.getCost());
					return;
				}
			}
		}
	}
}

void aih_CalcUnits()
{
	if (playerUnitsh <= 12) { minUnitsh = 10; }
	else if (playerUnitsh > 12 && playerUnitsh < 18) { minUnitsh = 15; }
	else if (playerUnitsh >= 18 && playerUnitsh <= 22) { minUnitsh = 20; }
	else if (playerUnitsh > 22 && playerUnitsh <= 28) { minUnitsh = 30; }
	else { minUnitsh = playerUnitsh + 5; }

	if (unitsh == 0 && trainingUnitsh == 0)
	{
		for (int a = 0; a < barracksh; a++) { aih_QU(15 / barracksh); }
		return;
	}

	if (unitsh + trainingUnitsh < minUnitsh)
	{
		int train = minUnitsh - unitsh - trainingUnitsh;
		for (int a = 0; a < barracksh; a++) { aih_QU(train / barracksh); }
	}

	if (playerUnitsh == 0) { aih_SUT(unitsh, FIGHT_BUILDS); }
	else if (playerUnitsh > 20 && playerUnitsh < 30) { aih_SUT(unitsh, FIGHT_UNITS); }
	else if (playerUnitsh >= 30) { aih_SUT(unitsh, FIGHT_UNITS); }
	else if (playerUnitsh < 10)
	{
		int set = frac<int>(unitsh, 1, 3);
		aih_SUT(unitsh, FIGHT_BUILDS);
		aih_SUT(set, FIGHT_UNITS);
	}
	else
	{
		aih_SUT(unitsh, FIGHT_UNITS);
		aih_SUT(unitsh / 2, FIGHT_BUILDS);
	}

	aih_SUTFUT(idleUnitsh, FIGHT_UNITS, IDLE);
}

void aih_Run(float updateTime)
{
	if (tmr_Updateh >= updateTime)
	{
		moneyh = p_AI.getMoney();
		
		tmr_Updateh = 0.0f;



		playerUnitsh = aih_CPU();
		unitsh = aih_CU();
		idleUnitsh = aih_CUDT(IDLE);
		unitAttackersh = aih_CUDT(FIGHT_UNITS);
		buildAttackersh = aih_CUDT(FIGHT_BUILDS);
		trainingUnitsh = aih_CUIT();
		barracksh = aih_CB();

		aih_CalcUnits();
		aih_CBS();
		if (canBuildBarracksh) { aih_PB(); }
		else if (canBuildTowerh) { aih_PT(); }
	}
	else { tmr_Updateh += getDeltaTime(); }
}