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

#define u_EntOnOverlay u_AllDynam[entOnOverlay]
#define b_EntOnOverlay b_AllDynam[entOnOverlay]
#define r_EntOnOverlay r_AllDynam[entOnOverlay]

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


Button m_EntOverlay[] =	   {Button("Default", xSpace(50), ySpace(7.5), 2, 2, WHITE, CENTER, false),
							Button("HP :", xSpace(5), ySpace(14), 2, 2, WHITE, RIGHT, false), Button("NV", m_EntOverlay[1].getXPos() + (strlen(m_EntOverlay[1].getName()) * 9 * 3), m_EntOverlay[1].getYPos(), m_EntOverlay[1].getHeight(), m_EntOverlay[1].getWidth(), WHITE, RIGHT, false),
							Button("DMG:", xSpace(5), ySpace(11.5), 2, 2, WHITE, RIGHT, false), Button("NV", m_EntOverlay[3].getXPos() + (strlen(m_EntOverlay[3].getName()) * 9 * 3), m_EntOverlay[3].getYPos(), m_EntOverlay[3].getHeight(), m_EntOverlay[3].getWidth(), WHITE, RIGHT, false),
							Button("Owner:", xSpace(5), ySpace(9), 2, 2, WHITE, RIGHT, false), Button(profileName, m_EntOverlay[5].getXPos(), m_EntOverlay[5].getYPos() - ySpace(2.5), m_EntOverlay[5].getHeight(), m_EntOverlay[5].getWidth(), playerColor, RIGHT, false),
							Button("Target:", xSpace(63), ySpace(14), 2, 2, WHITE, RIGHT, false),
							Button("Nothing", xSpace(63), ySpace(11.5), 2, 2, MAGENTA, RIGHT, false),
							Button("Units",xSpace(63), ySpace(9), 2, 2, WHITE, RIGHT),
							Button("Buildings", xSpace(63), ySpace(6.5), 2, 2, WHITE, RIGHT),
							Button("\0", xSpace(105), m_EntOverlay[10].getYPos(), 2, 2, WHITE, RIGHT, false),
							Button(" percent", xSpace(105), m_EntOverlay[11].getYPos(), 2, 2, WHITE, RIGHT, false),
							Button() };

Button m_PlyrOverlay[] = {  Button("Money:", xSpace(5), ySpace(14), 2, 2, WHITE, RIGHT, false), Button("NV", m_PlyrOverlay[0].getXPos() + (strlen(m_PlyrOverlay[0].getName()) * 9 * 3), m_PlyrOverlay[0].getYPos(), m_PlyrOverlay[0].getHeight(), m_PlyrOverlay[0].getWidth(), GREEN, RIGHT, false),
							Button("Steel:", xSpace(5), ySpace(11.5), 2, 2, WHITE, RIGHT, false), Button("NV", m_PlyrOverlay[2].getXPos() + (strlen(m_PlyrOverlay[2].getName()) * 9 * 3), m_PlyrOverlay[2].getYPos(), m_PlyrOverlay[2].getHeight(), m_PlyrOverlay[2].getWidth(), 0x8c8888, RIGHT, false),
							Button("Food :", xSpace(5), ySpace(9), 2, 2, WHITE, RIGHT, false), Button("NV", m_PlyrOverlay[4].getXPos() + (strlen(m_PlyrOverlay[4].getName()) * 9 * 3), m_PlyrOverlay[4].getYPos(), m_PlyrOverlay[4].getHeight(), m_PlyrOverlay[4].getWidth(), RED, RIGHT, false),
							Button("Buy:", xSpace(63), ySpace(14), 2, 2, WHITE, RIGHT, false),
							Button("Barracks", xSpace(63), ySpace(11.5), 2, 2, WHITE, RIGHT),
							Button("Tower", xSpace(63), ySpace(9), 2, 2, WHITE, RIGHT),
							Button("Cost: ", xSpace(63), ySpace(6.5), 2, 2, WHITE, RIGHT, false), Button("NV", m_PlyrOverlay[9].getXPos() + (strlen(m_PlyrOverlay[9].getName()) * 9 * 3), m_PlyrOverlay[9].getYPos(), m_PlyrOverlay[9].getHeight(), m_PlyrOverlay[9].getWidth(), GREEN, RIGHT, false),
							Button() };

void drawGameOverlay()
{
	drawTexture(ui_Overlay, xSpace(50), ySpace(50), xSpace(1, 1), ySpace(1, 1));
	m_EntOverlay[6].setTint(playerColor);
	m_EntOverlay[6].setName(profileName);
	m_EntOverlay[1].setName("HP :");
	m_EntOverlay[3].setName("DMG:");
	m_EntOverlay[5].setName("OWner:");

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
		m_EntOverlay[9].setName("Units");
		m_EntOverlay[10].setName("Buildings");
		m_EntOverlay[11].setName('\0');
		switch (u_EntOnOverlay.getTarget())
		{
		case NOTHING:
			m_EntOverlay[8].setTint(MAGENTA); m_EntOverlay[8].setClickable(false);
			m_EntOverlay[9].setTint(WHITE);  
			if (u_EntOnOverlay.getOwner() == 0) { m_EntOverlay[9].setClickable(true); }
			else { m_EntOverlay[9].setClickable(false); }
			m_EntOverlay[10].setTint(WHITE); 
			if(u_EntOnOverlay.getOwner() == 0) { m_EntOverlay[10].setClickable(true); }
			else { m_EntOverlay[10].setClickable(false); }
			break;
		case UNITS:
			m_EntOverlay[8].setTint(WHITE); 
			if (u_EntOnOverlay.getOwner() == 0) { m_EntOverlay[8].setClickable(true); }
			else { m_EntOverlay[8].setClickable(false); }
			m_EntOverlay[9].setTint(MAGENTA), m_EntOverlay[9].setClickable(false);
			m_EntOverlay[10].setTint(WHITE); 
			if (u_EntOnOverlay.getOwner() == 0) { m_EntOverlay[10].setClickable(true); }
			else { m_EntOverlay[10].setClickable(false); }
			break;
		case BUILDINGS:
			m_EntOverlay[8].setTint(WHITE);
			if (u_EntOnOverlay.getOwner() == 0) { m_EntOverlay[8].setClickable(true); }
			else { m_EntOverlay[8].setClickable(false); }
			m_EntOverlay[9].setTint(WHITE); 
			if (u_EntOnOverlay.getOwner() == 0) { m_EntOverlay[9].setClickable(true); }
			else { m_EntOverlay[9].setClickable(false); }
			m_EntOverlay[10].setTint(MAGENTA), m_EntOverlay[10].setClickable(false);
			break;
		}
		switch (drawMenu(m_EntOverlay))
		{
		case 8:
			for (int a = 0; a < unitSpawnIndex; a++) { if (u_Current.getSelected() && u_Current.getOwner() == 0) { u_Current.setTarget(NOTHING); u_Current.setTargetStatus(false); } }
			break;
		case 9:
			for (int a = 0; a < unitSpawnIndex; a++) { if (u_Current.getSelected() && u_Current.getOwner() == 0) { u_Current.setTarget(UNITS); u_Current.setTargetStatus(false); } }
			break;
		case 10:
			for (int a = 0; a < unitSpawnIndex; a++) { if (u_Current.getSelected() && u_Current.getOwner() == 0) { u_Current.setTarget(BUILDINGS); u_Current.setTargetStatus(false); } }
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
			break;
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
			break;
		default:
			drawTexture(r_EntOnOverlay.getSprite(), xSpace(50), ySpace(11), pd.w, pd.h, 0, true, r_EntOnOverlay.getSpriteIndex());
		}
		drawMenu(m_EntOverlay);
	}
	else
	{
		int COST = 0;
		int MONEY = p_Player.getMoney();

		if (MONEY > 9999) { MONEY = 9999; }
		if (m_PlyrOverlay[7].isButtonHighlighted()) { COST = b_HumanBarracks.getCost(); }
		else if (m_PlyrOverlay[8].isButtonHighlighted()) { COST = b_HumanTower.getCost(); }

		char money[5] = { itc(getThousandsPlace(MONEY)), itc(getHundredsPlace(MONEY)), itc(getTensPlace(MONEY)), itc(MONEY % 10), '\0' };
		char hp[4] = { itc(getHundredsPlace(COST)), itc(getTensPlace(COST)), itc(COST % 10), '\0' };
		
		m_PlyrOverlay[10].setName(hp);
		m_PlyrOverlay[1].setName(money);

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