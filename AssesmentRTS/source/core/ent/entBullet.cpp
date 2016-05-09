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

void spawnBullet(Unit &u, float tx, float ty)
{
	PosDim unitPosDim = u.getPosDim();
	Bullet blt;
	bool isIndexOpen = false;
	int openIndex;

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

	blt_AllDynam[openIndex] = Bullet(u.getUnitType(), u.getOwner(), unitPosDim, tx, ty, 75.0f, u.getDMG());
}

void spawnBullet(Building b, float tx, float ty)
{
	PosDim unitPosDim = b.getPosDim();
	bool isIndexOpen = false;
	int openIndex;

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

	blt_AllDynam[openIndex] = Bullet(0, b.getOwner(), unitPosDim, tx, ty, 75.0f, b.getDMG());
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

void checkCollision(Bullet &u, int index, int entType)
{
	PosDim ua[2] = { u.getPosDim() };

	switch (entType)
	{
	case 0: ua[1] = u_AllDynam[index].getPosDim(); break;
	case 1: ua[1] = b_AllDynam[index].getPosDim(); break;
	case 2: ua[1] = r_AllDynam[index].getPosDim(); break;
	}

	if (ua[0].x - (ua[0].w / 2) <= ua[1].x + (ua[1].w / 2) && ua[0].x + (ua[0].w / 2) >= ua[1].x - (ua[1].w / 2))
	{
		if (ua[0].y + (ua[0].h / 2) >= ua[1].y - (ua[1].h / 2) && ua[0].y - (ua[0].h / 2) <= ua[1].y + (ua[1].h / 2))
		{
			switch (entType)
			{
			case 0: u_AllDynam[index].setHP(u_AllDynam[index].getHP() - u.getDMG()); break;
			case 1: b_AllDynam[index].setHP(b_AllDynam[index].getHP() - u.getDMG()); break;
			case 2: r_AllDynam[index].setHP(r_AllDynam[index].getHP() - u.getDMG()); 
				switch (r_AllDynam[index].getID())
				{
				case 0: p_AllBase[u.getOwner()].addSteel(u.getDMG()); break;
				case 1: p_AllBase[u.getOwner()].addFodd (u.getDMG()); break;
				}
				break;
			}
			
			u = blt_Empty;
		}
	}
}
void bulletCollision()
{
	for (int a = 0; a < bulletSpawnIndex; a++)
	{
		PosDim pd = blt_Current.getPosDim();

		if (blt_Current.getID() == 1)
		{
			for (int b = 0; b < resourceSpawnIndex; b++)
			{
				if (r_AllDynam[b] == r_Empty) { continue; }
				checkCollision(blt_Current, b, 2);
			}
			continue;
		}

		if (blt_Current == blt_Empty) { continue; }
		for (int b = 0; b < unitSpawnIndex; b++)
		{
			if (u_AllDynam[b] == u_Empty || u_AllDynam[b].getOwner() == blt_Current.getOwner()) { continue; }
			else { checkCollision(blt_Current, b, 0); }
		}

		if (blt_Current == blt_Empty) { continue; }
		for (int b = 0; b < buildSpawnIndex; b++)
		{
			if (b_AllDynam[b] == b_Empty || b_AllDynam[b].getOwner() == blt_Current.getOwner()) { continue; }
			else { checkCollision(blt_Current, b, 1); }
		}
	}
}