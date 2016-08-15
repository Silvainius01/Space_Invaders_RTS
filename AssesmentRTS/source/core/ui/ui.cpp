#include "ui.h"
#include "../ent/ent.h"
#include <iostream>
using namespace std;

bool isUnitOnOverlay = false;
bool isBuildOnOverlay = false;
bool isResourceOnOverlay = false;

int overrideMouse = -1;
int entOnOverlay = 0;

float buildGrid[gridY][gridX];
float SCREEN[2] = { 995, 995 };

unsigned ui_Text;
unsigned ui_Human;
unsigned ui_Invader;
unsigned ui_TownCenter;
unsigned ui_Tower;
unsigned ui_Barracks;
unsigned ui_Overlay;
unsigned ui_Mouse;
unsigned ui_Background;
unsigned ui_Hotdog;
unsigned ui_Resource;
unsigned ui_Collector;
unsigned ui_Farm;
unsigned mouseTint = clr_CYAN;
unsigned RGBA[4] = { 0x00, 0x00, 0x00, 0xFF };

float xSpace(float num, float den) { return (SCREEN[0] / den) * num; }
float ySpace(float num, float den) { return (SCREEN[1] / den) * num; }

void initUI()
{
	ui_Text = loadTextureMap("./source/assets/text.png", 13, 5);
	ui_Human = loadTextureMap("./source/assets/human.png", 2);
	ui_Invader = loadTextureMap("./source/assets/invader.png", 4, 2);
	ui_TownCenter = loadTextureMap("./source/assets/town_center.png", 2, 2);
	ui_Tower = loadTextureMap("./source/assets/Tower.png", 2, 2);
	ui_Barracks = loadTextureMap("./source/assets/barracks.png", 2);
	ui_Overlay = loadTextureMap("./source/assets/overlay.png");
	ui_Background = loadTextureMap("./source/assets/background.png");
	ui_Hotdog = loadTextureMap("./source/assets/hotdog.png", 2);
	ui_Resource = loadTextureMap("./source/assets/resources.png", 1, 2);
	ui_Collector = loadTextureMap("./source/assets/collector.png", 2);
}

void initBuildGrid()
{
	for (int y = 0; y < gridY; y++)
	{
		for (int x = 0; x < gridX; x++)
		{
			buildGrid[y][x] = x;
		}
	}
	
}

char itc(int a)
{
	switch (a)
	{
	case 1: return '1';
	case 2: return '2';
	case 3: return '3';
	case 4: return '4';
	case 5: return '5';
	case 6: return '6';
	case 7: return '7';
	case 8: return '8';
	case 9: return '9';
	case 0: return '0';
	}
}

char ktc(int index, bool shift)
{
	switch (index)
	{
	case 0:
		if (shift) { return 'A'; }
		return 'a';
	case 1:
		if (shift) { return 'B'; }
		return 'b';
	case 2:
		if (shift) { return 'C'; }
		return 'c';
	case 3:
		if (shift) { return 'D'; }
		return 'd';
	case 4:
		if (shift) { return 'E'; }
		return 'e';
	case 5:
		if (shift) { return 'F'; }
		return 'f';
	case 6:
		if (shift) { return 'G'; }
		return 'g';
	case 7:
		if (shift) { return 'H'; }
		return 'h';
	case 8:
		if (shift) { return 'I'; }
		return 'i';
	case 9:
		if (shift) { return 'J'; }
		return 'j';
	case 10:
		if (shift) { return 'K'; }
		return 'k';
	case 11:
		if (shift) { return 'L'; }
		return 'l';
	case 12:
		if (shift) { return 'M'; }
		return 'm';
	case 13:
		if (shift) { return 'N'; }
		return 'n';
	case 14:
		if (shift) { return 'O'; }
		return 'o';
	case 15:
		if (shift) { return 'P'; }
		return 'p';
	case 16:
		if (shift) { return 'Q'; }
		return 'q';
	case 17:
		if (shift) { return 'R'; }
		return 'r';
	case 18:
		if (shift) { return 'S'; }
		return 's';
	case 19:
		if (shift) { return 'T'; }
		return 't';
	case 20:
		if (shift) { return 'U'; }
		return 'u';
	case 21:
		if (shift) { return 'V'; }
		return 'v';
	case 22:
		if (shift) { return 'W'; }
		return 'w';
	case 23:
		if (shift) { return 'X'; }
		return 'x';
	case 24:
		if (shift) { return 'Y'; }
		return 'y';
	case 25:
		if (shift) { return 'Z'; }
		return 'z';
	case 26:return '0';
	case 27:return '1';
	case 28:return '2';
	case 29:return '3';
	case 30:return '4';
	case 31:return '5';
	case 32:return '6';
	case 33:return '7';
	case 34:return '8';
	case 35:return '9';
	case 47:return ' ';
	default: return '\0';
	}
}
int cti(char a)
{
	switch (a)
	{
	case 'A': return 0;
	case 'B': return 1;
	case 'C': return 2;
	case 'D': return 3;
	case 'E': return 4;
	case 'F': return 5;
	case 'G': return 6;
	case 'H': return 7;
	case 'I': return 8;
	case 'J': return 9;
	case 'K': return 10;
	case 'L': return 11;
	case 'M': return 12;
	case 'N': return 13;
	case 'O': return 14;
	case 'P': return 15;
	case 'Q': return 16;
	case 'R': return 17;
	case 'S': return 18;
	case 'T': return 19;
	case 'U': return 20;
	case 'V': return 21;
	case 'W': return 22;
	case 'X': return 23;
	case 'Y': return 24;
	case 'Z': return 25;
	case 'a': return 26;
	case 'b': return 27;
	case 'c': return 28;
	case 'd': return 29;
	case 'e': return 30;
	case 'f': return 31;
	case 'g': return 32;
	case 'h': return 33;
	case 'i': return 34;
	case 'j': return 35;
	case 'k': return 36;
	case 'l': return 37;
	case 'm': return 38;
	case 'n': return 39;
	case 'o': return 40; 
	case 'p': return 41;  
	case 'q': return 42;
	case 'r': return 43;
	case 's': return 44;
	case 't': return 45;
	case 'u': return 46;
	case 'v': return 47;
	case 'w': return 48; 
	case 'x': return 49; 
	case 'y': return 50; 
	case 'z': return 51;
	case '1': return 52;
	case '2': return 53;
	case '3': return 54;
	case '4': return 55;
	case '5': return 56;
	case '6': return 57;
	case '7': return 58;
	case '8': return 59;
	case '9': return 60;
	case '0': return 61;
	case ':': return 62;
	case '\n': return -2;
	case '\0': return -3;
	default: return -1;
	}
}
float centerLine(const char *string, int startIndex, float x, float spc)
{
	int sltc = 1;

	for (int a = startIndex; string[a] != '\n' && string[a] != '\0'; a++)
	{
		sltc = a - startIndex + 1;
	}

	if (startIndex != 0) { spc += 1; }

	sltc *= spc;
	return sltc / 2;
}
float leftLine(const char *string, int startIndex, float x, float spc)
{
	int sltl;

	for (int a = startIndex; string[a] != '/n' && string[a] != '/0'; a++)
	{
		sltl = a - startIndex;
	}

	sltl *= spc;

	return x - sltl;
}
void drawText(const char *string, float x, float y, const int multH, const int multW, bool drawUp, TextAlign ta, unsigned tint)
{
	int CTI;
	int lineCount = -1;
	float spc = (9 * multW);
	float xKeep = x;
	for (int a = 0; a < strlen(string); a++)
	{
		CTI = cti(string[a]);
		if (a != 0) { x += spc; }
		if (CTI == -1) { continue; }
		else if (CTI == -3) { break; }
		else if (CTI == -2 || a == 0)
		{
			int index = a;
			int redux;
			lineCount++;
			if (a != 0) { index = a + 1; }
			switch (ta)
			{
			case CENTER:
				redux = centerLine(string, index, xKeep, spc);
				break;
			case RIGHT:
				redux = 0;
				break;
			case LEFT:
				redux = leftLine(string, index, xKeep, spc);
				break;
			}

			x = xKeep - redux - (spc * lineCount);

			if (a != 0)
			{
				if (drawUp) { y += (9 * multH) + ySpace(1); continue; }
				else { y -= (9 * multH) + ySpace(1); continue; }
			}
		}

		drawTexture(ui_Text, x, y, 9 * multW, 9 * multH, 0, false, CTI, tint);
	}
}

void drawBox(float x, float y, float h, float w, unsigned tint)
{
	drawLine(x, y, x + w, y, tint);
	drawLine(x, y, x, y + h, tint);
	drawLine(x + w, y, x + w, y + h, tint);
	drawLine(x, y + h, x + w, y + h, tint);
}

void drawMouse(unsigned sprite, int index, float h, float w, float multH, float multW)
{
	drawTexture(sprite, getMouseX(), getMouseY(), w * multW, h * multH, 0, true, index, mouseTint); 
}

//Red squares indicate where you cannot build anything
//Blue squares indicate where you can build barracks
//Green squares indicate where you can build towers
// xSpace(buildGrid[y][x] * 10) = x
// ySpace((buildGrid[y][x] + y + 2 - x) * 10) = y
void drawBuildGrid()
{
	// All: -1, 1 == RED
	// Towers: 3 == RED; 0, 2 == GREEN
	// Barracks: 0, 2 == RED; 3 == GREEN;
	int bat[gridY + 1][gridX + 1];

	bat[gridY][gridX] = -2;

	int mgx = gridY;
	int mgy = gridX;

	float mx = getMouseX();
	float my = getMouseY();

	for (int a = 0; a < gridY; a++)
	{
		for (int b = 0; b < gridX; b++)
		{
			bat[a][b] = -1;
		}
	}

	for (int a = 0; a < buildSpawnIndex; a++)
	{
		int x = b_Current.gridPos[1];
		int y = b_Current.gridPos[0];

		if (b_Current == b_HumanTower)
		{
			if (y + 1 < 8)
			{
				drawBox(xSpace(buildGrid[y][x] * 10), ySpace((buildGrid[y + 1][x] + y + 3 - x) * 10), ySpace(10), xSpace(10), clr_GREEN);
				bat[y + 1][x] = 2;
			}
			if (y - 1 > -1)
			{
				drawBox(xSpace(buildGrid[y][x] * 10), ySpace((buildGrid[y - 1][x] + y + 1 - x) * 10), ySpace(10), xSpace(10), clr_GREEN);


				bat[y - 1][x] = 2;
			}
			if (x + 1 < 10)
			{
				drawBox(xSpace(buildGrid[y][x + 1] * 10), ySpace((buildGrid[y][x] + y + 2 - x) * 10), ySpace(10), xSpace(10), clr_GREEN);
				bat[y][x + 1] = 2;
			}
			if (x - 1 > -1)
			{
				drawBox(xSpace(buildGrid[y][x - 1] * 10), ySpace((buildGrid[y][x] + y + 2 - x) * 10), ySpace(10), xSpace(10), clr_GREEN);
				bat[y][x - 1] = 2;
			}
		}
	}

	for (int a = 0; a < buildSpawnIndex; a++)
	{
		int x = b_Current.gridPos[1];
		int y = b_Current.gridPos[0];

		if (b_Current == b_HumanBarracks)
		{
			bat[y][x] = 0;
		}
	}

	for (int a = 0; a < buildSpawnIndex; a++)
	{
		int x = b_Current.gridPos[1];
		int y = b_Current.gridPos[0];

		if (b_Current == b_HumanTower)
		{
			if (bat[y][x] == 0) { bat[y][x] = 1; }
			else { bat[y][x] = 3; }
		}
	}

	for (int a = 0; a < buildSpawnIndex; a++)
	{
		int x = b_Current.gridPos[1];
		int y = b_Current.gridPos[0];

		if (b_Current == b_HumanTC)
		{
			bat[y][x] = 1;
		}
	}

	for (int a = 0; a < gridY; a++)
	{
		for (int b = 0; b < gridX; b++)
		{
			if (bat[a][b] == 0) { drawBox(xSpace(buildGrid[a][b] * 10), ySpace((buildGrid[a][b] + a + 2 - b) * 10), ySpace(10), xSpace(10), clr_GREEN); }
		}
	}

	for (int a = 0; a < gridY; a++)
	{
		for (int b = 0; b < gridX; b++)
		{
			if (bat[a][b] == 3) { drawBox(xSpace(buildGrid[a][b] * 10), ySpace((buildGrid[a][b] + a + 2 - b) * 10), ySpace(10), xSpace(10), clr_BLUE); }
		}
	}

	for (int a = 0; a < gridY; a++)
	{
		for (int b = 0; b < gridX; b++)
		{
			if (bat[a][b] == 1) { drawBox(xSpace(buildGrid[a][b] * 10), ySpace((buildGrid[a][b] + a + 2 - b) * 10), ySpace(10), xSpace(10), clr_RED); }
		}
	}

	for (int a = 0; a < gridY; a++)
	{
		for (int b = 0; b < gridX; b++)
		{
			float x = xSpace(buildGrid[a][b] * 10);
			float y = ySpace((buildGrid[a][b] + a + 2 - b) * 10);
			float h = ySpace(10) + y;
			float w = xSpace(10) + x;

			if (mx > x && mx < w && my > y && my < h)
			{
				mgx = b; mgy = a;
				b = gridX; a = gridY;
			}
		}
	}

	//PRINT bat[mgy][mgx] NL;

	switch (bat[mgy][mgx])
	{
	case -1: case 1:
		mouseTint = clr_RED; break;
	case 0:  case 2:
		if (overrideMouse == 1) { mouseTint = clr_GREEN; }
		else if (overrideMouse == 0) { mouseTint = clr_RED; }
		break;
	case 3:
		if (overrideMouse == 1) { mouseTint = clr_RED; }
		else if (overrideMouse == 0) { mouseTint = clr_GREEN; }
		break;
	default:
		mouseTint = clr_RED;
	}
}