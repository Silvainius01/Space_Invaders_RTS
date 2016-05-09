#pragma once
#include "../ui/ui.h"
#include "../Game/game.h"
#include <iostream>
using namespace std;

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

enum Target
{
	NOTHING = -1,
	UNITS = 0,
	FOOD = 1,
	STEEL = 2,
	BUILDINGS = 4
};

class Entity
{
protected:
	PosDim pd;
	unsigned sprite;
	int hp, dmg, spriteIndex = 0, colorSprite;
	int cost = 5;
	int ownerIndex;
	bool selected = false;
	char *name;
public:
	PosDim getPosDim();
	int getOwner();
	unsigned getSprite();
	int getHP();
	int getDMG();
	int getSpriteIndex();
	int getColoredIndex();
	int getCost();
	bool getSelected();
	char *getName();

	void setPosDim(PosDim PD);
	void setSprite(unsigned s);
	void setHP(int HP);
	void setDMG(int DMG);
	void setSelected(bool cr);
	void setSpriteIndex(int si);
	void setColoredIndex(int ci);
	void setName(char *n);
	void setCost(int c);
	void setOwner(int playerIndex);
};
extern int rotation;
#define getX getPosDim().x
#define getY getPosDim().y
#define getH getPosDim().h
#define getW getPosDim().w
#define getPos { getX, getY, -1, -1 }
#define getDim { -1, -1, getH, getW }

class Unit : public Entity
{
private:
	Target target = NOTHING;
	int u_Target = -1;
	int b_Target = -1;
	int r_Target = -1;
	int unitID, unitType;
	float speed, speedX, speedY;
	float atkSpeed, atkRad, lastAtk = 0.0f;
	float saveTX, targetX, targetY, saveTY;
	bool moving = false, hasSlope = false, hasSavedCoords = false, collided = false;
	bool hasTarget = false, isUnitTarget = true, isFoodTarget = false, targetInRange = false;
public:
	//Always false for humans. It is a flag used by the AI for grouping.
	bool groupFlag = false;

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
	float getDistToTar();
	int getID();
	int getTargetedUnit();
	int getTargetedBuild();
	int getTargetedResource();
	int getUnitType();
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
	void setUnitType(int t);
	void setTargetCoords(float x, float y);
	void stopMoving();
	void setAxisSpeeds(float sx, float sy);
	void setAtkRad(float ar);
	void setSavedCoords();
	void setUnitCollision(bool hasCollided);
	void setTargetedUnit(int index);
	void setTargetedBuild(int index);
	void setTargetedResource(int index);
	void setTargetInRange(bool tir);
	void setAttackSpeed(float as);
	void setLastAttack(float la);
	void setTargetStatus(bool ht);
	void print();

	//Default Constructor
	Unit(void)
	{
		pd = { 0 ,0, 9, 9 };
		sprite = ui_Text;
		spriteIndex = 0;
		colorSprite = 0;
		unitID = -1;
		hp = 0;
		dmg = 0;
		speed = 0;
		speedX = 0;
		speedY = 0;
		targetX = 0;
		targetY = 0;
		moving = 0;
		hasSlope = false;
		selected = false;
		name = "Default";
		atkRad = 0;
		saveTX = 0;
		saveTY = 0;
		hasSavedCoords = false;
		target = NOTHING;
		targetInRange = false;
		hasTarget = false;
		atkSpeed = 0;
		groupFlag = false;
		ownerIndex = -1;
		lastAtk = 0;
		unitType = -1;
	}
	//Verbose constructor
	Unit(int ID, int unitType, char *name, unsigned sprite, int spriteIndex, int colorIndex, int HP, int DMG, float spd, float range, float attackSpeed,int ownerIndex)
	{
		setHP(HP); setDMG(DMG); setSpeed(spd);
		setSprite(sprite); setID(ID); setUnitType(unitType);
		setSpriteIndex(spriteIndex); setColoredIndex(colorIndex);
		setName(name); setAtkRad(range);
		setAttackSpeed(attackSpeed);
		setOwner(ownerIndex);
		stopMoving();
		setLastAttack(0);
		target = NOTHING;
		targetInRange = false;
		hasTarget = false;
	}

	Unit &operator=(Unit &u)
	{
		pd				= u.getPosDim();
		sprite			= u.getSprite(); 
		spriteIndex		= u.getSpriteIndex();
		colorSprite		= u.getColoredIndex();
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
		groupFlag       = u.groupFlag;
		ownerIndex		= u.getOwner();
		lastAtk			= u.getLastAtk();
		unitType		= u.getUnitType();
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
const int unitIndex = 6;
extern int unitSpawnIndex;
extern Unit u_AllBase[unitIndex];
extern Unit *u_AllDynam;
#define u_Empty		u_AllBase[0]
#define u_Human		u_AllBase[1]
#define u_Invader	u_AllBase[2]
#define u_Hotdog	u_AllBase[3]
#define u_HumanCollector u_AllBase[4]
#define u_InvaderCollector u_AllBase[5]
#define u_Current	u_AllDynam[a]
#define u_Targeted	u_AllDynam[u.getTargetedUnit()]
#define u_EntOnOverlay u_AllDynam[entOnOverlay]
#define b_EntOnOverlay b_AllDynam[entOnOverlay]
#define r_EntOnOverlay r_AllDynam[entOnOverlay]

class Building : public Entity
{
private:
	int maxQueue, buildingID;
	float atkRad, saveTrainTime = 0, baseTrainTime = 3.0f, currentTrainTime = 3.0f;
	float atkSpeed, lstAtk = 0.0f;
public:
	//Note: This return the index used to initialize, not the last index
	int queue[10];
	int gridPos[2];
	int getMaxQueue();
	int getID();
	float getAtkRad();
	float getElapsedTrainTime();
	float getAtkSpeed();
	float getLastAtk();
	float getBaseTrainTime();
	float getTrainTime();

	void setMaxQueue(int mq);
	void setAtkRad(float ar);
	void setID(int ID);
	void setElapsedTrainTime(float ett);
	void setAtkSpeed(float as);
	void setLastAtk(float la);
	void setTrainTime(float tt);

	Building(void)
	{
		setSprite(ui_Text); setID(-1);
		setHP(200); setDMG(0);
		setMaxQueue(1); setAtkRad(0);
		setSelected(false);
		setName("DEFAULT");
	}
	Building(int ID, char *name, unsigned sprite, int spriteIndex, int coloredIndex, int HP, int DMG, float atkRad, float atkSpd, int cost, int ownerIndex)
	{
		setSprite(sprite); 
		setID(ID); setHP(HP); setDMG(DMG);
		setAtkRad(atkRad);
		setSpriteIndex(spriteIndex); setColoredIndex(coloredIndex);
		setName(name);
		setAtkSpeed(atkSpd);
		setCost(cost);
		setOwner(ownerIndex);
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
		colorSprite = b.getColoredIndex();
		name		= b.getName();
		atkSpeed	= b.getAtkSpeed();
		cost		= b.getCost();
		gridPos[0]	= b.gridPos[0];
		gridPos[1]	= b.gridPos[1];
		ownerIndex	= b.getOwner();
		lstAtk		= b.getLastAtk();
		currentTrainTime = b.getElapsedTrainTime();
		for (int a = 0; a < 10; a++) { queue[a] = b.queue[a]; }
		return *this;
	}
	bool operator==(Building &b)
	{
		if (b.getID() == this->getID()) { return true; }
		else { return false; }
	}
	operator int() const { return buildingID; }
};
const int buildingIndex = 9;
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
#define b_HumanFarm			b_AllBase[7]
#define b_InvaderFarm		b_AllBase[8]
#define b_Current			b_AllDynam[a]
#define b_Targeted			b_AllDynam[u.getTargetedBuild()]

class Bullet : public Entity
{
private:
	float speed, speedX, speedY;
	float targetX, targetY;
	int bulletID;
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
	Bullet(int ID, int owner, PosDim location, float targetX, float targetY, float speed, float dmg)
	{
		setPosDim({ location.x, location.y, 1, 1 });
		setTargetCoords(targetX, targetY);
		setOwner(owner);
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
		ownerIndex = blt.getOwner();
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
const int bulletIndex = 1;
extern Bullet blt_AllBase[bulletIndex];
extern Bullet *blt_AllDynam;
#define blt_Empty blt_AllBase[0]
#define blt_Current blt_AllDynam[a]

class Resource : public Entity
{
private:
	int resourceID;
	float resistance;
public:
	int getID();
	float getResistance();

	void setID(int id);
	void setResistance(float r);

	Resource(void)
	{
		sprite = ui_Text;
		hp = 0;
		dmg = 0;
		spriteIndex = 0;
		colorSprite = 0;
		selected = false;
		name = "Defualt";
		setID(-1);
		resistance = 0.0f;
	}
	Resource(int ID, char *name, unsigned sprite, int spiteIndex, int coloredIndex, int resourceCount, float collectResistance)
	{
		setID(ID); setName(name);
		setSprite(sprite); setSpriteIndex(spiteIndex); setColoredIndex(coloredIndex);
		setHP(resourceCount); setResistance(collectResistance);
		selected = false;
	}

	Resource &operator=(Resource &r)
	{
		pd				= r.getPosDim();
		hp				= r.getHP();
		resourceID		= r.getID();
		resistance		= r.getResistance();
		sprite			= r.getSprite();
		spriteIndex		= r.getSpriteIndex();
		colorSprite		= r.getColoredIndex();
		name			= r.getName();
		selected		= r.getSelected();
		return *this;
	}

	bool operator==(Resource &r)
	{
		if (resourceID == r.getID()) { return true; }
		return false;
	}

	operator int() const { return resourceID; }
};
const int resourceIndex = 3;
extern int resourceSpawnIndex;
extern Resource r_AllBase[resourceIndex];
extern Resource *r_AllDynam;
#define r_Empty r_AllBase[0]
#define r_Steel r_AllBase[1]
#define r_Food r_AllBase[2]
#define r_Current r_AllDynam[a]
#define r_Targeted r_AllDynam[u.getTargetedResource()]


extern float dist(PosDim start, PosDim end);

//In ent.cpp

extern void initEnts();
extern void selectEnts(float selbox[4]);

//In entUnit.cpp

extern void spawnUnit(Unit u, int owner, float x = getMouseX(), float y = getMouseY(), float tx = 0, float ty = 0);
extern void updateEnts();
extern void moveSelectedUnits();
extern void moveUnit(Unit &u);
extern void findTarget(Unit &u);
extern void shootTarget(Unit &u);
extern Unit findUnitBase(int ID);

//In entBuild.cpp

extern void spawnBuild(Building b, float x = getMouseX(), float y = getMouseY());
extern void addToQueue(Unit u, Building &b);
extern void updateQueue(Building &b);
extern void checkForTargets(Building &b);

//In entBullet.cpp

extern void spawnBullet(Unit &u, float tx, float ty);
extern void spawnBullet(Building b, float tx, float ty);
extern void moveBullet(Bullet &blt);
extern void bulletCollision();

//In entResource.cpp

extern void spawnResource(Resource r, float x = getMouseX(), float y = getMouseY());

/* Debug Functions */

extern void killUnits(); //In entUnit.cpp
extern void killBuild(); //In entBuild.cpp
extern void killResource(); //In entResource.cpp