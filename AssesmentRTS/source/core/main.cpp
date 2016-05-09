#include "./libraries/sfwdraw.h"
#include "./ui/ui.h"
#include "./ent/ent.h"
#include <iostream>
#include "./ui/input.h"
#include "fileio\fileio.h"
#include "Game\game.h"
#include <ctime>
using namespace std;
using namespace sfw;

bool clickedR = false;
bool clickedL = false;
bool isUserInMenu = true;
bool printGrid = false;
bool debug = true;
bool first = true;
bool ai = true;
bool aih = false;
bool chatBoxOpen = false;
bool chatShift = false;
int chatPlace = 0;
int chatKey;
float x, y;
float tempTimer = 0.0f;
char chat[30];

int checkEndConds()
{
	bool flagHTC = false;
	bool flagITC = false;

	for (int a = 0; a < buildSpawnIndex; a++)
	{
		if (b_Current == b_HumanTC) { flagHTC = true; }
		if (b_Current == b_InvaderTC) { flagITC = true; }
	}

	if (flagHTC && !flagITC) { m_End[0].setName("You Win"); return 0; }
	if (!flagHTC && flagITC) { return 1; }
	else { return -1; }
}

void debugGameMode()
{
	int mouse = checkMouse();
	bool shift = getKey(KEY_LSHIFT);
	int key = checkBindings();

	if (getKey(KEY_RSHIFT) && !shift) { shift = true; }

	if (debug && !chatBoxOpen)
	{
		switch (key)
		{
		case KEY_B:
			if (shift) { spawnBuild(b_InvaderBarracks); }
			else { spawnBuild(b_HumanBarracks); }
			break;
		case KEY_C:
			if (shift) { spawnBuild(b_InvaderTC); }
			else { spawnBuild(b_HumanTC); }
			break;
		case KEY_E:
			tempTimer = 180.0f;
			break;
		case KEY_M:
			if (shift) { p_AI.addMoney(2000); }
			else 
			{ 
				p_Player.addMoney(2000);
				p_Player.addSteel(2000);
				p_Player.addFodd(2000);
			}
			break;
		case KEY_O:
			if (shift) { spawnUnit(u_InvaderCollector, 1); }
			else { spawnUnit(u_HumanCollector, 0); }
			break;
		case KEY_P:
			if (isUnitOnOverlay) { u_EntOnOverlay.print(); }
			break;
		case KEY_R:
			if (shift) { spawnResource(r_Steel); }
			else { spawnResource(r_Food); }
			break;
		case KEY_T:
			if (shift) { spawnBuild(b_InvaderTower); }
			else { spawnBuild(b_HumanTower); }
			break;
		case KEY_U:
			if (shift) { spawnUnit(u_Invader, 1); }
			else { spawnUnit(u_Human, 0); }
			break;
		case KEY_RCTRL:
			if (shift)
			{
				switch (aih)
				{
				case true:  aih = false; break;
				case false: aih =  true; break;
				}
			}
			else
			{
				switch (ai)
				{
				case true:  ai = false; break;
				case false: ai = true; break;
				}
			}
		break;
		case KEY_LCTRL:
			switch (printGrid)
			{
			case true: printGrid = false; break;
			case false: printGrid = true; break;
			}
			break;
		case KEY_BACKSPACE:
			killUnits(); killBuild(); killResource(); break;		
		}
	}
	
	if (key == KEY_ENTER)
	{
		switch (chatBoxOpen)
		{
		case true: chatBoxOpen = false; break;
		case false: chatBoxOpen = true;
		}
	}

	if (mouse == MOUSE_BUTTON_LEFT)
	{
		switch (overrideMouse)
		{
		case 0:
			if (mouseTint == clr_GREEN && !debug)
			{
				spawnBuild(b_HumanBarracks);
				p_Player.addMoney(-(b_HumanBarracks.getCost()));
				overrideMouse = -1;
				mouseTint = clr_CYAN;
			}
			else if(debug)
			{
				spawnBuild(b_HumanBarracks);
				p_Player.addMoney(-(b_HumanBarracks.getCost()));
				overrideMouse = -1;
				mouseTint = clr_CYAN;
			}
			break;
		case 1:
			if (mouseTint == clr_GREEN && !debug)
			{
				spawnBuild(b_HumanTower);
				p_Player.addMoney(-(b_HumanTower.getCost()));
				overrideMouse = -1;
				mouseTint = clr_CYAN;
			}
			else if (debug)
			{
				spawnBuild(b_HumanTower);
				p_Player.addMoney(-(b_HumanTower.getCost()));
				overrideMouse = -1;
				mouseTint = clr_CYAN;
			}
			break;
		default:
			if (!clickedL) { x = getMouseX(); y = getMouseY(); clickedL = true; }
			if (y <= ySpace(20)) { clickedL = false; }
			drawBox(x, y, -(y - getMouseY()), -(x - getMouseX()), clr_MAGENTA);
			break;
		}
	}
	else
	{
		if (clickedL)
		{
			float selbox[4];
			selbox[0] = x; selbox[1] = y; selbox[2] = -(y - getMouseY()); selbox[3] = -(x - getMouseX());
			selectEnts(selbox);
			clickedL = false;
		}
	}

	if (mouse == MOUSE_BUTTON_RIGHT)
	{
		if (overrideMouse == -1)
		{
			moveSelectedUnits();
		}
		else
		{
			overrideMouse = -1;
			mouseTint = clr_CYAN;
		}
	}

	if (startDCT && tmr_DoubleClick < 0.25f) { tmr_DoubleClick += getDeltaTime(); }
	else { tmr_DoubleClick = 0.0f; startDCT = false;  }

	if (printGrid)
	{
		const int boxSize = 10;

		for (float x = xSpace(0); x / xSpace(5) != 20; x += xSpace(boxSize))
		{
			for (float y = ySpace(20); y / ySpace(5) != 20; y += ySpace(boxSize))
			{
				drawBox(x, y, ySpace(boxSize), xSpace(boxSize), clr_RED);
			}
		}
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
		case 1: playerColor = clr_BLUE; break;
		case 2: playerColor = clr_RED; break;
		case 3: playerColor = clr_BLACK; break;
		case 4: playerColor = clr_CYAN; break;
		case 5: playerColor = clr_MAGENTA; break;
		case 6: playerColor = clr_YELLOW; break;
		case 7: playerColor = clr_GREEN; break;
		case 8: playerColor = clr_WHITE; break;
		case 9: m_CurrentMenu = OPTION; break;
		default: drawSelBox(m_Clr[getSelectedColor()], clr_BLACK); break;
		}
		break;
	/*case OPT_CTRL_MOVE:
		break;
	case OPT_CTRL_HOTKEY:
		break;*/
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
	initBuildGrid();

	srand(time(NULL));

	for (int a = 0; a < 30; a++)
	{
		chat[a] = '\0';
	}

	

	while (stepContext())
	{
		drawTexture(ui_Background, xSpace(00), ySpace(100), xSpace(100), ySpace(82.5), 0, false);

		if (isUserInMenu) { debugMenuMode(); }
		else
		{
			if (first)
			{
				initEnts();
				initBuildGrid();
				initGame();
				first = false;
			}

			switch (checkEndConds())
			{
			case -1: 
				debugGameMode(); 
				updateEnts(); 
				tempTimer += getDeltaTime();
				if (ai) { ai_Run(); } 
				if (aih) { aih_Run();  }
				if (tempTimer >= 180.0f) { rRep(); tempTimer = 0.0f; }
				break;
			default:
				if (drawMenu(m_End) != -1)
				{
					isUserInMenu = true;
					first = true;
					for(int a = 0; a < unitSpawnIndex; a++)
					{
						u_Current = u_Empty;
					}
					unitSpawnIndex = 0; 
					spawnUnit(u_Empty, -1);
					m_CurrentMenu = MAIN;
				}
			}

			switch (chatBoxOpen)
			{
			case true:
				chatKey = checkKeys();
				chatShift = false;
				if (getKey(KEY_LSHIFT) || getKey(KEY_RSHIFT)) { chatShift = true; }
				chat[chatPlace] = ktc(chatKey, chatShift);
				if (chatKey == 59 && chatPlace > 0) { chatPlace--; chat[chatPlace] = '\0'; }
				else if (chat[chatPlace] != '\0' && chatPlace != 30) { chatPlace++; }
				m_ChatBox[0].setName(chat);
				drawMenu(m_ChatBox, -1, true);
				break;
			case false:
				if (chat[0] != '\0')
				{
					cheatChecker(chat);
					chatPlace = 0;
					for (int a = 0; a < 30; a++)
					{
						chat[a] = '\0';
					}
				}
			}
		}

		switch (overrideMouse)
		{
		case 0:
			drawMouse(ui_Barracks, 0, hi_Barracks, wi_Barracks, 1, 1); 
			drawBuildGrid();
			break;
		case 1: 
			drawMouse(ui_Tower, 1, hi_Tower, wi_Tower, 1, 1); 
			drawBuildGrid();
			break;
		default: drawMouse(); break;
		}

		
	}
}