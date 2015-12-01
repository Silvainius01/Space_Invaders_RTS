#include "ui.h"
#include <iostream>
using namespace std;

bool isUnitOnOverlay = false;
bool isBuildOnOverlay = false;

int entOnOverlay = 0;

float SCREEN[2] = { 995, 995 };

unsigned ui_Text;
unsigned ui_Human;
unsigned ui_Invader;
unsigned ui_TownCenter;
unsigned ui_Tower;
unsigned ui_Barracks;
unsigned ui_Overlay;

float xSpace(float num, float den) { return (SCREEN[0] / den) * num; }
float ySpace(float num, float den) { return (SCREEN[1] / den) * num; }

void initUI()
{
	ui_Text = loadTextureMap("./source/assets/text.png", 13, 5);
	ui_Human = loadTextureMap("./source/assets/human.png");
	ui_Invader = loadTextureMap("./source/assets/invader.png", 2, 2);
	ui_TownCenter = loadTextureMap("./source/assets/town_center.png", 1, 2);
	ui_Tower = loadTextureMap("./source/assets/Tower.png", 2);
	ui_Barracks = loadTextureMap("./source/assets/barracks.png");
	ui_Overlay = loadTextureMap("./source/assets/overlay.png");
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

void drawMouse() { drawTexture(ui_Text, getMouseX(), getMouseY(), wi_Text * 3, hi_Text * 3, 0, true, 49, CYAN); }