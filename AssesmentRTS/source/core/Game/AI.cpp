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
	for (int a = 0; a < unitSpawnIndex; a++)
	{
		if (u_Current == u_Human)
		{
			pd = u_Current.getPosDim();

			if (pd.x > xSpace(65) && pd.y < ySpace(55))
			{
				if (countBuildingAttackers && u_Current.getTarget() == BUILDINGS) { count++; }
				else { count++; }
			}
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
	int blerg;
	for (int a = 0; a != 100; a++)
	{
		//ai_CPUIA();
	}
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

}
void ai_Run(float updateTime)
{
	if (tmr_Update >= updateTime)
	{
		money = p_AI.getMoney();

		tmr_Update = 0.0f;

		playerUnits = ai_CPU();
		playerUnitsNearMe = ai_CPUNB(false);
		attackingPlayerUnits = ai_CPUNB(true);
		units = ai_CU();
		idleUnits = ai_CUDT(IDLE);
		unitAttackers = ai_CUDT(FIGHT_UNITS);
		buildAttackers = ai_CUDT(FIGHT_BUILDS);
		trainingUnits = ai_CUIT();
		barracks = ai_CB();
		towers = ai_CT();

		ai_CalcUnits();
	}
	else { tmr_Update += getDeltaTime(); }

}