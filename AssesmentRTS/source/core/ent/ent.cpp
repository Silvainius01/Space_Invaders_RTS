#include "ent.h"
#include "math.h"
#include "../ui/input.h"
#include "../ui/ui.h"
using namespace std;
using namespace sfw;

PosDim	 Entity::getPosDim()			{ return pd; }
unsigned Entity::getSprite()			{ return sprite; }
int		 Entity::getHP()				{ return hp; }
int		 Entity::getDMG()				{ return dmg; }
int		 Entity::getSpriteIndex()		{ return spriteIndex; }
bool	 Entity::getSelected()			{ return selected; }
char*	 Entity::getName()				{ return name; }
void	 Entity::setPosDim(PosDim PD)	{ pd = PD; }
void	 Entity::setSprite(unsigned s)	{ sprite = s; }
void	 Entity::setHP(int HP)			{ hp = HP; }
void	 Entity::setDMG(int DMG)		{ dmg = DMG; }
void	 Entity::setSelected(bool cr)	{ selected = cr; }
void	 Entity::setSpriteIndex(int si) { spriteIndex = si; }
void	 Entity::setName(char *n)		{ name = n; }

int dealDamage(Building &b, Unit &u, bool isBuildAtkUnit);

void initEnts()
{
	u_Empty		= Unit();
	u_AllDynam	= new Unit[unitSpawnIndex];
	u_AllDynam[0] = u_Empty;

	u_Human		= Unit(0, "Human\nSoldier", ui_Human, 100, 5, 20.0f, 100.0f, 1.13f);
	u_Invader	= Unit(1, "Invader\nSoldier", ui_Invader, 100, 2, 20.0f, 100.0f, 0.5f);

	b_Empty		= Building();
	b_AllDynam	= new Building[buildSpawnIndex];
	b_AllDynam[0] = b_Empty;

	b_HumanTC			= Building(0, "Town\nCenter", ui_TownCenter, 500, 0, 1, 0, 0, 1);
	b_HumanTower		= Building(1, "Tower", ui_Tower, 300, 25, 1, 150.0f, 1.5f, 1);
	b_HumanBarracks		= Building(2, "Barracks", ui_Barracks, 200, 0, 10, 0, 0, 0);
	b_InvaderTC =		b_HumanTC;			b_InvaderTC.setID(3);		b_InvaderTC.setSpriteIndex(0);
	b_InvaderTower =	b_HumanTower;		b_InvaderTower.setID(4);	b_InvaderTower.setSpriteIndex(0);
	b_InvaderBarracks = b_HumanBarracks;	b_InvaderBarracks.setID(5);	b_InvaderBarracks.setSpriteIndex(0);

	blt_Empty = Bullet();
	blt_AllDynam = new Bullet[bulletSpawnIndex];
	blt_AllDynam[0] = blt_Empty;

	blt_Human = Bullet(0, 50.0f, u_Human.getDMG());
	blt_Invader = Bullet(1, 50.0f, u_Invader.getDMG());
	blt_HumanTower = Bullet(2, 60, b_HumanTower.getDMG());
	blt_InvaderTower = Bullet(3, 60, b_InvaderTower.getDMG());
}

void checkCollision(Unit &u, int index, bool isBuild = false)
{
	PosDim ua[2] = { u.getPosDim() };

	if (isBuild) { ua[1] = b_AllDynam[index].getPosDim(); }
	else { ua[1] = u_AllDynam[index].getPosDim(); }

	// l = x - (w / 2)
	// r = x + (w / 2)
	// u = y + (h / 2)
	// d = y - (h / 2) 

	if (ua[0].x - (ua[0].w / 2) <= ua[1].x + (ua[1].w / 2) && ua[0].x + (ua[0].w / 2) >= ua[1].x - (ua[1].w / 2))
	{
		if (ua[0].y + (ua[0].h / 2) >= ua[1].y - (ua[1].h / 2) && ua[0].y - (ua[0].h / 2) <= ua[1].y + (ua[1].h / 2))
		{
			//The right side of these equations is the passed unit, and the right is the detected collision.
			//left - right, right - left, up - down, down - up
			//Left collision, Right collision, Down collision, Up collision
			float sideSort[4] = { abs((ua[0].x - (ua[0].w / 2)) - (ua[1].x + (ua[1].w / 2))), abs((ua[0].x + (ua[0].w / 2)) - (ua[1].x - (ua[1].w / 2))), abs((ua[0].y - (ua[0].h / 2)) - (ua[1].y + (ua[1].h / 2))), abs((ua[0].y + (ua[0].h / 2)) - (ua[1].y - (ua[1].h / 2))) };
			float sideSave[4]; for (int a = 0; a < 4; a++) { sideSave[a] = sideSort[a]; }
			int side;
			bool flag = false; 

			if (!u.doesUnitHaveSavedCoords())
			{
				if (!u.isUnitMoving()) { u.setTargetCoords(ua[0].x, ua[0].y); }
				u.setSavedCoords();
			}
			u.setUnitCollision(true);

			for (int a = 0, b = 3; a < b; a++)
			{

				if (sideSort[a] > sideSort[a + 1])
				{
					float swap = sideSort[a];
					sideSort[a] = sideSort[a + 1];
					sideSort[a + 1] = swap;
					flag = true;
				}

				if (a == b - 1 && flag)
				{
					a = -1;
					b--;
				
					flag = false;
				}
			}
			for (int a = 0; a < 4; a++)
			{
				if (sideSort[0] == sideSave[a])
				{
					side = a + 1;
				}
			}

			switch (side)
			{
			case 1: //Left will ALWAYS move up.
				u.setTargetCoords(ua[0].x, (ua[0].y + ua[1].h));
				break;
			case 2: //Right will ALWAYS move down.
				u.setTargetCoords(ua[0].x, (ua[0].y - ua[1].h));
				break;
			case 3: //Down will ALWAYS move right
				u.setTargetCoords(ua[0].x + ua[1].w, ua[0].y);
				break;
			case 4: //Up will ALWAYS move left.
				u.setTargetCoords(ua[0].x - ua[1].w, ua[0].y);
				break;
			}

		}
	}
}
void getCollisionCandidates(Unit &u)
{
	//const float step = PI / 12;
	const float r = u.getAtkRad();
	bool checkBuilds = false;
	int spawnIndex = unitSpawnIndex;
	PosDim rupd = u.getPosDim();
	PosDim ucpd;

	if (rupd.x + (rupd.h / 2) > xSpace(79) - (b_InvaderTower.getPosDim().w / 2) && rupd.y - (rupd.h / 2) < ySpace(41) + (b_InvaderTower.getPosDim().h / 2))
	{ 
		checkBuilds = true;
		spawnIndex = buildSpawnIndex;
	}
	else if (rupd.x < xSpace(20) + (b_HumanTower.getPosDim().w / 2) && rupd.y > ySpace(80) + (b_HumanTower.getPosDim().h / 2))
	{ 
		checkBuilds = true; 
		spawnIndex = buildSpawnIndex;
	}

	for (int a = 0; a < spawnIndex; a++)
	{
		if (checkBuilds) { ucpd = b_Current.getPosDim(); }
		else { ucpd = u_Current.getPosDim(); }

		if (u_Current == u_Empty || rupd == ucpd) { continue; }
		else
		{
			if (ucpd.x >= rupd.x - r && ucpd.x <= rupd.x + r)
			{
				if (ucpd.y >= rupd.y - r && ucpd.y <= rupd.y + r)
				{
					if(checkBuilds) { checkCollision(u, a, true); }
					else { checkCollision(u, a); }
				}
			}
		}
	}
}
void updateEnts()
{
	PosDim _pd;
	isUnitOnOverlay = false;
	isBuildOnOverlay = false;

	for (int a = 0; a < unitSpawnIndex; a++)
	{
		if (u_Current != u_Empty)
		{
			if (u_Current.getHP() <= 0) { u_Current = u_Empty; }
			else
			{
				_pd = u_Current.getPosDim();

				if (!u_Current.hasUnitCollided()) { getCollisionCandidates(u_Current); }
				
				if (u_Current.getTarget() != NOTHING && !u_Current.doesUnitHaveTarget()) { findTarget(u_Current); }
				
				if (u_Current.isTargetInRange()) { shootTarget(u_Current); }
				
				if (u_Current.isUnitMoving() && !u_Current.isTargetInRange()) { moveUnit(u_Current); }
				
				if (u_Current.doesUnitHaveSavedCoords() && !u_Current.isUnitMoving())
				{
					u_Current.setTargetCoords(u_Current.getSavedX(), u_Current.getSavedY());
					u_Current.setUnitCollision(false);
				}

				drawTexture(u_Current.getSprite(), _pd.x, _pd.y, _pd.w, _pd.h, 0, true, 0);

				if (u_Current.getSelected())
				{
					drawBox(_pd.x - (_pd.w / 2), _pd.y - (_pd.h / 2), _pd.h, _pd.w, MAGENTA);
					if (!isUnitOnOverlay) { isUnitOnOverlay = true; entOnOverlay = a; }
				}
			}
		}
	}

	for (int a = 0; a < buildSpawnIndex; a++)
	{
		if (b_Current == b_Empty) { continue; 
		}
		_pd = b_Current.getPosDim();

		if (b_Current.getHP() <= 0) { b_Current = b_Empty; }
		else
		{
			updateQueue(b_Current);

			drawTexture(b_Current.getSprite(), _pd.x, _pd.y, _pd.w, _pd.h, 0, true, b_Current.getSpriteIndex());

			if (b_Current == b_InvaderTower || b_Current == b_HumanTower) { checkForTargets(b_Current); }

			if (b_Current.getSelected())
			{
				drawBox(_pd.x - (_pd.w / 2), _pd.y - (_pd.h / 2), _pd.h, _pd.w, MAGENTA);
				if (!isBuildOnOverlay) { isBuildOnOverlay = true; entOnOverlay = a; }
			}
		}
	}

	for (int a = 0; a < bulletSpawnIndex; a++)
	{
		if (blt_Current == blt_Empty) { continue; }

		_pd = blt_Current.getPosDim();

		drawCircle(_pd.x, _pd.y, ySpace(1));

		if (_pd.x != blt_Current.getTargetX() && _pd.y != blt_Current.getTargetY())
		{
			moveBullet(blt_Current);
		}
	}

	bulletCollision();
}

void selectEnts(float selbox[4])
{
	PosDim pd;
	int save;
	bool isShiftPressed = false;
	bool gotUnit = false;

	if (checkBindings(true) == KEY_LSHIFT) { isShiftPressed = true; }

	if (selbox[3] < 0 && selbox[2] < 0)
	{
		selbox[0] += selbox[3];
		selbox[1] += selbox[2];
		selbox[2] = -selbox[2];
		selbox[3] = -selbox[3];
	}
	else if (selbox[3] < 0)
	{
		selbox[0] += selbox[3];
		selbox[3] = -selbox[3];
	}
	else if (selbox[2] < 0)
	{
		selbox[1] += selbox[2];
		selbox[2] = -selbox[2];
	}

	for (int a = 0; a < unitSpawnIndex; a++)
	{
		pd = u_Current.getPosDim();


		if (pd.y + pd.h >= selbox[1] && pd.y <= selbox[1] + selbox[2])
		{
			if (pd.x <= selbox[0] + selbox[3] && pd.x + pd.w >= selbox[0])
			{
				u_Current.setSelected(true);
				gotUnit = true;
			}
			else if (!isShiftPressed) { u_Current.setSelected(false); }
		}
		else if (!isShiftPressed) { u_Current.setSelected(false); }

		if (a == unitSpawnIndex - 1)
		{
			save = a;
			for (a = 0; a < buildSpawnIndex; a++)
			{
				if (gotUnit) { b_Current.setSelected(false); continue; }

				pd = b_Current.getPosDim();

				if (pd.y + pd.h >= selbox[1] && pd.y <= selbox[1] + selbox[2])
				{
					if (pd.x <= selbox[0] + selbox[3] && pd.x + pd.w >= selbox[0])
					{
						b_Current.setSelected(true);
					}
					else if (!isShiftPressed) { b_Current.setSelected(false); }
				}
				else if (!isShiftPressed) { b_Current.setSelected(false); }
			}
			a = save;
		}
	}
}