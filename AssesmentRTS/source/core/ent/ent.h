#pragma once
#include "../ui/ui.h"
#include <iostream>
using namespace std;

enum Target
{
	NOTHING = -1,
	UNITS = 0,
	BUILDINGS = 4,
};

struct PosDim 
{ 
	float x, y, h, w; 

	bool operator==(PosDim &pd)
	{
		if (pd.x == this->x)
		{
			if (pd.y == this->y)
			{
				if (pd.w == this->w)
				{
					if (pd.h == this->h)
					{
						return true;
					}
				}
			}
		}

		return false;
	}
};

class Entity
{
protected:
	PosDim pd;
	unsigned sprite;
	int hp, dmg, spriteIndex = 0;
	bool selected = false;
	char *name;
public:
	PosDim getPosDim();
	unsigned getSprite();
	int getHP();
	int getDMG();
	int getSpriteIndex();
	bool getSelected();
	char *getName();

	void setPosDim(PosDim PD);
	void setSprite(unsigned s);
	void setHP(int HP);
	void setDMG(int DMG);
	void setSelected(bool cr);
	void setSpriteIndex(int si);
	void setName(char *n);
};

class Unit : public Entity
{
private:
	Target target = NOTHING;
	int u_Target = -1;
	int b_Target = -1;
	float speed, speedX, speedY;
	float atkSpeed, atkRad, lastAtk = 0.0f;
	float saveTX, targetX, targetY, saveTY;
	bool moving = false, hasSlope = false, hasSavedCoords = false, collided = false;
	bool hasTarget = false, isUnitTarget = true, targetInRange = false;
	int unitID;
public:
	Target getTarget();
	float getSpeed();
	float getTargetX();
	float getTargetY();
	float getSpeedX();
	float getSpeedY();
	float getAtkRad();
	float getSavedX();
	float getSavedY();
	float getAttackSpeed();
	float getLastAtk();
	int getID();
	int getTargetedUnit();
	int getTargetedBuild();
	bool isUnitMoving();
	bool doesUnitHaveSlope();
	bool doesUnitHaveSavedCoords();
	bool hasUnitCollided();
	bool doesUnitHaveTarget();
	bool isTargetUnit();
	bool isTargetInRange();

	void setTarget(Target t);
	void setSpeed(float s);
	void setID(int ID);
	void setTargetCoords(float x, float y);
	void stopMoving();
	void setAxisSpeeds(float sx, float sy);
	void setAtkRad(float ar);
	void setSavedCoords();
	void setUnitCollision(bool hasCollided);
	void setTargetedUnit(int index);
	void setTargetedBuild(int index);
	void setTargetInRange(bool tir);
	void setAttackSpeed(float as);
	void setLastAttack(float la);
	void setTargetStatus(bool ht);
	//Default Constructor
	Unit(void)
	{
		setHP(10); setDMG(1); setSpeed(10);
		setSprite(ui_Text); setID(-1);
		setName("DEFAULT");
	}
	//Verbose constructor
	Unit(int ID, char *name, unsigned sprt, int HP, int DMG, float spd, float ar, float as)
	{
		setHP(HP); setDMG(DMG); setSpeed(spd);
		setSprite(sprt); setID(ID);
		setName(name); setAtkRad(ar);
		setAttackSpeed(as);
	}

	Unit &operator=(Unit &u)
	{
		pd				= u.getPosDim();
		sprite			= u.getSprite(); 
		unitID			= u.getID();
		hp				= u.getHP(); 
		dmg				= u.getDMG(); 
		speed			= u.getSpeed();
		speedX			= u.getSpeedX();
		speedY			= u.getSpeedY();
		targetX			= u.getTargetX(); 
		targetY			= u.getTargetY();
		moving			= u.isUnitMoving(); 
		hasSlope		= u.doesUnitHaveSlope();
		selected		= u.getSelected();
		name			= u.getName();
		atkRad			= u.getAtkRad();
		saveTX			= u.getSavedX();
		saveTY			= u.getSavedY();
		hasSavedCoords	= u.doesUnitHaveSavedCoords();
		target			= u.getTarget();
		atkSpeed		= u.getAttackSpeed();
		return *this;
	}
	bool operator==(Unit &u)
	{
		if (u.getID() == this->getID()) { return true; }
		else { return false; }
	}
	bool operator!=(Unit &u)
	{
		if (u.getID() != this->getID()) { return true; }
		else { return false; }
	}
};
const int unitIndex = 3;
extern int unitSpawnIndex;
extern Unit u_AllBase[unitIndex];
extern Unit *u_AllDynam;
#define u_Empty		u_AllBase[0]
#define u_Human		u_AllBase[1]
#define u_Invader	u_AllBase[2]
#define u_Current	u_AllDynam[a]

class Building : public Entity
{
private:
	int maxQueue, buildingID;
	float atkRad, saveTrainTime = 0;
	float atkSpeed, lstAtk = 0.0f;
public:
	int queue[10];
	//Note: This return the index used to initialize, not the last index
	int getMaxQueue();
	int getID();
	float getAtkRad();
	float getElapsedTrainTime();
	float getAtkSpeed();
	float getLastAtk();

	void setMaxQueue(int mq);
	void setAtkRad(float ar);
	void setID(int ID);
	void setElapsedTrainTime(float ett);
	void setAtkSpeed(float as);
	void setLastAtk(float la);

	Building(void)
	{
		setSprite(ui_Text); setID(-1);
		setHP(200); setDMG(0);
		setMaxQueue(1); setAtkRad(0);
		setSelected(false);
		setName("DEFAULT");
	}
	Building(int ID, char *name, unsigned s, int HP, int DMG, int mq, float ar, float as, int si)
	{
		setSprite(s); setID(ID);
		setHP(HP); setDMG(DMG);
		setMaxQueue(mq); setAtkRad(ar);
		setSpriteIndex(si);
		setName(name);
		setAtkSpeed(as);
	}

	Building &operator=(Building &b)
	{
		pd			= b.getPosDim();
		sprite		= b.getSprite();
		hp			= b.getHP();
		dmg			= b.getDMG();
		selected	= b.getSelected();
		buildingID	= b.getID();
		maxQueue	= b.getMaxQueue();
		atkRad		= b.getAtkRad();
		spriteIndex = b.getSpriteIndex();
		name		= b.getName();
		maxQueue	= b.getMaxQueue();
		atkSpeed	= b.getAtkSpeed();
		return *this;
	}
	bool operator==(Building &b)
	{
		if (b.getID() == this->getID()) { return true; }
		else { return false; }
	}
};
const int buildingIndex = 7;
extern int buildSpawnIndex;
extern Building b_AllBase[buildingIndex];
extern Building *b_AllDynam;
#define b_Empty				b_AllBase[0]
#define b_HumanTC			b_AllBase[1]
#define b_HumanTower		b_AllBase[2]
#define b_HumanBarracks		b_AllBase[3]
#define b_InvaderTC			b_AllBase[4]
#define b_InvaderTower		b_AllBase[5]
#define b_InvaderBarracks	b_AllBase[6]
#define b_Current			b_AllDynam[a]

class Bullet : public Entity
{
private:
	float speed, speedX, speedY;
	float targetX, targetY;
	int bulletID, dmg;
	bool hasSlope = false;
public:
	float getSpeed();
	float getSpeedX();
	float getSpeedY();
	float getTargetX();
	float getTargetY();
	bool doesBulletHaveSlope();
	int getID();

	void setID(int ID);
	void setSpeed(float s);
	void setAxisSpeeds(float sx, float sy);
	void setTargetCoords(float tx, float ty);
	void setSlope(bool hs);

	//Default Constructor
	Bullet(void)
	{
		setSpeed(5); 
		setDMG(1);
		setID(-1);
	}
	//Verbose Constructor
	Bullet(int ID, float speed, float dmg)
	{
		setSpeed(speed);
		setDMG(dmg);
		setID(ID);
	}

	Bullet &operator=(Bullet blt)
	{
		pd = blt.getPosDim();
		bulletID = blt.getID();
		dmg = blt.getDMG();
		speed = blt.getSpeed();
		speedX = blt.getSpeedX();
		speedY = blt.getSpeed();
		targetX = blt.getTargetX();
		targetY = blt.getTargetY();
		hasSlope = false;
		return *this;
	}
	bool operator==(Bullet &blt)
	{
		if (blt.getID() == this->getID()) { return true; }
		else { return false; }
	}
};
extern int bulletSpawnIndex;
const int bulletIndex = 5;
extern Bullet blt_AllBase[bulletIndex];
extern Bullet *blt_AllDynam;
#define blt_Empty blt_AllBase[0]
#define blt_Human blt_AllBase[1]
#define blt_Invader blt_AllBase[2]
#define blt_HumanTower blt_AllBase[3]
#define blt_InvaderTower blt_AllBase[4]
#define blt_Current blt_AllDynam[a]

//In ent.cpp

extern void initEnts();
extern void selectEnts(float selbox[4]);

//In entUnit.cpp

extern void spawnUnit(Unit u, float x = getMouseX(), float y = getMouseY());
extern void updateEnts();
extern bool selectUnits( float selbox[4]);
extern void moveSelectedUnits();
extern void moveUnit(Unit &u);
extern void findTarget(Unit &u);
extern void shootTarget(Unit &u);

//In entBuild.cpp

extern void spawnBuild(Building b, float x = getMouseX(), float y = getMouseY());
extern void selectBuilds(float selbox[4]);
extern void killBuild();
extern void addToQueue(Unit u, Building &b);
extern void updateQueue(Building &b);
extern void checkForTargets(Building &b);

//In entBullet.cpp

extern void spawnBullet(Unit u, float tx, float ty);
extern void spawnBullet(Building b, float tx, float ty);
extern void moveBullet(Bullet &blt);
extern void bulletCollision();

/* Debug Functions */

extern void killUnits(); //In entUnit.cpp