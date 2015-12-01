#include "./libraries/sfwdraw.h"
#include "./ui/ui.h"
#include "./ent/ent.h"
#include <iostream>
#include "./ui/input.h"
#include "fileio\fileio.h"
#include "Game\game.h"
using namespace std;
using namespace sfw;

bool clickedR = false;
bool clickedL = false;
bool isUserInMenu = true;
bool first = true;
float x, y;

void debugGameMode()
{
	if (first)
	{
		initGame();
		first = false;
	}

	/*int key = checkBindings();*/
	int mouse = checkMouse();

	/*if (key == KEY_I) { spawnUnit(u_Invader); }
	if (key == KEY_H)  { spawnUnit(u_Human); }
	
	if (key == KEY_B) { spawnBuild(b_HumanBarracks); }
	if (key == KEY_T) { spawnBuild(b_HumanTower); }
	if (key == KEY_C) { spawnBuild(b_HumanTC); }

	if (key == SHF_B) { spawnBuild(b_InvaderBarracks); }
	if (key == SHF_T) { spawnBuild(b_InvaderTower); }
	if (key == SHF_C) { spawnBuild(b_InvaderTC); }
	
	if (key == KEY_BACKSPACE) { killUnits(); killBuild(); }*/
	
	if (mouse == MOUSE_BUTTON_LEFT)
	{
		if (!clickedL) { x = getMouseX(); y = getMouseY(); clickedL = true; }
		if (y <= ySpace(20)) { clickedL = false; }
		drawBox(x, y, -(y - getMouseY()), -(x - getMouseX()), MAGENTA);
	}
	else
	{
		if (clickedL)
		{
			float selbox[4];
			selbox[0] = x; selbox[1] = y; selbox[2] = -(y - getMouseY()); selbox[3] = -(x - getMouseX());
			selectEnts(selbox);
		}
		clickedL = false;
	}

	if (mouse == MOUSE_BUTTON_RIGHT)
	{
		moveSelectedUnits();
	}

	drawGameOverlay();
}

void debugMenuMode()
{
	switch (m_CurrentMenu)
	{
	case MAIN:
		switch (drawMenu(m_Main))
		{
		case 1: m_CurrentMenu = PLAY; break;
		case 2: m_CurrentMenu = OPTION; break;
		default: break;
		}
		break;
	case PLAY:
		switch (drawMenu(m_Play))
		{
		case 1: isUserInMenu = false;
		case 2: m_CurrentMenu = DEFAULT; break;
		default: break;
		}
		break;
	case OPTION:
		switch (drawMenu(m_Optn))
		{
		case 1: m_CurrentMenu = OPT_PROFILE; break;
		case 2: m_CurrentMenu = OPT_CONTROLS; break;
		case 3: m_CurrentMenu = OPT_SCREEN; break;
		case 4: m_CurrentMenu = OPT_COLOR; break;
		case 5: m_CurrentMenu = MAIN; break;
		default: break;
		}
		break;
	case OPT_CONTROLS:
		switch (drawMenu(m_Ctrl))
		{
		case 1: m_CurrentMenu = OPT_CTRL_MOVE; break;
		case 2: m_CurrentMenu = OPT_CTRL_HOTKEY; break;
		case 3: m_CurrentMenu = OPTION; break;
		default: break;
		}
		break;
	case OPT_SCREEN:
		switch (drawMenu(m_Scrn))
		{
		case 1: case 2: m_CurrentMenu = DEFAULT; break;
		case 3: m_CurrentMenu = OPTION; break;
		default: break;
		}
		break;
	case OPT_COLOR:
		switch (drawMenu(m_Clr))
		{
		case 1: playerColor = BLUE; break;
		case 2: playerColor = RED; break;
		case 3: playerColor = GREEN; break;
		case 4: playerColor = CYAN; break;
		case 5: playerColor = MAGENTA; break;
		case 6: playerColor = YELLOW; break;
		case 7: playerColor = BLACK; break;
		case 8: playerColor = WHITE; break;
		case 9: m_CurrentMenu = OPTION; break;
		default: drawSelBox(m_Clr[getSelectedColor()], BLACK); break;
		}
		break;
	case OPT_CTRL_MOVE:
		break;
	case OPT_CTRL_HOTKEY:
		break;
	default:
		switch (drawMenu(m_Dflt))
		{
		case 1: m_CurrentMenu = MAIN; break;
		default: break;
		}
	}
}

void main()
{
	initContext(SCREEN[0], SCREEN[1], "RTS");
	initUI();
	initEnts();

	float tempTimer = 0.0f;

	while (stepContext())
	{
		if (isUserInMenu) { debugMenuMode(); }
		else 
		{ 
			debugGameMode(); updateEnts(); ai_Run();
		}

		drawMouse();
	}
}