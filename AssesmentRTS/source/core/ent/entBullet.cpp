#include "ent.h"
#include "../ui/ui.h"
#include "../math.h"
using namespace std;

float	Bullet::getSpeed() { return speed; }
float	Bullet::getSpeedX() { return speedX; }
float	Bullet::getSpeedY() { return speedY; }
float	Bullet::getTargetX() { return targetX; }
float	Bullet::getTargetY() { return targetY; }
int		Bullet::getID() { return bulletID; }
bool	Bullet::doesBulletHaveSlope() { return hasSlope; }
void	Bullet::setSpeed(float s) { speed = s; }
void	Bullet::setAxisSpeeds(float sx, float sy) { speedX = sx, speedY = sy, hasSlope = true; }
void	Bullet::setID(int ID) { bulletID = ID; }
void	Bullet::setTargetCoords(float tx, float ty) { targetX = tx, targetY = ty; }
void	Bullet::setSlope(bool hs) { hasSlope = hs; }

int bulletSpawnIndex = 1;
Bullet blt_AllBase[bulletIndex];
Bullet *blt_AllDynam;

float time = 0;

void spawnBullet(Unit u, float tx, float ty)
{
	PosDim unitPosDim = u.getPosDim();
	Bullet blt;
	bool isIndexOpen = false;
	int openIndex;

	switch (u.getID())
	{
	case 0:
		blt = blt_Human;
		break;
	case 1:
		blt = blt_Invader;
		break;
	default:
		blt = blt_Empty;
		break;
	}

	for (int a = 0; a < bulletSpawnIndex; a++)
	{
		if (blt_Current == blt_Empty)
		{
			isIndexOpen = true;
			openIndex = a;
			a = bulletSpawnIndex;
		}
	}

	if (!isIndexOpen)
	{
		Bullet *temp = new Bullet[bulletSpawnIndex];
		for (int a = 0; a < bulletSpawnIndex; a++) { temp[a] = blt_Current; }
		openIndex = bulletSpawnIndex;
		bulletSpawnIndex++;
		delete[] blt_AllDynam;
		blt_AllDynam = new Bullet[bulletSpawnIndex];
		for (int a = 0; a < openIndex; a++) { blt_Current = temp[a]; }
		delete[] temp;
	}

	blt_AllDynam[openIndex] = blt;
	blt_AllDynam[openIndex].setPosDim({ unitPosDim.x, unitPosDim.y, 1, 1 });
	blt_AllDynam[openIndex].setTargetCoords(tx, ty);
	blt_AllDynam[openIndex].setDMG(u.getDMG());
}

void spawnBullet(Building b, float tx, float ty)
{
	PosDim unitPosDim = b.getPosDim();
	Bullet blt;
	bool isIndexOpen = false;
	int openIndex;

	switch (b.getID())
	{
	case 1:
		blt = blt_Human;
		break;
	case 4:
		blt = blt_Invader;
		break;
	default:
		blt = blt_Empty;
		break;
	}

	for (int a = 0; a < bulletSpawnIndex; a++)
	{
		if (blt_Current == blt_Empty)
		{
			isIndexOpen = true;
			openIndex = a;
			a = bulletSpawnIndex;
		}
	}

	if (!isIndexOpen)
	{
		Bullet *temp = new Bullet[bulletSpawnIndex];
		for (int a = 0; a < bulletSpawnIndex; a++) { temp[a] = blt_Current; }
		openIndex = bulletSpawnIndex;
		bulletSpawnIndex++;
		delete[] blt_AllDynam;
		blt_AllDynam = new Bullet[bulletSpawnIndex];
		for (int a = 0; a < openIndex; a++) { blt_Current = temp[a]; }
		delete[] temp;
	}

	blt_AllDynam[openIndex] = blt;
	blt_AllDynam[openIndex].setPosDim({ unitPosDim.x, unitPosDim.y, 1, 1 });
	blt_AllDynam[openIndex].setTargetCoords(tx, ty);
	blt_AllDynam[openIndex].setDMG(b.getDMG());
}

void moveBullet(Bullet &blt)
{
	PosDim pd = blt.getPosDim();
	float tx = blt.getTargetX();
	float ty = blt.getTargetY();

	if (pd.x - tx < 1 && pd.x - tx > -1)
	{
		if (pd.y - ty < 1 && pd.y - ty > -1)
		{
			blt = blt_Empty;
			return;
		}
	}

	if (blt.doesBulletHaveSlope())
	{
		if (tx < pd.x) { pd.x -= blt.getSpeedX() * getDeltaTime(); }
		else { pd.x += blt.getSpeedX() * getDeltaTime(); }
		if (ty < pd.y) { pd.y -= blt.getSpeedY() * getDeltaTime(); }
		else { pd.y += blt.getSpeedY() * getDeltaTime(); }
		blt.setPosDim(pd);
	}
	else
	{
		float time = (sqrt(expo<float>(abs(tx - pd.x), 2) + expo<float>(abs(ty - pd.y), 2))) / (blt.getSpeed());
		blt.setAxisSpeeds(abs(tx - pd.x) / time, abs(ty - pd.y) / time);

		if (tx < pd.x) { pd.x -= blt.getSpeedX() * getDeltaTime(); }
		else { pd.x += blt.getSpeedX() * getDeltaTime(); }
		if (ty < pd.y) { pd.y -= blt.getSpeedY() * getDeltaTime(); }
		else { pd.y += blt.getSpeedY() * getDeltaTime(); }
		blt.setPosDim(pd);
	}
}

void checkCollision(Bullet &u, int index, bool isBuild = false)
{
	PosDim ua[2] = { u.getPosDim() };

	if (isBuild) { ua[1] = b_AllDynam[index].getPosDim(); }
	else { ua[1] = u_AllDynam[index].getPosDim(); }

	if (ua[0].x - (ua[0].w / 2) <= ua[1].x + (ua[1].w / 2) && ua[0].x + (ua[0].w / 2) >= ua[1].x - (ua[1].w / 2))
	{
		if (ua[0].y + (ua[0].h / 2) >= ua[1].y - (ua[1].h / 2) && ua[0].y - (ua[0].h / 2) <= ua[1].y + (ua[1].h / 2))
		{
			u = blt_Empty;
			if (isBuild) { b_AllDynam[index].setHP(b_AllDynam[index].getHP() - u.getDMG()); }
			else { u_AllDynam[index].setHP(u_AllDynam[index].getHP() - u.getDMG()); }
		}
	}
}
void bulletCollision()
{
	bool isBulletHuman;

	for (int a = 0; a < bulletSpawnIndex; a++)
	{
		PosDim pd = blt_Current.getPosDim();
		switch (blt_Current.getID())
		{
		case 0: case 2:
			isBulletHuman = true;
			break;
		case 1: case 3:
			isBulletHuman = false;
			break;
		default: continue;
		}

		if (isBulletHuman)
		{
			for (int b = 0; b < unitSpawnIndex; b++)
			{
				if (u_AllDynam[b] == u_Human) { continue; }
				else { checkCollision(blt_Current, b); }
			}
			for (int b = 0; b < buildSpawnIndex; b++)
			{
				if (b_AllDynam[b] == b_HumanBarracks || b_AllDynam[b] == b_HumanTC || b_AllDynam[b] == b_HumanTower) { continue; }
				else { checkCollision(blt_Current, b, true); }
			}
		}
		else
		{
			for (int b = 0; b < unitSpawnIndex; b++)
			{
				if (u_AllDynam[b] == u_Invader) { continue; }
				else { checkCollision(blt_Current, b); }
			}
			for (int b = 0; b < buildSpawnIndex; b++)
			{
				if (b_AllDynam[b] == b_InvaderBarracks || b_AllDynam[b] == b_InvaderTC || b_AllDynam[b] == b_InvaderTower) { continue; }
				else { checkCollision(blt_Current, b, true); }
			}
		}
	}
}