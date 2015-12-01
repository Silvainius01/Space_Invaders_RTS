#include "ent.h"
#include "..\math.h"

int Building::getMaxQueue() { return maxQueue; }
int Building::getID() { return buildingID; }
float Building::getAtkRad() { return atkRad; }
float Building::getElapsedTrainTime() { return saveTrainTime; }
float Building::getAtkSpeed() { return atkSpeed; }
float Building::getLastAtk() { return lstAtk; }
void Building::setMaxQueue(int mq)
{
	if (mq == 0) { mq = 1; }
	maxQueue = 10;
	for (int a = 0; a < maxQueue; a++)
	{
		queue[a] = -1;
	}
}
void Building::setAtkRad(float ar) { atkRad = ar; }
void Building::setID(int ID) { buildingID = ID; }
void Building::setElapsedTrainTime(float ett) { saveTrainTime = ett; }
void Building::setAtkSpeed(float as) { atkSpeed = as; }
void Building::setLastAtk(float la) { lstAtk = la; }

int buildSpawnIndex = 1;
Building b_AllBase[buildingIndex];
Building *b_AllDynam = NULL;


float getBuildW(Building &b)
{
	int idMatch;

	for (int a = 0; a < buildingIndex; a++)
	{
		if (b == b_AllBase[a])
		{
			idMatch = b_AllBase[a].getID();
			a = buildingIndex;
		}
		else { idMatch = -1; }
	}

	switch (idMatch)
	{
	case 0: case 3: return wi_TownCenter;
	case 1: case 4: return wi_Tower;
	case 2: case 5: return wi_Barracks;
	default: cout << "\nDefaulted with: " << idMatch; return 9;
	}
}
float getBuildH(Building &b)
{
	int idMatch;

	for (int a = 0; a < buildingIndex; a++)
	{
		if (b == b_AllBase[a])
		{
			idMatch = b_AllBase[a].getID();
			a = buildingIndex;
		}
		else { idMatch = -1; }
	}

	switch (idMatch)
	{
	case 0: case 3: return hi_TownCenter;
	case 1: case 4: return hi_Tower;
	case 2: case 5: return hi_Barracks;
	default: cout << "\nDefaulted with: " << idMatch; return 9;
	}
}

void checkForTargets(Building &b)
{
	float dist;
	float comp = 900000;
	int closest;
	bool inited = false;
	PosDim unitPD;
	PosDim buildPD = b.getPosDim();

	for (int a = 0; a < unitSpawnIndex; a++)
	{
		if (u_Current == u_Invader || u_Current == u_Empty) { continue; }
		else
		{
			unitPD = u_Current.getPosDim();
			dist = sqrt(expo<float>(abs(buildPD.x - unitPD.x), 2) + expo<float>(abs(buildPD.y - unitPD.y), 2));
			if (dist < comp) { comp = dist; }
		}
	}

	if (comp < b.getAtkRad())
	{
		cout << b.getAtkSpeed() << ", " << b.getLastAtk() << endl;
		if (b.getLastAtk() >= b.getAtkSpeed())
		{
			b.setLastAtk(0.0f);
			spawnBullet(b, unitPD.x, unitPD.y);
		}
		else
		{
			b.setLastAtk(b.getLastAtk() + getDeltaTime());
		}
	}
}

void spawnBuild(Building b, float x, float y)
{
	bool isIndexOpen = false;
	int openIndex;

	for (int a = 0; a < buildSpawnIndex; a++)
	{
		if (b_Current == b_Empty)
		{
			isIndexOpen = true;
			openIndex = a;
			a = buildSpawnIndex;
		}
	}

	if (!isIndexOpen)
	{
		Building *temp = new Building[buildSpawnIndex];
		for (int a = 0; a < buildSpawnIndex; a++) { temp[a] = b_AllDynam[a]; }
		openIndex = buildSpawnIndex;
		buildSpawnIndex++;
		delete[] b_AllDynam;
		b_AllDynam = new Building[buildSpawnIndex];
		for (int a = 0; a < openIndex; a++) { b_Current = temp[a]; }
		b.setPosDim({ x, y, getBuildH(b), getBuildW(b) });
		b_AllDynam[openIndex] = b;
		delete[]temp;
	}
	else
	{
		b.setPosDim({ x, y, getBuildH(b), getBuildW(b) });
		b_AllDynam[openIndex] = b;
	}
}
void killBuild()
{
	for (int a = 0; a < buildSpawnIndex; a++)
	{
		if (b_Current.getSelected())
		{
			b_Current.setHP(0);
		}
	}
}

void addToQueue(Unit u, Building &b)
{
	for (int a = 0; a < b.getMaxQueue(); a++)
	{
		if (b.queue[a] == -1) { b.queue[a] = u.getID(); a = b.getMaxQueue(); }
	}

	if (b.queue[b.getMaxQueue() - 1] != -1) { cout << "Queue is full\n"; }
}

void updateQueue(Building &b)
{
	float trainTime = 3.0f;
	PosDim pd = b.getPosDim();

	if (b.queue[0] == -1) { return; }

	if (b == b_HumanBarracks)
	{
		for (int a = 0; a < unitSpawnIndex; a++)
		{
			if (u_Current == u_Human) { trainTime += 0.25f; }
		}
	}
	else if (b == b_InvaderBarracks)
	{
		for (int a = 0; a < unitSpawnIndex; a++)
		{
			if (u_Current == u_Invader) { trainTime += 0.25f; }
		}
	}
	else { return; }

	cout << b.getElapsedTrainTime() << endl;

	if (b.getElapsedTrainTime() >= trainTime)
	{
		b.setElapsedTrainTime(0.0f);
		switch (b.queue[0])
		{
		case 0:
			spawnUnit(u_Human, pd.x, pd.y - pd.h);
			break;
		case 1:
			spawnUnit(u_Invader, pd.x, pd.y - pd.h);
			break;
		}

		
		
		for (int a = 0; a < b.getMaxQueue() - 1; a++)
		{
			b.queue[a] = b.queue[a + 1];
		}
		b.queue[b.getMaxQueue() - 1] = -1;
	}
	else
	{
		b.setElapsedTrainTime(b.getElapsedTrainTime() + getDeltaTime());
	}
}