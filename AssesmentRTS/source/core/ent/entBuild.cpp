#include "ent.h"
#include "..\math.h"

int Building::getMaxQueue() { return maxQueue; }
int Building::getID() { return buildingID; }
float Building::getAtkRad() { return atkRad; }
float Building::getElapsedTrainTime() { return saveTrainTime; }
float Building::getAtkSpeed() { return atkSpeed; }
float Building::getLastAtk() { return lstAtk; }
float Building::getBaseTrainTime() { return baseTrainTime; }
float Building::getTrainTime() { return currentTrainTime; }
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
void Building::setTrainTime(float tt) { currentTrainTime = tt; }

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
	float Dist = xSpace(101);
	float comp = xSpace(101);
	PosDim targetPD = { 0, 0, -1 , -1 };
	PosDim closest;
	PosDim buildPD = b.getPosDim();

	for (int a = 0; a < unitSpawnIndex; a++)
	{
		if (b.getOwner() == u_Current.getOwner() || u_Current == u_Empty) { continue; }
		else
		{
			targetPD = u_Current.getPosDim();
			Dist = sqrt(expo<float>(abs(buildPD.x - targetPD.x), 2) + expo<float>(abs(buildPD.y - targetPD.y), 2));
			if (Dist < comp) { comp = Dist; closest = targetPD; }
		}
	}

	if (Dist > b.getAtkRad())
	{
		for (int a = 0; a < buildSpawnIndex; a++)
		{
			if (b.getOwner() == b_Current.getOwner() || b_Current == b_Empty) { continue; }
			else
			{
				targetPD = b_Current.getPosDim();
				Dist = dist(buildPD, targetPD);
				if (Dist < comp) { comp = Dist; closest = targetPD; }
			}
		}
	}

	if (comp < b.getAtkRad())
	{
		if (b.getLastAtk() >= b.getAtkSpeed())
		{
			b.setLastAtk(0.0f);
			spawnBullet(b, closest.x, closest.y);
		}
		else
		{
			b.setLastAtk(b.getLastAtk() + getDeltaTime());
		}
	}
}

void spawnBuild(Building b, float x, float y)
{
	PosDim pd;
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
		delete[]temp;
	}


	if ((rotation / 90) % 2 != 0) { b.setPosDim({ x, y, getBuildW(b), getBuildH(b) }); }
	else { b.setPosDim({ x, y, getBuildH(b), getBuildW(b) }); }

	pd = b.getPosDim();
	for (int y = 0; y < gridY; y++)
	{
		PosDim s;
		for (int x = 0; x < gridX; x++)
		{
			s.x = xSpace(buildGrid[y][x] * 10);
			s.y = ySpace((buildGrid[y][x] + y + 2 - x) * 10);
			s.h = s.y + ySpace(10);
			s.w = s.x + xSpace(10);

			if (pd.x > s.x && pd.x < s.w && pd.y > s.y && pd.y < s.h) 
			{ 
				b.gridPos[0] = y;
				b.gridPos[1] = x;
				x = gridX; y = gridY;
			}
		}
	}

	b_AllDynam[openIndex] = b;
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
	for (int a = 0; a < 10; a++)
	{
		if (b.queue[a] == -1) 
		{ 
			b.queue[a] = u.getID(); 
			return;
		}
	}
}

void updateQueue(Building &b)
{
	float trainTime = b.getBaseTrainTime();
	PosDim pd = b.getPosDim();

	if (b.queue[0] == -1) { return; }

	for (int a = 0; a < unitSpawnIndex; a++) { if (b.getOwner() == u_Current.getOwner()) { trainTime += 0.25f; } }

	b.setTrainTime(trainTime);

	if (b.getElapsedTrainTime() >= trainTime)
	{
		b.setElapsedTrainTime(0.0f);

		spawnUnit(findUnitBase(b.queue[0]), b.getOwner(), pd.x, pd.y - pd.h);
		
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