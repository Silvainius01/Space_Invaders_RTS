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

int tempCount = 0;
int money;
int playerUnits;
int playerUnitsNearMe;
int attackingPlayerUnits;
int units;
int idleUnits;
int unitAttackers;
int buildAttackers;
int trainingUnits;
int minUnits = 10;
int barracks;
int towers;
int ai_bg[gridY][gridX];
int tmr_Count = 0;

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

//Counts the number of player units near/in the base.
//It will know how many units are attacking its buildings in this area if told to count them
int ai_CPUNB(bool countBuildingAttackers)
{
	int count = 0;
	PosDim pd;
	PosDim bpd;

	for (int a = 0; a < unitSpawnIndex; a++)
	{
		if (u_Current == u_Human && u_Current.getTarget() == BUILDINGS)
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

//Counts how many units are being trained
int ai_CUIT()
{
	int count = 0;
	for (int a = 0; a < buildSpawnIndex; a++)
	{
		if (b_Current == b_InvaderBarracks)
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

	canBuildTower = false;
	canBuildBarracks = false;

	for (int a = 0; a < gridY; a++)
	{
		for (int b = 0; b < gridX; b++)
		{
			ai_bg[a][b] = -1;
		}
	}

	for (int a = 0; a < buildSpawnIndex; a++)
	{
		int x = b_Current.gridPos[1];
		int y = b_Current.gridPos[0];

		if (b_Current == b_InvaderTower)
		{
			if (y + 1 != 8)
			{
				drawBox(xSpace(buildGrid[y][x] * 10), ySpace((buildGrid[y + 1][x] + y + 3 - x) * 10), ySpace(10), xSpace(10), GREEN);
				ai_bg[y + 1][x] = 2;
			}
			if (y - 1 != -1)
			{
				drawBox(xSpace(buildGrid[y][x] * 10), ySpace((buildGrid[y - 1][x] + y + 1 - x) * 10), ySpace(10), xSpace(10), GREEN);
				ai_bg[y - 1][x] = 2;
			}
			if (x + 1 != 10)
			{
				drawBox(xSpace(buildGrid[y][x + 1] * 10), ySpace((buildGrid[y][x] + y + 2 - x) * 10), ySpace(10), xSpace(10), GREEN);
				ai_bg[y][x + 1] = 2;
			}
			if (x - 1 != -1)
			{
				drawBox(xSpace(buildGrid[y][x - 1] * 10), ySpace((buildGrid[y][x] + y + 2 - x) * 10), ySpace(10), xSpace(10), GREEN);
				ai_bg[y][x - 1] = 2;
			}
		}
	}

	for (int a = 0; a < buildSpawnIndex; a++)
	{
		int x = b_Current.gridPos[1];
		int y = b_Current.gridPos[0];

		if (b_Current == b_InvaderBarracks)
		{
			ai_bg[y][x] = 0;
		}
	}

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

	for (int a = 0; a < buildSpawnIndex; a++)
	{
		int x = b_Current.gridPos[1];
		int y = b_Current.gridPos[0];

		if (b_Current == b_InvaderTC)
		{
			ai_bg[y][x] = 1;
		}
	}

	for (int a = 0; a < gridY; a++)
	{
		for (int b = 0; b < gridX; b++)
		{
			switch (ai_bg[a][b])
			{
			case 0: case 2: canBuildTower = true; break;
			case 3: canBuildBarracks = true; break;
			}
		}
	}
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
	
}

//Build a barracks
void ai_PB()
{

}

void ai_CalcUnits()
{
	if (playerUnits <= 12) { minUnits = 10; }
	else if (playerUnits > 12 && playerUnits < 18) { minUnits = 15; }
	else if (playerUnits >= 18 && playerUnits <= 22) { minUnits = 20; }
	else if (playerUnits > 22 && playerUnits <= 28) { minUnits = 30; }
	else { minUnits = playerUnits + 5; }

	if (units == 0 && trainingUnits == 0)
	{
		for (int a = 0; a < barracks; a++) { ai_QU(15 / barracks); }
		return;
	}

	if (units + trainingUnits < minUnits)
	{
		int train = minUnits - units - trainingUnits;
		for (int a = 0; a < barracks; a++) { ai_QU(train / barracks); }
	}

	if (playerUnits == 0) { ai_SUT(units, FIGHT_BUILDS); }
	else if (playerUnits > 20 && playerUnits < 30) { ai_SUT(units, FIGHT_UNITS); }
	else if (playerUnits >= 30) { ai_SUT(units, FIGHT_UNITS); }
	else if (playerUnits < 10)
	{
		int set = frac<int>(units, 1, 3);
		ai_SUT(units, FIGHT_BUILDS);
		ai_SUT(set, FIGHT_UNITS);
	}
	else
	{
		ai_SUT(units, FIGHT_UNITS);
		ai_SUT(units / 2, FIGHT_BUILDS);
	}

	ai_SUTFUT(idleUnits, FIGHT_UNITS, IDLE);
}
void ai_CalcBuilds()
{
	ai_CBS();
	if (money < b_InvaderTower.getCost()) { canBuildTower = false; return; }

	if (canBuildTower || canBuildBarracks)
	{
		if (canBuildTower)
		{
			int bogp[2];
			int punt = 0;
			int comp = 0;
			int val = 0;

			for (int a = 0; a < buildSpawnIndex; a++)
			{
				if (b_Current == b_InvaderTower)
				{
					PosDim pd = b_Current.getPosDim();
					val = ai_CPUIA(pd.x - (pd.x / 2), pd.y - (pd.y / 2), b_Current.getAtkRad(), b_Current.getAtkRad());
					if (val > comp)
					{
						comp = val;
						bogp[0] = b_Current.gridPos[0];
						bogp[1] = b_Current.gridPos[1];
					}
					punt += val;
				}
			}

			if (punt >= unitAttackers / 2)
			{
				int openSlots[4] = { -1, -1, -1, -1 };
				int index = 0;
				int comp = 0;
				int y = bogp[0];
				int x = bogp[1];

				if (y + 1 < 8 && y - 1 > -1)
				{
					if (buildGrid[y + 1][x] == 0 || buildGrid[y + 1][x] == 2)
						openSlots[2] = ai_CPUIA(xSpace(buildGrid[y][x] * 10), ySpace((buildGrid[y + 1][x] + y + 3 - x) * 10), ySpace(10), xSpace(10));
					if (buildGrid[y - 1][x] == 0 || buildGrid[y - 1][x] == 2)
						openSlots[0] = ai_CPUIA(xSpace(buildGrid[y][x] * 10), ySpace((buildGrid[y - 1][x] + y + 1 - x) * 10), ySpace(10), xSpace(10));
				}
				if (x + 1 < 10 && x - 1 > -1)
				{
					if (buildGrid[y][x + 1] == 0 || buildGrid[y][x + 1] == 2)
						openSlots[1] = ai_CPUIA(xSpace(buildGrid[y][x + 1] * 10), ySpace((buildGrid[y][x] + y + 2 - x) * 10), ySpace(10), xSpace(10));
					if (buildGrid[y][x - 1] == 0 || buildGrid[y][x - 1] == 2)
						openSlots[3] = ai_CPUIA(xSpace(buildGrid[y][x - 1] * 10), ySpace((buildGrid[y][x] + y + 2 - x) * 10), ySpace(10), xSpace(10));
				}

				for (int a = 0; a < 4; a++)
				{
					if (openSlots[a] > comp) { comp = openSlots[a]; index = a; }
				}

				if (openSlots[index] > -1)
				{
					switch (index)
					{
					case 0: spawnBuild(b_InvaderTower, xSpace(buildGrid[y][x] * 10) + (xSpace(buildGrid[y][x] * 10) / 2), ySpace((buildGrid[y - 1][x] + y + 1 - x) * 10) + (ySpace((buildGrid[y - 1][x] + y + 1 - x) * 10) / 2)); break;
					case 1: spawnBuild(b_InvaderTower, xSpace(buildGrid[y][x + 1] * 10) + (xSpace(buildGrid[y][x + 1] * 10) / 2), ySpace((buildGrid[y][x] + y + 2 - x) * 10) + (ySpace((buildGrid[y][x] + y + 2 - x) * 10) / 2)); break;
					case 2: spawnBuild(b_InvaderTower, xSpace(buildGrid[y][x] * 10) + (xSpace(buildGrid[y][x] * 10) / 2), ySpace((buildGrid[y + 1][x] + y + 3 - x) * 10) + (ySpace((buildGrid[y + 1][x] + y + 3 - x) * 10) / 2)); break;
					case 3: spawnBuild(b_InvaderTower, xSpace(buildGrid[y][x - 1] * 10) + (xSpace(buildGrid[y][x - 1] * 10) / 2), ySpace((buildGrid[y][x] + y + 2 - x) * 10) + (ySpace((buildGrid[y][x] + y + 2 - x) * 10) / 2)); break;
					}
				}
			}
		}

		if (money < b_InvaderBarracks.getCost()) { canBuildBarracks = false; }

		if (canBuildBarracks)
		{
			
		}
	}
}
void ai_Run(float updateTime)
{
	if (tmr_Update >= updateTime)
	{
		money = p_AI.getMoney();

		tmr_Update = 0.0f;

		playerUnits = ai_CPU();
		units = ai_CU();
		idleUnits = ai_CUDT(IDLE);
		unitAttackers = ai_CUDT(FIGHT_UNITS);
		buildAttackers = ai_CUDT(FIGHT_BUILDS);
		trainingUnits = ai_CUIT();
		barracks = ai_CB();
		towers = ai_CT();

		ai_CalcUnits();
		ai_CalcBuilds();
	}
	else { tmr_Update += getDeltaTime(); }

}