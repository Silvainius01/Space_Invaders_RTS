#include "./input.h"
#include "./ui.h"
#include "../ent/ent.h"
#include "../math.h"
#include "../fileio/fileio.h"
#include "../Game/game.h"

#include <iostream>
#include <string>
#include <sstream>
using namespace std;



char *doTheThingToProfName()
{
	char *temp = new char[strlen(profileName) + 1];
	char *name = temp;
	
	for (int a = 0; a < strlen(profileName); a++)
	{
		temp[a + 1] = profileName[a];
	}

	temp[0] = '\n';
	for (int a = 0; name[a] != '\0'; a++)
	{
		name[a] = temp[a];
	}
	delete[]temp;
	return name;
}


Button m_EntOverlay[] = {	Button("Default", xSpace(50), ySpace(7.5), 2, 2, clr_WHITE, CENTER, false),
							Button("HP :", xSpace(5), ySpace(14), 2, 2, clr_WHITE, RIGHT, false), Button("NV", m_EntOverlay[1].getXPos() + (strlen(m_EntOverlay[1].getName()) * 9 * 3), m_EntOverlay[1].getYPos(), m_EntOverlay[1].getHeight(), m_EntOverlay[1].getWidth(), clr_WHITE, RIGHT, false),
							Button("DMG:", xSpace(5), ySpace(11.5), 2, 2, clr_WHITE, RIGHT, false), Button("NV", m_EntOverlay[3].getXPos() + (strlen(m_EntOverlay[3].getName()) * 9 * 3), m_EntOverlay[3].getYPos(), m_EntOverlay[3].getHeight(), m_EntOverlay[3].getWidth(), clr_WHITE, RIGHT, false),
							Button("Owner:", xSpace(5), ySpace(9), 2, 2, clr_WHITE, RIGHT, false), Button(profileName, m_EntOverlay[5].getXPos(), m_EntOverlay[5].getYPos() - ySpace(2.5), m_EntOverlay[5].getHeight(), m_EntOverlay[5].getWidth(), playerColor, RIGHT, false),
							Button("Target:", xSpace(63), ySpace(14), 2, 2, clr_WHITE, RIGHT, false),
							Button("Nothing", xSpace(63), ySpace(11.5), 2, 2, clr_MAGENTA, RIGHT, false),
							Button("Units",xSpace(63), ySpace(9), 2, 2, clr_WHITE, RIGHT),
							Button("Buildings", xSpace(63), ySpace(6.5), 2, 2, clr_WHITE, RIGHT),
							Button("\0", xSpace(105), m_EntOverlay[10].getYPos(), 2, 2, clr_WHITE, RIGHT, false),
							Button(" percent", xSpace(105), m_EntOverlay[11].getYPos(), 2, 2, clr_WHITE, RIGHT, false),
							Button("\0", m_EntOverlay[8].getXPos() + ((strlen(m_EntOverlay[8].getName()) - 1) * 9 * 3), m_EntOverlay[8].getYPos(), m_EntOverlay[8].getHeight(), m_EntOverlay[8].getWidth(), clr_WHITE, RIGHT, true),
							Button("Health", xSpace(101), m_EntOverlay[9].getYPos(), m_EntOverlay[9].getHeight(), m_EntOverlay[9].getWidth(), clr_WHITE, RIGHT, true),
							Button() };

Button m_PlyrOverlay[] = {  Button("Money:", xSpace(5), ySpace(14), 2, 2, clr_WHITE, RIGHT, false), Button("NV", m_PlyrOverlay[0].getXPos() + (strlen(m_PlyrOverlay[0].getName()) * 9 * 3), m_PlyrOverlay[0].getYPos(), m_PlyrOverlay[0].getHeight(), m_PlyrOverlay[0].getWidth(), clr_GREEN, RIGHT, false),
							Button("Steel:", xSpace(5), ySpace(11.5), 2, 2, clr_WHITE, RIGHT, false), Button("NV", m_PlyrOverlay[2].getXPos() + (strlen(m_PlyrOverlay[2].getName()) * 9 * 3), m_PlyrOverlay[2].getYPos(), m_PlyrOverlay[2].getHeight(), m_PlyrOverlay[2].getWidth(), clr_STEEL, RIGHT, false),
							Button("Food :", xSpace(5), ySpace(9), 2, 2, clr_WHITE, RIGHT, false), Button("NV", m_PlyrOverlay[4].getXPos() + (strlen(m_PlyrOverlay[4].getName()) * 9 * 3), m_PlyrOverlay[4].getYPos(), m_PlyrOverlay[4].getHeight(), m_PlyrOverlay[4].getWidth(), clr_RED, RIGHT, false),
							Button("Buy:", xSpace(63), ySpace(14), 2, 2, clr_WHITE, RIGHT, false),
							Button("Barracks", xSpace(63), ySpace(11.5), 2, 2, clr_WHITE, RIGHT),
							Button("Tower", xSpace(63), ySpace(9), 2, 2, clr_WHITE, RIGHT),
							Button("Cost: ", xSpace(63), ySpace(6.5), 2, 2, clr_WHITE, RIGHT, false), Button("NV", m_PlyrOverlay[9].getXPos() + (strlen(m_PlyrOverlay[9].getName()) * 9 * 3), m_PlyrOverlay[9].getYPos(), m_PlyrOverlay[9].getHeight(), m_PlyrOverlay[9].getWidth(), clr_GREEN, RIGHT, false),
							Button() };

void drawGameOverlay()
{
	drawTexture(ui_Overlay, xSpace(50), ySpace(50), xSpace(1, 1), ySpace(1, 1));
	m_EntOverlay[6].setTint(playerColor);
	m_EntOverlay[6].setName(profileName);
	m_EntOverlay[1].setName("HP :");
	m_EntOverlay[3].setName("DMG:");
	m_EntOverlay[4].setTint(clr_WHITE);
	m_EntOverlay[5].setName("Owner:");

	if (isUnitOnOverlay)
	{
		int HP = u_EntOnOverlay.getHP();
		int DMG = u_EntOnOverlay.getDMG();
		char hp[4] = { itc(getHundredsPlace(HP)), itc(getTensPlace(HP)), itc(HP % 10), '\0' };
		char dmg[3] = { (itc(getTensPlace(DMG))), itc(DMG % 10), '\0' };
		PosDim pd = u_EntOnOverlay.getPosDim();
		drawTexture(u_EntOnOverlay.getSprite(), xSpace(50), ySpace(11), pd.w, pd.h);
		drawCircle(pd.x, pd.y, u_EntOnOverlay.getAtkRad());
		m_EntOverlay[0].setName(u_EntOnOverlay.getName());
		m_EntOverlay[2].setName(hp);
		m_EntOverlay[4].setName(dmg);
		if (u_EntOnOverlay.getOwner() == 1) { m_EntOverlay[6].setName("Invaders"); m_EntOverlay[6].setTint(p_AI.getPlayerColor()); }
		else { m_EntOverlay[6].setTint(playerColor); m_EntOverlay[6].setName(profileName); }
		m_EntOverlay[7].setName("Target:");
		m_EntOverlay[8].setName("Nothing");
		switch (u_EntOnOverlay.getUnitType())
		{
		case 1:
			m_EntOverlay[9].setName("Food");
			m_EntOverlay[10].setName("Steel");
			break;
		default:
			m_EntOverlay[9].setName("Units");
			m_EntOverlay[10].setName("Buildings");
		}
		m_EntOverlay[11].setName('\0');
		switch (u_EntOnOverlay.getTarget())
		{
		case NOTHING:
			m_EntOverlay[8].setTint(clr_MAGENTA); m_EntOverlay[8].setClickable(false);
			m_EntOverlay[9].setTint(clr_WHITE);  
			if (u_EntOnOverlay.getOwner() == 0) { m_EntOverlay[9].setClickable(true); }
			else { m_EntOverlay[9].setClickable(false); }
			m_EntOverlay[10].setTint(clr_WHITE); 
			if(u_EntOnOverlay.getOwner() == 0) { m_EntOverlay[10].setClickable(true); }
			else { m_EntOverlay[10].setClickable(false); }
			break;
		case UNITS: case FOOD:
			m_EntOverlay[8].setTint(clr_WHITE); 
			if (u_EntOnOverlay.getOwner() == 0) { m_EntOverlay[8].setClickable(true); }
			else { m_EntOverlay[8].setClickable(false); }
			m_EntOverlay[9].setTint(clr_MAGENTA), m_EntOverlay[9].setClickable(false);
			m_EntOverlay[10].setTint(clr_WHITE); 
			if (u_EntOnOverlay.getOwner() == 0) { m_EntOverlay[10].setClickable(true); }
			else { m_EntOverlay[10].setClickable(false); }
			break;
		case BUILDINGS: case STEEL:
			m_EntOverlay[8].setTint(clr_WHITE);
			if (u_EntOnOverlay.getOwner() == 0) { m_EntOverlay[8].setClickable(true); }
			else { m_EntOverlay[8].setClickable(false); }
			m_EntOverlay[9].setTint(clr_WHITE); 
			if (u_EntOnOverlay.getOwner() == 0) { m_EntOverlay[9].setClickable(true); }
			else { m_EntOverlay[9].setClickable(false); }
			m_EntOverlay[10].setTint(clr_MAGENTA), m_EntOverlay[10].setClickable(false);
			break;
		}
		switch (drawMenu(m_EntOverlay))
		{
		case 8:
			for (int a = 0; a < unitSpawnIndex; a++) 
			{ 
				if (u_Current.getSelected() && u_Current.getOwner() == 0) 
				{ 
					u_Current.setTarget(NOTHING); 
					u_Current.setTargetStatus(false); 
				} 
			}
			break;
		case 9:
			for (int a = 0; a < unitSpawnIndex; a++) 
			{ 
				if (u_Current.getSelected() && u_Current.getOwner() == 0) 
				{ 
					switch (u_Current.getUnitType())
					{
					case 1: u_Current.setTarget(FOOD); break;
					default: u_Current.setTarget(UNITS);
					}
					u_Current.setTargetStatus(false); 
				} 
			}
			break;
		case 10:
			for (int a = 0; a < unitSpawnIndex; a++)
			{ 
				if (u_Current.getSelected() && u_Current.getOwner() == 0)
				{ 
					switch (u_Current.getUnitType())
					{
					case 1: u_Current.setTarget(STEEL); break;
					default: u_Current.setTarget(BUILDINGS);
					}
					u_Current.setTargetStatus(false); 
				} 
			}
			break;
		}
	}
	else if (isBuildOnOverlay)
	{
		int HP = b_EntOnOverlay.getHP();
		int DMG = b_EntOnOverlay.getDMG();
		int percent = 100 * (b_EntOnOverlay.getElapsedTrainTime() / b_EntOnOverlay.getTrainTime());
		char hp[4] = { itc(getHundredsPlace(HP)), itc(getTensPlace(HP)), itc(HP % 10), '\0' };
		char dmg[4] = { itc(getHundredsPlace(DMG)), itc(getTensPlace(DMG)), itc(DMG % 10), '\0' };
		char train[3]{ itc(getTensPlace(percent)), itc(percent % 10), '\0' };
		PosDim pd = b_EntOnOverlay.getPosDim();
		drawTexture(b_EntOnOverlay.getSprite(), xSpace(50), ySpace(11), pd.w, pd.h, 0, true, b_AllDynam[entOnOverlay].getSpriteIndex());
		m_EntOverlay[0].setName(b_EntOnOverlay.getName());
		m_EntOverlay[2].setName(hp);
		if (b_EntOnOverlay == b_HumanTower || b_EntOnOverlay == b_InvaderTower) { m_EntOverlay[4].setName(dmg); }
		else { m_EntOverlay[4].setName("NV"); }
		if (b_EntOnOverlay.getOwner() == 1) { m_EntOverlay[6].setName("Invaders"); m_EntOverlay[6].setTint(p_AI.getPlayerColor()); }
		else { m_EntOverlay[6].setTint(playerColor); m_EntOverlay[6].setName(profileName); }
		if (b_EntOnOverlay == b_HumanBarracks || b_EntOnOverlay == b_InvaderBarracks)
		{
			char *cnt;
			int count = 0;
			for (int a = 0; a < b_EntOnOverlay.getMaxQueue(); a++) { if (b_EntOnOverlay.queue[a] != -1) { count++; } }
			switch (count)
			{
			case 1: cnt = "1"; break;
			case 2: cnt = "2"; break;
			case 3: cnt = "3"; break;
			case 4: cnt = "4"; break;
			case 5: cnt = "5"; break;
			case 6: cnt = "6"; break;
			case 7: cnt = "7"; break;
			case 8: cnt = "8"; break;
			case 9: cnt = "9"; break;
			case 10: cnt = "10"; break;
			default: cnt = "0"; break;
			}
			m_EntOverlay[7].setName("Train:");
			m_EntOverlay[8].setName("Soldier");
			m_EntOverlay[8].setClickable(true);
			m_EntOverlay[9].setName("Queue:");
			m_EntOverlay[9].setClickable(false);
			m_EntOverlay[10].setClickable(false);
			m_EntOverlay[10].setName(cnt);
			m_EntOverlay[11].setName(train);
			m_EntOverlay[11].setXPos(m_EntOverlay[10].getXPos() + ((strlen(m_EntOverlay[10].getName()) + 1) * 9 * 3));
			m_EntOverlay[12].setXPos(m_EntOverlay[11].getXPos() + (strlen(m_EntOverlay[11].getName()) * 9));
			m_EntOverlay[13].setName("Collector");
			m_EntOverlay[13].setXPos(m_EntOverlay[8].getXPos() + ((strlen(m_EntOverlay[8].getName()) - 1) * 9 * 3));
			m_EntOverlay[14].setName("\0");
		}
		else if (b_EntOnOverlay == b_HumanTC)
		{
			int STEEL = 0;
			int FOOD  = 0;
			
			// lolz more EAS
			if		(m_EntOverlay[ 8].isButtonHighlighted()) { STEEL = 150; FOOD = 150; }
			else if (m_EntOverlay[ 9].isButtonHighlighted()) { STEEL = 250; FOOD =  50; }
			else if (m_EntOverlay[13].isButtonHighlighted()) { STEEL = 100; FOOD = 200; }
			else if (m_EntOverlay[14].isButtonHighlighted()) { STEEL =  50; FOOD = 250; }
			
			char steel[4] = { itc(getHundredsPlace(STEEL)), itc(getTensPlace(STEEL)), itc(STEEL % 10), '\0' };
			char food [4] = { itc(getHundredsPlace(FOOD) ), itc(getTensPlace(FOOD) ), itc(FOOD  % 10), '\0' };
			m_EntOverlay[7].setName("Unit Upgrades:");
			m_EntOverlay[8].setName("Range");
			if (m_EntOverlay[8].getTint() != clr_STEEL) { m_EntOverlay[8].setClickable(true); }
			m_EntOverlay[9].setName("Damage");
			if (m_EntOverlay[9].getTint() != clr_STEEL) { m_EntOverlay[9].setClickable(true); }
			m_EntOverlay[13].setName("Rate of Fire");
			if (m_EntOverlay[13].getTint() != clr_STEEL) { m_EntOverlay[13].setClickable(true); }
			m_EntOverlay[13].setXPos(m_EntOverlay[8].getXPos() + ((strlen(m_EntOverlay[8].getName()) - 1) * 9 * 3));
			m_EntOverlay[10].setName("Cost:");
			m_EntOverlay[11].setName(steel);
			m_EntOverlay[11].setTint(clr_STEEL);
			m_EntOverlay[11].setXPos(m_EntOverlay[10].getXPos() + ((strlen(m_EntOverlay[10].getName())) * 9 * 3));
			m_EntOverlay[12].setName(food);
			m_EntOverlay[12].setTint(clr_RED);
			m_EntOverlay[12].setXPos(m_EntOverlay[11].getXPos() + ((strlen(m_EntOverlay[11].getName())) * 9 * 3));
			m_EntOverlay[14].setName("Health");
			m_EntOverlay[14].setXPos(m_EntOverlay[9].getXPos() + ((strlen(m_EntOverlay[9].getName()) - 1) * 9 * 3));
		}
		else if (b_EntOnOverlay == b_HumanTower)
		{
			int STEEL = 0;
			int FOOD = 0;

			// lolz more EAS
			if (m_EntOverlay[8].isButtonHighlighted()) { STEEL = 150; FOOD = 150; }
			else if (m_EntOverlay[9].isButtonHighlighted()) { STEEL = 250; FOOD = 50; }
			else if (m_EntOverlay[13].isButtonHighlighted()) { STEEL = 100; FOOD = 200; }

			char steel[4] = { itc(getHundredsPlace(STEEL)), itc(getTensPlace(STEEL)), itc(STEEL % 10), '\0' };
			char food[4] = { itc(getHundredsPlace(FOOD)), itc(getTensPlace(FOOD)), itc(FOOD % 10), '\0' };
			m_EntOverlay[7].setName("Building Upgrades:");
			m_EntOverlay[8].setName("Range");
			if (m_EntOverlay[8].getTint() != clr_STEEL) { m_EntOverlay[8].setClickable(true); }
			m_EntOverlay[9].setName("Damage");
			if (m_EntOverlay[9].getTint() != clr_STEEL) { m_EntOverlay[9].setClickable(true); }
			m_EntOverlay[13].setName("Rate of Fire");
			if (m_EntOverlay[13].getTint() != clr_STEEL) { m_EntOverlay[13].setClickable(true); }
			m_EntOverlay[13].setXPos(m_EntOverlay[8].getXPos() + ((strlen(m_EntOverlay[8].getName()) - 1) * 9 * 3));
			m_EntOverlay[10].setName("Cost:");
			m_EntOverlay[10].setClickable(false);
			m_EntOverlay[11].setName(steel);
			m_EntOverlay[11].setTint(clr_STEEL);
			m_EntOverlay[11].setXPos(m_EntOverlay[10].getXPos() + ((strlen(m_EntOverlay[10].getName())) * 9 * 3));
			m_EntOverlay[12].setName(food);
			m_EntOverlay[12].setTint(clr_RED);
			m_EntOverlay[12].setXPos(m_EntOverlay[11].getXPos() + ((strlen(m_EntOverlay[11].getName())) * 9 * 3));
		}
		else { m_EntOverlay[7].setName('\0'); }
		switch (drawMenu(m_EntOverlay))
		{
		case 8:
			
			if (b_EntOnOverlay == b_HumanBarracks)
			{
				for (int a = 0; a < buildSpawnIndex; a++)
				{
					if (b_Current == b_HumanBarracks && b_Current.getSelected())
					{
						addToQueue(u_Human, b_Current);
					}
				}
			}
			else if (b_EntOnOverlay == b_HumanTC)
			{
				if (p_Player.getSteel() >= 150 && p_Player.getFood() >= 150)
				{
					p_Player.addSteel(-150);
					p_Player.addFodd(-150);
					for (int a = 0; a < unitSpawnIndex; a++)
					{
						if (u_Current.getOwner() == 0) { u_Current.setAtkRad(u_Current.getAtkRad() + 5.0f); }
					}
					for (int a = 0; a < unitIndex; a++)
					{
						if (u_AllBase[a].getOwner() == 0) { u_AllBase[a].setAtkRad(u_AllBase[a].getAtkRad() + 5.0f); }
					}
				}
			}
			else if (b_EntOnOverlay == b_HumanTower)
			{
				if (p_Player.getSteel() >= 150 && p_Player.getFood() >= 150)
				{
					p_Player.addSteel(-150);
					p_Player.addFodd(-150);
					for (int a = 0; a < buildSpawnIndex; a++)
					{
						if (b_Current.getOwner() == 0 && b_Current.getAtkRad() > 0)
						{
							b_Current.setAtkRad(b_Current.getAtkRad() + 20.0f);
							if (a < buildingIndex)
							{
								b_AllBase[a].setAtkRad(b_AllBase[a].getAtkRad() + 10);
							}
						}
					}
				}
				
			}
			break;
		case 9:
			if (b_EntOnOverlay == b_HumanTC)
			{
				if (p_Player.getSteel() >= 250 && p_Player.getFood() >= 50)
				{
					p_Player.addSteel(-250);
					p_Player.addFodd(-50);
					for (int a = 0; a < unitSpawnIndex; a++)
					{
						if (u_Current.getOwner() == 0) { u_Current.setDMG(u_Current.getDMG() + 2); }
					}
					for (int a = 0; a < unitIndex; a++)
					{
						if (u_AllBase[a].getOwner() == 0) { u_AllBase[a].setDMG(u_AllBase[a].getDMG() + 2); }
					}
				}
			}
			else if (b_EntOnOverlay == b_HumanTower)
			{
				if (p_Player.getSteel() >= 250 && p_Player.getFood() >= 50)
				{
					p_Player.addSteel(-250);
					p_Player.addFodd(-50);
					for (int a = 0; a < buildSpawnIndex; a++)
					{
						if (b_Current.getOwner() == 0 && b_Current.getDMG() > 0)
						{
							b_Current.setDMG(b_Current.getDMG() + 10);
							if (a < buildingIndex)
							{
								b_AllBase[a].setDMG(b_AllBase[a].getDMG() + 10);
							}
						}
					}
				}
			}
		case 13:
			if (b_EntOnOverlay == b_HumanBarracks)
			{
				for (int a = 0; a < buildSpawnIndex; a++)
				{
					if (b_Current == b_HumanBarracks && b_Current.getSelected())
					{
						addToQueue(u_HumanCollector, b_Current);
					}
				}
			}
			else if (b_EntOnOverlay == b_HumanTC)
			{
				if (p_Player.getSteel() >= 100 && p_Player.getFood() >= 200)
				{
					p_Player.addSteel(-100);
					p_Player.addFodd(-200);
					for (int a = 0; a < unitSpawnIndex; a++)
					{
						if (u_Current.getOwner() == 0) { u_Current.setAttackSpeed(u_Current.getAttackSpeed() - 0.1f); }
					}
					for (int a = 0; a < unitIndex; a++)
					{
						if (u_AllBase[a].getOwner() == 0) { u_AllBase[a].setAttackSpeed(u_AllBase[a].getAttackSpeed() - 0.1f); }
					}
				}
			}
			else if (b_EntOnOverlay == b_HumanTower)
			{
				if (p_Player.getSteel() >= 100 && p_Player.getFood() >= 200)
				{
					p_Player.addSteel(-100);
					p_Player.addFodd(-200);
					for (int a = 0; a < buildSpawnIndex; a++)
					{
						if (b_Current.getOwner() == 0 && b_Current.getAtkSpeed() > 0)
						{
							b_Current.setAtkSpeed(b_Current.getAtkSpeed() - 0.25f);
							if (a < buildingIndex)
							{
								b_AllBase[a].setAtkSpeed(b_AllBase[a].getAtkSpeed() - 0.25f);
							}
						}
					}
				}
			}
		case 14:
			if (b_EntOnOverlay == b_HumanTC)
			{
				if (p_Player.getSteel() >= 50 && p_Player.getFood() >= 250)
				{
					p_Player.addSteel(-50);
					p_Player.addFodd(-250);
					for (int a = 0; a < unitIndex; a++)
					{
						if (u_AllBase[a].getOwner() == 0) { u_AllBase[a].setHP(u_AllBase[a].getHP() + 5); }
					}
				}
				
			}
		default:
			break;
		}
		if (b_EntOnOverlay == b_HumanBarracks && getKey('S'))
		{
			for (int a = 0; a < buildSpawnIndex; a++)
			{
				if (b_Current == b_HumanBarracks && b_Current.getSelected())
				{
					addToQueue(u_Human, b_Current);
				}
			}
		}
	}
	else if (isResourceOnOverlay)
	{
		int HP = r_EntOnOverlay.getHP();
		char hp[4] = { itc(getHundredsPlace(HP)), itc(getTensPlace(HP)), itc(HP % 10), '\0' };
		m_EntOverlay[0].setName(r_EntOnOverlay.getName());
		m_EntOverlay[1].setName("Amount Left:");
		m_EntOverlay[4].setName(hp); 
		m_EntOverlay[3].setName(" "); m_EntOverlay[2].setName(" ");
		m_EntOverlay[5].setName('\0');
		PosDim pd = r_EntOnOverlay.getPosDim();
		switch (r_EntOnOverlay.getID())
		{
		case 1:
			drawTexture(r_EntOnOverlay.getSprite(), xSpace(50), ySpace(11), pd.w, pd.h, 90, true, r_EntOnOverlay.getSpriteIndex());
			m_EntOverlay[4].setTint(m_PlyrOverlay[5].getTint());
			break;
		default:
			drawTexture(r_EntOnOverlay.getSprite(), xSpace(50), ySpace(11), pd.w, pd.h, 0, true, r_EntOnOverlay.getSpriteIndex());
			m_EntOverlay[4].setTint(m_PlyrOverlay[3].getTint());
		}
		drawMenu(m_EntOverlay);
	}
	else
	{
		int COST = 0;
		int MONEY = p_Player.getMoney();
		int STEEL = p_Player.getSteel();
		int FOOD = p_Player.getFood();

		if (MONEY > 9999) { MONEY = 9999; }
		if (m_PlyrOverlay[7].isButtonHighlighted()) { COST = b_HumanBarracks.getCost(); }
		else if (m_PlyrOverlay[8].isButtonHighlighted()) { COST = b_HumanTower.getCost(); }

		//Lol Esme Allignment Syndrome (EAS)
		char money[5] = { itc(getThousandsPlace(MONEY)), itc(getHundredsPlace(MONEY)), itc(getTensPlace(MONEY)), itc(MONEY % 10), '\0' };
		char cost [4] = {								 itc(getHundredsPlace(COST) ), itc(getTensPlace(COST) ), itc(COST  % 10), '\0' };
		char steel[5] = { itc(getThousandsPlace(STEEL)), itc(getHundredsPlace(STEEL)), itc(getTensPlace(STEEL)), itc(STEEL % 10), '\0' };
		char food [5] = { itc(getThousandsPlace(FOOD) ), itc(getHundredsPlace(FOOD) ), itc(getTensPlace(FOOD) ), itc(FOOD  % 10), '\0' };

		m_PlyrOverlay[10].setName(cost);
		m_PlyrOverlay[1].setName(money);
		m_PlyrOverlay[3].setName(steel);
		m_PlyrOverlay[5].setName(food);

		switch (drawMenu(m_PlyrOverlay))
		{
		case 7: 
			if(p_Player.getMoney() >= b_HumanBarracks.getCost()) { overrideMouse = 0; }
			break;
		case 8:	
			if (p_Player.getMoney() >= b_HumanTower.getCost()) { overrideMouse = 1; }
			break;
		default: break;
		}
	}
}