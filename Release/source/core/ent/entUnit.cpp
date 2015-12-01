#include "./ent.h"
#include "../math.h"
#include "../Game/game.h"

Target	Unit::getTarget()				{ return target; }
int		Unit::getID()					{ return unitID; }
int		Unit::getTargetedUnit()			{ return u_Target; }
int		Unit::getTargetedBuild()		{ return b_Target; }
float	Unit::getSpeed()				{ return speed; }
float	Unit::getTargetX()				{ return targetX; }
float	Unit::getTargetY()				{ return targetY; }
float	Unit::getSpeedX()				{ return speedX; }
float	Unit::getSpeedY()				{ return speedY; }
float	Unit::getAtkRad()				{ return atkRad; }
float	Unit::getSavedX()				{ return saveTX; }
float	Unit::getSavedY()				{ return saveTY; }
float	Unit::getAttackSpeed()			{ return atkSpeed; }
float	Unit::getLastAtk()				{ return lastAtk; }
bool	Unit::isUnitMoving()			{ return moving; }
bool	Unit::doesUnitHaveSlope()		{ return hasSlope; }
bool	Unit::doesUnitHaveSavedCoords() { return hasSavedCoords; }
bool	Unit::hasUnitCollided()			{ return collided; }
bool	Unit::doesUnitHaveTarget()		{ return hasTarget; }
bool	Unit::isTargetUnit()			{ return isUnitTarget; }
bool	Unit::isTargetInRange()			{ return targetInRange; }
void	Unit::setSpeed(float s)	{ speed = s; }
void	Unit::setID(int ID)	{ unitID = ID; }
void	Unit::setTargetCoords(float x, float y) { targetX = x; targetY = y; moving = true; hasSlope = false; }
void	Unit::stopMoving() { moving = false; hasSlope = false; }
void	Unit::setAxisSpeeds(float sx, float sy) { speedX = sx; speedY = sy; hasSlope = true; }
void	Unit::setAtkRad(float ar) { atkRad = ar; }
void	Unit::setSavedCoords() { saveTX = targetX, saveTY = targetY, hasSavedCoords = true; }
void	Unit::setUnitCollision(bool hasCollided) { collided = hasCollided; if (!hasCollided) { hasSavedCoords = false; } }
void	Unit::setTarget(Target t) { target = t; }
void	Unit::setTargetedUnit(int index) { u_Target = index; isUnitTarget = true; hasTarget = true;}
void	Unit::setTargetedBuild(int index) { b_Target = index; isUnitTarget = false; hasTarget = true; }
void	Unit::setTargetInRange(bool tir) { targetInRange = tir; }
void	Unit::setAttackSpeed(float as) { atkSpeed = as; }
void	Unit::setLastAttack(float la) { lastAtk = la; }
void	Unit::setTargetStatus(bool ht) { hasTarget = ht; }

int unitSpawnIndex = 1;
Unit u_AllBase[unitIndex];
Unit *u_AllDynam = NULL;

float getUnitW(Unit &u)
{
	int idMatch;

	for (int a = 0; a < unitIndex; a++)
	{
		if (u == u_AllBase[a]) 
		{ 
			idMatch = u_AllBase[a].getID(); 
			a = unitIndex; 
		}
		else { idMatch = -1; }
	}

	switch (idMatch)
	{
	case 0: return wi_Human;
	case 1: return wi_Invader;
	default: cout << "\nDefaulted with: " << idMatch; return 9;
	}
}
float getUnitH(Unit &u)
{
	int idMatch;
	for (int a = 0; a < unitIndex; a++)
	{
		if (u == u_AllBase[a]) { idMatch = u_AllBase[a].getID(); a = unitIndex; }
		else { idMatch = -1; }
	}

	switch (idMatch)
	{
	case 0: return hi_Human;
	case 1: return hi_Invader;
	default: cout << "\nDefaulted with: " << idMatch; return 9;
	}
}

void spawnUnit(Unit u, float x, float y, float tx, float ty)
{
	bool isIndexOpen = false;
	int openIndex;

	for (int a = 0; a < unitSpawnIndex; a++)
	{
		if (u_Current == u_Empty)
		{
			isIndexOpen = true;
			openIndex = a;
			a = unitSpawnIndex;
		}
	}

	if (!isIndexOpen)
	{
		Unit *temp = new Unit[unitSpawnIndex];
		for (int a = 0; a < unitSpawnIndex; a++) { temp[a] = u_Current; }
		openIndex = unitSpawnIndex;
		unitSpawnIndex++;
		delete[] u_AllDynam;
		u_AllDynam = new Unit[unitSpawnIndex];
		for (int a = 0; a < openIndex; a++) { u_Current = temp[a]; }
		delete[]temp;
	}

	if (tx != 0 && ty != 0) { u.setTargetCoords(tx, ty); }
	u.setPosDim({ x, y, getUnitH(u), getUnitW(u) });
	u_AllDynam[openIndex] = u;
}
void killUnits()
{
	for (int a = 0; a < unitSpawnIndex; a++)
	{
		if (u_Current.getSelected())
		{
			u_Current.setHP(0);
		}
	}
}

void shootTarget(Unit &u)
{
	if (u.getLastAtk() >= u.getAttackSpeed())
	{
		PosDim blerg;
		PosDim temp = u.getPosDim();
		bool inited = false;
		float r = u.getAtkRad();
		if (u.isTargetUnit())
		{
			if (u_AllDynam[u.getTargetedUnit()] == u_Empty) { u.setTargetStatus(false); u.setTargetInRange(false); }
			else { blerg = u_AllDynam[u.getTargetedUnit()].getPosDim(); inited = true; }
		}
		else
		{
			if (b_AllDynam[u.getTargetedBuild()] == b_Empty) { u.setTargetStatus(false); u.setTargetInRange(false); }
			else { blerg = b_AllDynam[u.getTargetedBuild()].getPosDim(); inited = true; }
		}

		if (inited)
		{
			float dist = sqrt(expo<float>(abs(temp.x - blerg.x), 2) + expo<float>(abs(temp.y - blerg.y), 2));
			if (dist < r)
			{
				u.setLastAttack(0);
				spawnBullet(u, blerg.x, blerg.y);
			}
			else if (blerg.x != u.getTargetX() && blerg.y != u.getTargetY());
			{
				u.setTargetInRange(false);
				u.setTargetCoords(blerg.x, blerg.y);
			}
		}
	}
	else
	{
		u.setLastAttack(u.getLastAtk() + getDeltaTime());
		if (u.getLastAtk() < 0.0f) { u.setLastAttack(0.0f); }
	}
}
void findTarget(Unit &u)
{
	PosDim temp = u.getPosDim();
	PosDim blerg;
	PosDim closest = temp;
	float dist = xSpace(1000);

	switch (u.getTarget())
	{
	case UNITS:
		if (u == u_Human)
		{
			for (int a = 0; a < unitSpawnIndex; a++)
			{
				if (u_Current == u_Human || u_Current == u_Empty) { continue; }
				else
				{
					float comp;
					blerg = u_Current.getPosDim();
					comp = sqrt(expo<float>(abs(blerg.x - temp.x), 2) + expo<float>(abs(blerg.y - temp.y), 2));
					if (comp < dist) { dist = comp; u.setTargetedUnit(a); closest = blerg; }
				}
			}
		}
		else
		{
			for (int a = 0; a < unitSpawnIndex; a++)
			{
				if (u_Current == u_Invader || u_Current == u_Empty) { continue; }
				else
				{
					float comp;
					blerg = u_Current.getPosDim();
					comp = sqrt(expo<float>(abs(blerg.x - temp.x), 2) + expo<float>(abs(blerg.y - temp.y), 2));
					if (comp < dist) { dist = comp; u.setTargetedUnit(a); closest = blerg; }
				}
			}
		}
		u.setTargetCoords(closest.x, closest.y);
		break;
	case BUILDINGS:
		if (u == u_Human)
		{
			for (int a = 0; a < buildSpawnIndex; a++)
			{
				if (b_Current == b_HumanBarracks || b_Current == b_HumanTC || b_Current == b_HumanTower || b_Current == b_Empty) { continue; }
				else
				{
					float comp;
					blerg = b_Current.getPosDim();
					comp = sqrt(expo<float>(abs(blerg.x - temp.x), 2) + expo<float>(abs(blerg.y - temp.y), 2));
					if (comp < dist) { dist = comp; u.setTargetedBuild(a); closest = blerg; }
				}
			}
		}
		else
		{
			for (int a = 0; a < buildSpawnIndex; a++)
			{
				if (b_Current == b_InvaderBarracks || b_Current == b_InvaderTC || b_Current == b_InvaderTower || b_Current == b_Empty) { continue; }
				else
				{
					float comp;
					blerg = b_Current.getPosDim();
					comp = sqrt(expo<float>(abs(blerg.x - temp.x), 2) + expo<float>(abs(blerg.y - temp.y), 2));
					if (comp < dist) { dist = comp; u.setTargetedBuild(a); closest = blerg; }
				}
			}
		}
		u.setTargetCoords(closest.x, closest.y);
		break;
	default:
		break;
	}
}
void moveUnit(Unit &u)
{
	PosDim temp = u.getPosDim();
	float n[] = { u.getTargetX(), u.getTargetY(), temp.x - n[0], temp.y - n[1] };
	
	if (u.getTarget() != NOTHING)
	{
		if (u.doesUnitHaveTarget())
		{
			float r = u.getAtkRad();
			bool inited = false;
			PosDim blerg;

			if (u.isTargetUnit())
			{
				if (u_AllDynam[u.getTargetedUnit()] == u_Empty) { u.setTargetStatus(false); u.setTargetInRange(false); }
				else { blerg = u_AllDynam[u.getTargetedUnit()].getPosDim(); inited = true; }
			}
			else
			{
				if (b_AllDynam[u.getTargetedBuild()] == b_Empty) { u.setTargetStatus(false); u.setTargetInRange(false); }
				else { blerg = b_AllDynam[u.getTargetedBuild()].getPosDim(); inited = true; }
			}

			if (inited)
			{
				float dist = sqrt(expo<float>(abs(temp.x - blerg.x), 2) + expo<float>(abs(temp.y - blerg.y), 2));
				if (dist < r)
				{
					u.stopMoving();
					u.setTargetInRange(true);
				}
				else if (blerg.x != u.getTargetX() && blerg.y != u.getTargetY());
				{
					u.setTargetCoords(blerg.x, blerg.y);
				}
			}
		}
	}
	
	if (n[2] <= 1 && n[2] >= -1 && n[3] <= 1 && n[3] >= -1)
	{
		u.stopMoving();
	}
	else if (u.doesUnitHaveSlope() && !u.isTargetInRange())
	{
		if (n[0] < temp.x) { temp.x -= u.getSpeedX() * getDeltaTime(); }
		else { temp.x += u.getSpeedX() * getDeltaTime(); }
		if (n[1] < temp.y) { temp.y -= u.getSpeedY() * getDeltaTime(); }
		else { temp.y += u.getSpeedY() * getDeltaTime(); }
	}
	else
	{
		float time = (sqrt(expo<float>(n[2], 2) + expo<float>(n[3], 2))) / (u.getSpeed());
		u.setAxisSpeeds(abs(n[2]) / time, abs(n[3]) / time);

		if (n[0] < temp.x) { temp.x -= u.getSpeedX() * getDeltaTime(); }
		else if (n[0] > temp.x) { temp.x += u.getSpeedX() * getDeltaTime(); }
		if (n[1] < temp.y) { temp.y -= u.getSpeedY() * getDeltaTime(); }
		else if (n[1] > temp.y) { temp.y += u.getSpeedY() * getDeltaTime(); }
	}

	u.setPosDim(temp);
}
void moveSelectedUnits()
{
	float coords[2] = { getMouseX(), getMouseY() };

	for (int a = 0; a < unitSpawnIndex; a++)
	{
		if (u_Current.getSelected() && u_Current.getTarget() == NOTHING)
		{
			PosDim unit = u_Current.getPosDim();
			if (coords[0] > xSpace(100) - (unit.w / 2)) { coords[0] = xSpace(100) - (unit.h / 2); }
			else if (coords[0] < xSpace(0) + (unit.w / 2)) { coords[0] = xSpace(0) + (unit.w / 2); }
			if (coords[1] > ySpace(100) - (unit.h / 2)) { coords[1] = ySpace(100) - (unit.h / 2); }
			else if (coords[1] < ySpace(20) + (unit.h / 2)) { coords[1] = ySpace(20) + (unit.h / 2); }
			u_Current.setTargetCoords(coords[0], coords[1]);
			u_Current.setUnitCollision(false);
		}
	}
}
