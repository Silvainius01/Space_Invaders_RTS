#include "ent.h"
#include "../math.h"
#include "../ui/input.h"
#include "../ui/ui.h"
#include "../Game/game.h"
using namespace std;
using namespace sfw;

PosDim	 Entity::getPosDim()			{ return pd; }
int		 Entity::getOwner()				{ return ownerIndex; }
unsigned Entity::getSprite()			{ return sprite; }
int		 Entity::getHP()				{ return hp; }
int		 Entity::getDMG()				{ return dmg; }
int		 Entity::getSpriteIndex()		{ return spriteIndex; }
int		 Entity::getCost()				{ return cost; }
int		 Entity::getColoredIndex() { return colorSprite; }
bool	 Entity::getSelected()			{ return selected; }
char*	 Entity::getName()				{ return name; }
void	 Entity::setPosDim(PosDim PD)	{ pd = PD; }
void	 Entity::setSprite(unsigned s)	{ sprite = s; }
void	 Entity::setHP(int HP)			{ hp = HP; }
void	 Entity::setDMG(int DMG)		{ dmg = DMG; }
void	 Entity::setSelected(bool cr)	{ selected = cr; }
void	 Entity::setSpriteIndex(int si) { spriteIndex = si; }
void	 Entity::setColoredIndex(int ci) { colorSprite = ci; }
void	 Entity::setName(char *n)		{ name = n; }
void	 Entity::setCost(int c)			{ cost = c; }
void	 Entity::setOwner(int playerIndex) { ownerIndex = playerIndex; }

float dist(PosDim start, PosDim end)
{
	return sqrt(expo<float>(end.x - start.x, 2) + expo<float>(end.y - start.y, 2));
}

void initEnts()
{
	u_Empty		= Unit();
	u_AllDynam	= new Unit[unitSpawnIndex];
	u_AllDynam[0] = u_Empty;
					//ID, Name, Sprite, HP, DMG, Speed, Range, Attack Speed, owner index
	u_Human		= Unit(0, "Human\nSoldier", ui_Human, 0, 1, 50, 5, 30.0f, 100.0f, 1.15f, 0);
	u_Invader	= Unit(1, "Invader\nSoldier", ui_Invader, 0, 2, 75, 2, 33.0f, 75.0f, 0.35f, 1);
	u_Hotdog	= Unit(2, "Hotdog", ui_Hotdog, 0, 1, 613, 265, 1145.0f, 132.0f, 0.3065f, 0);
	u_HumanCollector = Unit(3, "Human\nCollector", ui_Collector, 0, 1, 25, 10, 25.0f, 25.0f, 2.0f, 0);
	u_InvaderCollector = Unit(4, "Invader\nCollector", ui_Invader, 1, 3, 25, 10, 25.0f, 25.0f, 2.0f, 1);

	b_Empty		= Building();
	b_AllDynam	= new Building[buildSpawnIndex];
	b_AllDynam[0] = b_Empty;

	b_HumanTC			= Building(0, "Town\nCenter", ui_TownCenter, 2, 3, 500, 0, 0, 0, 0, 0);
	b_HumanTower		= Building(1, "Tower", ui_Tower, 1, 3, 300, 25, 150.0f, 1.5f, 200, 0);
	b_HumanBarracks		= Building(2, "Barracks", ui_Barracks, 0, 1, 200, 0, 0, 0, 350, 0);
	b_InvaderTC			= Building(3, "Town\nCenter", ui_TownCenter, 0, 1, 500, 0, 0, 0, 0, 1);
	b_InvaderTower		= Building(4, "Tower", ui_Tower, 0, 2, 300, 25, 150.0f, 1.5f, 200, 1);
	b_InvaderBarracks   = Building(5, "Barracks", ui_Barracks, 0, 1, 200, 0, 0.0f, 0.0f, 350, 1);
	//b_Farm = Building(6, "Farm", )

	blt_Empty = Bullet();
	blt_AllDynam = new Bullet[bulletSpawnIndex];
	blt_AllDynam[0] = blt_Empty;

	r_Empty = Resource();
	r_AllDynam = new Resource[resourceSpawnIndex];
	r_AllDynam[0] = r_Empty;

	r_Steel = Resource(0, "Steel\nCache", ui_Resource, 0, 0, 500, 0.5f);
	r_Food	= Resource(1, "Food\nCache", ui_Resource, 1, 1, 500, 0.5f);
}

void checkCollision(Unit &u, int index, bool isBuild = false)
{
	PosDim ua[2] = { u.getPosDim() };

	if (isBuild)
	{ 
		if (index >= buildSpawnIndex) { PRINT "Collision index out of range: " << index _ isBuild NL; return; }
		ua[1] = b_AllDynam[index].getPosDim(); 
	}
	else 
	{ 
		if (index >= unitSpawnIndex) { PRINT "Collision index out of range: " << index _ isBuild NL; return; }
		ua[1] = u_AllDynam[index].getPosDim(); 
	}

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
void getCollisionCandidates(Unit &u, bool checkBuilds = false)
{
	float r;
	float dist;
	int spawnIndex = 1;
	PosDim rupd = u.getPosDim();
	PosDim ucpd;

	if (rupd.h > rupd.w) { r = rupd.h; }
	else { r = rupd.w; }

	for (int a = 0; a < spawnIndex; a++)
	{
		if (checkBuilds) { ucpd = b_Current.getPosDim(); spawnIndex = buildSpawnIndex; }
		else { ucpd = u_Current.getPosDim(); spawnIndex = unitSpawnIndex; }
		
		if (checkBuilds) { if (b_Current == b_Empty) { continue; } }
		else { if (u_Current == u_Empty || rupd == ucpd) { continue; } }

		dist = sqrt(expo<float>(abs(rupd.x - ucpd.x), 2) + expo<float>(abs(rupd.y - ucpd.y), 2));

		if (dist <= r)
		{
			if (checkBuilds) { checkCollision(u, a, true); }
			checkCollision(u, a);
			if (u.hasUnitCollided()) { a = spawnIndex; }
		}

		if (a == spawnIndex - 1 && !checkBuilds)
		{
			checkBuilds = true;
			a = 0;
		}
	}
}
void updateEnts()
{
	PosDim _pd;
	isUnitOnOverlay = false;
	isBuildOnOverlay = false;
	isResourceOnOverlay = false;

	//Update all buildings
	for (int a = 0; a < buildSpawnIndex; a++)
	{
		if (b_Current == b_Empty) { continue; }
		_pd = b_Current.getPosDim();

		//Despawn dead buildings
		if (b_Current.getHP() <= 0)
		{
			int count = 0;
			if (b_Current == b_HumanBarracks)
			{
				for (int a = 0; a < 10; a++) { if (b_Current.queue[a] != -1) { count++; } else { a = 10; } }
				p_AI.addMoney((count * 10));
			}
			else if (b_Current == b_HumanTower) { p_AI.addMoney(50); }
			else if (b_Current == b_InvaderBarracks)
			{
				count = 0;
				for (int a = 0; a < 10; a++) { if (b_Current.queue[a] != -1) { count++; } else { a = 10; } }
				p_Player.addMoney((count * 10));
			}
			else if (b_Current == b_InvaderTower) { p_Player.addMoney(50); }
			b_Current = b_Empty;
		}
		else
		{
			updateQueue(b_Current);
			drawTexture(b_Current.getSprite(), _pd.x, _pd.y, _pd.w, _pd.h, 0, true, b_Current.getSpriteIndex());
			if (b_Current == b_HumanTC || b_Current == b_HumanTower || b_Current == b_HumanBarracks)
			{
				drawTexture(b_Current.getSprite(), _pd.x, _pd.y, _pd.w, _pd.h, 0, true, b_Current.getColoredIndex(), p_Player.getPlayerColor());
			}
			else if (b_Current == b_InvaderTC || b_Current == b_InvaderTower || b_Current == b_InvaderBarracks)
			{
				drawTexture(b_Current.getSprite(), _pd.x, _pd.y, _pd.w, _pd.h, 0, true, b_Current.getColoredIndex(), p_AI.getPlayerColor());
			}

			if (b_Current == b_InvaderTower || b_Current == b_HumanTower) { checkForTargets(b_Current); }

			if (b_Current.getSelected())
			{
				drawBox(_pd.x - (_pd.w / 2), _pd.y - (_pd.h / 2), _pd.h, _pd.w, MAGENTA);
				if (!isBuildOnOverlay) { isBuildOnOverlay = true; entOnOverlay = a; }
			}
		}
	}

	//Update units
	for (int a = 0; a < unitSpawnIndex; a++)
	{
		if (u_Current != u_Empty)
		{
			//Despawn dead units
			if (u_Current.getHP() <= 0) 
			{ 
				if (u_Current == u_Invader) { p_Player.addMoney(10); }
				else if (u_Current == u_Human) { p_AI.addMoney(10); }
				u_Current = u_Empty; 
			}
			else
			{
				_pd = u_Current.getPosDim();

				//Units only collide with each other if they don't have a target, because reasons.
				//Go ahead, comment out that getTarget() parameter. See what happens.
				if (!u_Current.hasUnitCollided() && u_Current.getTarget() == NOTHING) { getCollisionCandidates(u_Current); }
				
				if (u_Current.getTarget() != NOTHING && !u_Current.doesUnitHaveTarget()) { findTarget(u_Current); }
				else if (u_Current.getTarget() != NOTHING && u_Current.getDistToTar() > u_Current.getAtkRad() * 1.5) { findTarget(u_Current); }

				if (u_Current.isTargetInRange()) { shootTarget(u_Current); }
				
				if (u_Current.isUnitMoving() && !u_Current.isTargetInRange()) { moveUnit(u_Current); }
				
				if (u_Current.doesUnitHaveSavedCoords() && !u_Current.isUnitMoving() && !u_Current.isTargetInRange())
				{
					u_Current.setTargetCoords(u_Current.getSavedX(), u_Current.getSavedY());
					u_Current.setUnitCollision(false);
				}

				drawTexture(u_Current.getSprite(), _pd.x, _pd.y, _pd.w, _pd.h, 0, true, u_Current.getSpriteIndex());
				if (u_Current.getOwner() == 0)
				{
					drawTexture(u_Current.getSprite(), _pd.x, _pd.y, _pd.w, _pd.h, 0, true, u_Current.getColoredIndex(), p_Player.getPlayerColor());
				}
				else if (u_Current.getOwner() == 1)
				{
					drawTexture(u_Current.getSprite(), _pd.x, _pd.y, _pd.w, _pd.h, 0, true, u_Current.getColoredIndex(), p_AI.getPlayerColor());
				}


				if (u_Current.getSelected())
				{
					drawBox(_pd.x - (_pd.w / 2), _pd.y - (_pd.h / 2), _pd.h, _pd.w, MAGENTA);
					if (!isUnitOnOverlay) { isUnitOnOverlay = true; entOnOverlay = a; }
				}
			}
		}
	}

	//Update resources
	for (int a = 0; a < resourceSpawnIndex; a++)
	{
		if (r_Current.getHP() <= 0) { r_Current = r_Empty; continue; }

		_pd = r_Current.getPosDim();

		switch (r_Current.getID())
		{
		case 1:
			drawTexture(r_Current.getSprite(), _pd.x, _pd.y, _pd.w, _pd.h, 90, true, r_Current.getSpriteIndex());
			break;
		default:
			drawTexture(r_Current.getSprite(), _pd.x, _pd.y, _pd.w, _pd.h, 0, true, r_Current.getSpriteIndex());
		}

		if (r_Current.getSelected())
		{
			switch (r_Current.getID())
			{
			case 1:
				_pd.w = frac<float>(_pd.w, 7, 10);
				drawBox(_pd.x - (_pd.h / 2), _pd.y - (_pd.w / 2), _pd.w, _pd.h, MAGENTA);
				break;
			default:
				drawBox(_pd.x - (_pd.w / 2), _pd.y - (_pd.h / 2), _pd.h, _pd.w, MAGENTA);
			}

			if (!isResourceOnOverlay) { isResourceOnOverlay = true; entOnOverlay = a; }
		}
	}

	//Update bullets
	for (int a = 0; a < bulletSpawnIndex; a++)
	{
		if (blt_Current == blt_Empty) { continue; }

		_pd = blt_Current.getPosDim();

		switch (blt_Current.getOwner())
		{
		case 0: drawCircle(_pd.x, _pd.y, 2, 6, p_Player.getPlayerColor()); break;
		case 1: drawCircle(_pd.x, _pd.y, 2, 6, p_AI.getPlayerColor()); break;
		}

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
	bool gotPlayerUnit = false;
	bool gotBuild = false;
	bool gotPlayerBuild = false;
	bool dcSelected = false;

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

		if (u_Current.getOwner() == 0 && dcSelected) { continue; }

		if (u_Current.getOwner() != 0 && gotPlayerUnit) { u_Current.setSelected(false); continue; }

		if (pd.y + (pd.h / 2) >= selbox[1] && pd.y - (pd.h / 2) <= selbox[1] + selbox[2])
		{
			if (pd.x - (pd.w / 2) <= selbox[0] + selbox[3] && pd.x + (pd.w / 2) >= selbox[0])
			{
				if (u_Current.getOwner() == 0 && u_Current.getSelected() && startDCT)
				{
					cout << "Double Click passed\n";
					for (int b = 0; b < unitSpawnIndex; ++b)
					{
						if (u_AllDynam[b] == u_Current && u_AllDynam[b].getTarget() == u_Current.getTarget())
						{
							u_AllDynam[b].setSelected(true);
							cout << b << "," << u_AllDynam[b].getSelected() << " ";
						}
					}
					dcSelected = true;
					cout << endl;
				}
				else
				{
					u_Current.setSelected(true);
					if (u_Current.getOwner() == 0 && !gotPlayerUnit)
					{
						 gotPlayerUnit = true;
						for (int b = a; b != -1; b--) { if (u_AllDynam[b].getOwner() != 0) { u_AllDynam[b].setSelected(false); } }
					}
					gotUnit = true;
					startDCT = true;
				}
			}
			else if (!isShiftPressed) { u_Current.setSelected(false); }
		}
		else if (!isShiftPressed) { u_Current.setSelected(false); }
	}

	for (int a = 0; a < buildSpawnIndex; a++)
	{
		if (dcSelected) { continue; }
		if (gotUnit) { b_Current.setSelected(false); continue; }
		if (b_Current.getOwner() == 1 && gotPlayerBuild) { b_Current.setSelected(false); continue; }

		pd = b_Current.getPosDim();

		if (pd.y + (pd.h / 2) >= selbox[1] && pd.y - (pd.h / 2) <= selbox[1] + selbox[2])
		{
			if (pd.x - (pd.w / 2) <= selbox[0] + selbox[3] && pd.x + (pd.w / 2) >= selbox[0])
			{
				if (b_Current.getOwner() == 0 && b_Current.getSelected() && startDCT)
				{
					dcSelected = true;
					for (int b = 0; b < buildSpawnIndex; b++)
					{
						if (b_AllDynam[b] == b_Current) { b_AllDynam[b].setSelected(true); }
						else { b_AllDynam[b].setSelected(false); }
					}
				}
				else
				{
					b_Current.setSelected(true);
					gotBuild = true;
					if (b_Current.getOwner() == 0) { gotPlayerBuild = true; }
					startDCT = true;
				}
			}
			else if (!isShiftPressed) { b_Current.setSelected(false); }
		}
		else if (!isShiftPressed) { b_Current.setSelected(false); }
	}

	for (int a = 0; a < resourceSpawnIndex; a++)
	{
		if (dcSelected) { continue; }
		if (gotUnit || gotBuild) { r_Current.setSelected(false); continue; }

		pd = r_Current.getPosDim();

		if (pd.y + (pd.h / 2) >= selbox[1] && pd.y - (pd.h / 2) <= selbox[1] + selbox[2])
		{
			if (pd.x - (pd.w / 2) <= selbox[0] + selbox[3] && pd.x + (pd.w / 2) >= selbox[0])
			{
				r_Current.setSelected(true);
			}
			else if (!isShiftPressed) { r_Current.setSelected(false); }
		}
		else if (!isShiftPressed) { r_Current.setSelected(false); }
	}
}