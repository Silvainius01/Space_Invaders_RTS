#include "./input.h"
#include "./ui.h"
#include "../ent/ent.h"
#include "../math.h"
#include "../fileio/fileio.h"
#include <iostream>
using namespace std;

#define u_EntOnOverlay u_AllDynam[entOnOverlay]
#define b_EntOnOverlay b_AllDynam[entOnOverlay]

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


Button m_EntOverlay[] = { Button("Default", xSpace(50), ySpace(7.5), 2, 2, WHITE, CENTER, false),
							Button("HP :", xSpace(5), ySpace(14), 2, 2, WHITE, RIGHT, false), Button("NV", m_EntOverlay[1].getXPos() + (strlen(m_EntOverlay[1].getName()) * 9 * 3), m_EntOverlay[1].getYPos(), m_EntOverlay[1].getHeight(), m_EntOverlay[1].getWidth(), WHITE, RIGHT, false),
							Button("DMG:", xSpace(5), ySpace(11.5), 2, 2, WHITE, RIGHT, false), Button("NV", m_EntOverlay[3].getXPos() + (strlen(m_EntOverlay[3].getName()) * 9 * 3), m_EntOverlay[3].getYPos(), m_EntOverlay[3].getHeight(), m_EntOverlay[3].getWidth(), WHITE, RIGHT, false),
							Button("Owner:", xSpace(5), ySpace(9), 2, 2, WHITE, RIGHT, false), Button(profileName, m_EntOverlay[5].getXPos(), m_EntOverlay[5].getYPos() - ySpace(2.5), m_EntOverlay[5].getHeight(), m_EntOverlay[5].getWidth(), playerColor, RIGHT, false),
							Button("Target:", xSpace(63), ySpace(14), 2, 2, WHITE, RIGHT, false),
								Button("Nothing", xSpace(63), ySpace(11.5), 2, 2, MAGENTA, RIGHT, false),
								Button("Units",xSpace(63), ySpace(9), 2, 2, WHITE, RIGHT),
								Button("Buildings", xSpace(63), ySpace(6.5), 2, 2, WHITE, RIGHT),
							Button() };

void drawGameOverlay()
{
	drawTexture(ui_Overlay, xSpace(50), ySpace(50), xSpace(1, 1), ySpace(1, 1));
	m_EntOverlay[6].setTint(playerColor);
	m_EntOverlay[6].setName(profileName);

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
		if (u_EntOnOverlay == u_Invader) { m_EntOverlay[6].setName("Invaders"); m_EntOverlay[6].setTint(RED); }
		else { m_EntOverlay[6].setTint(playerColor); m_EntOverlay[6].setName(profileName); }
		m_EntOverlay[7].setName("Target:");
		m_EntOverlay[8].setName("Nothing");
		m_EntOverlay[9].setName("Units");
		m_EntOverlay[10].setName("Buildings");
		switch (u_EntOnOverlay.getTarget())
		{
		case NOTHING:
			m_EntOverlay[8].setTint(MAGENTA), m_EntOverlay[8].setClickable(false);
			m_EntOverlay[9].setTint(WHITE), m_EntOverlay[9].setClickable(true);
			m_EntOverlay[10].setTint(WHITE), m_EntOverlay[10].setClickable(true);
			break;
		case UNITS:
			m_EntOverlay[8].setTint(WHITE), m_EntOverlay[8].setClickable(true);
			m_EntOverlay[9].setTint(MAGENTA), m_EntOverlay[9].setClickable(false);
			m_EntOverlay[10].setTint(WHITE), m_EntOverlay[10].setClickable(true);
			break;
		case BUILDINGS:
			m_EntOverlay[8].setTint(WHITE), m_EntOverlay[8].setClickable(true);
			m_EntOverlay[9].setTint(WHITE), m_EntOverlay[9].setClickable(true);
			m_EntOverlay[10].setTint(MAGENTA), m_EntOverlay[10].setClickable(false);
			break;
		}
		switch (drawMenu(m_EntOverlay))
		{
		case 8:
			for (int a = 0; a < unitSpawnIndex; a++) { if (u_Current.getSelected()) { u_Current.setTarget(NOTHING); u_Current.setTargetStatus(false); } }
			break;
		case 9:
			for (int a = 0; a < unitSpawnIndex; a++) { if (u_Current.getSelected()) { u_Current.setTarget(UNITS); u_Current.setTargetStatus(false); } }
			break;
		case 10:
			for (int a = 0; a < unitSpawnIndex; a++) { if (u_Current.getSelected()) { u_Current.setTarget(BUILDINGS); u_Current.setTargetStatus(false); } }
			break;
		}
	}
	else if (isBuildOnOverlay)
	{
		int HP = b_EntOnOverlay.getHP();
		int DMG = b_EntOnOverlay.getDMG();
		char hp[4] = { itc(getHundredsPlace(HP)), itc(getTensPlace(HP)), itc(HP % 10), '\0' };
		char dmg[4] = { itc(getHundredsPlace(DMG)), itc(getTensPlace(DMG)), itc(DMG % 10), '\0' };
		PosDim pd = b_EntOnOverlay.getPosDim();
		drawTexture(b_EntOnOverlay.getSprite(), xSpace(50), ySpace(11), pd.w, pd.h, 0, true, b_AllDynam[entOnOverlay].getSpriteIndex());
		m_EntOverlay[0].setName(b_EntOnOverlay.getName());
		m_EntOverlay[2].setName(hp);
		if (b_EntOnOverlay == b_HumanTower || b_EntOnOverlay == b_InvaderTower) { m_EntOverlay[4].setName(dmg); }
		else { m_EntOverlay[4].setName("NV"); }
		if (b_EntOnOverlay == b_InvaderTC || b_EntOnOverlay == b_InvaderBarracks || b_EntOnOverlay == b_InvaderTower) { m_EntOverlay[6].setName("Invaders"); m_EntOverlay[6].setTint(RED); }
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
			m_EntOverlay[10].setName(cnt);
		}
		else { m_EntOverlay[7].setName('\0'); }
		//m_EntOverlay[10].setName();  //Counts the amount in queue and portrays the number.
		switch (drawMenu(m_EntOverlay))
		{
		case 8:
			if (b_EntOnOverlay == b_HumanBarracks)
			{
				addToQueue(u_Human, b_EntOnOverlay);
			}
			else if (b_EntOnOverlay == b_InvaderBarracks)
			{
				addToQueue(u_Invader, b_EntOnOverlay);
			}
			break;
		default:
			break;
		}
	}
}