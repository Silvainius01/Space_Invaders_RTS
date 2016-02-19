#include "./ent.h"
#include "../math.h"
#include "../Game/game.h"

int Resource::getID() { return resourceID; }
float Resource::getResistance() { return resistance; }
void Resource::setID(int id) { resourceID = id; }
void Resource::setResistance(float r) { resistance = r; }

int resourceSpawnIndex = 1;
Resource r_AllBase[resourceIndex];
Resource *r_AllDynam = NULL;

float getResourceW(Resource &r)
{
	switch (r.getID())
	{
	case 0: return wi_Steel;
	case 1: return wi_Food;
	default: cout << "Missing Resource Width: " << r.getID() << endl;
	}
}
float getReourceH(Resource &r)
{
	switch (r.getID())
	{
	case 0: return hi_Steel;
	case 1: return hi_Food;
	default: cout << "Missing Resource Height: " << r.getID() << endl;
	}
}

void spawnResource(Resource r, float x, float y)
{
	bool isIndexOpen = false;
	int openIndex;

	for (int a = 0; a < resourceSpawnIndex; a++)
	{
		if (r_Current == r_Empty)
		{
			isIndexOpen = true;
			openIndex = a;
			a = resourceSpawnIndex;
		}
	}

	if (!isIndexOpen)
	{
		if (resourceSpawnIndex <= 0) { resourceSpawnIndex = 1; }
		Resource *temp = new Resource[resourceSpawnIndex];
		for (int a = 0; a < resourceSpawnIndex; a++) { temp[a] = r_Current; }
		openIndex = resourceSpawnIndex;
		resourceSpawnIndex++;
		delete[] r_AllDynam;
		r_AllDynam = new Resource[resourceSpawnIndex];
		for (int a = 0; a < openIndex; a++) { r_Current = temp[a]; }
		delete[] temp;
	}

	r.setPosDim({ x, y, getReourceH(r), getResourceW(r) });
	r_AllDynam[openIndex] = r;
}

void killResource()
{
	for (int a = 0; a < resourceSpawnIndex; a++)
	{
		if (r_Current.getSelected())
		{
			r_Current = r_Empty;
		}
	}
}