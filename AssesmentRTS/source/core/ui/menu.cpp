#include "./input.h"
#include "./ui.h"
#include "../ent/ent.h"
#include "../math.h"
#include"../fileio/fileio.h"
#include <iostream>
using namespace std;

TextAlign Button::getAllign() { return ta; }
const char* Button::getName() const { return _string; }
float Button::getXPos() { return xPos; }
float Button::getYPos() { return yPos; }
float Button::getHeight() { return height; }
float Button::getWidth() { return width; }
bool  Button::isClickable() { return clickable; }
bool Button::isButtonHighlighted() { return highlighted; }
unsigned Button::getTint() { return tint; }

void Button::setName(char *n) { strcpy_s(_string, n); usesSprite = false; }
void Button::setXPos(float x) { xPos = x; }
void Button::setYPos(float y) { yPos = y; }
void Button::setHeight(float h) { height = h; }
void Button::setWidth(float w) { width = w; }
void Button::setPos(float x, float y) { setXPos(x), setYPos(y); }
void Button::setDim(float h, float w) { setHeight(h), setWidth(w); }
void Button::setPosDim(float x, float y, float h, float w) { setPos(x, y), setDim(h, w); }
void Button::setAllign(TextAlign TA) { ta = TA; }
void Button::setClickable(bool c) { clickable = c; }
void Button::setTint(unsigned color) { tint = color;}
void Button::isHighlighted(bool ih) { highlighted = ih; }

Menu m_CurrentMenu = MAIN;

Button m_Dflt[3] = {Button("No menu\nimplemented", xSpace(50), ySpace(80), 5, 5, clr_WHITE, CENTER, false),
					Button("MAIN MENU", xSpace(50), ySpace(45), 3, 3), Button() };

Button m_Main[5] = {Button("SPACE\nINVADERS\n  RTS", xSpace(50), ySpace(80), 3, 3, clr_WHITE,CENTER, false),
					Button("PLAY", xSpace(50), ySpace(50), 3, 3),
					Button("OPTIONS", xSpace(50), ySpace(40), 3, 3),
					Button("EXIT", xSpace(50), ySpace(30), 3, 3), Button() };

Button m_Play[4] = {Button("PLAY GAME", xSpace(50), ySpace(80), 3, 3, clr_WHITE, CENTER, false),
					Button("NEW", xSpace(50), ySpace(50), 3, 3),
					Button("LOAD", xSpace(50), ySpace(40), 3, 3), Button() };

Button m_Optn[7] = {Button("OPTIONS", xSpace(50), ySpace(80), 3, 3, clr_WHITE, CENTER, false),
					Button("PROFILE", xSpace(50), ySpace(60), 3, 3),
					Button("CONTROLS", xSpace(50), ySpace(50), 3, 3),
					Button("SCREEN RESOLUTION", xSpace(50), ySpace(40), 3, 3),
					Button("COLOR", xSpace(50), ySpace(30), 3, 3),
					Button("BACK", xSpace(50), ySpace(20), 3, 3), Button() };

Button m_Ctrl[5] = {Button("CONTROLS", xSpace(50), ySpace(80), 3, 3, clr_WHITE, CENTER, false),
					Button("MOVEMENT", xSpace(50), ySpace(60), 3, 3),
					Button("HOTKEYS", xSpace(50), ySpace(50), 3, 3),
					Button("BACK", xSpace(50), ySpace(40), 3, 3), Button() };

Button m_Scrn[5] = {Button("SCREEN RESOLUTION", xSpace(50), ySpace(80), 3, 3, clr_WHITE, CENTER, false),
					Button("WIDTH", xSpace(50), ySpace(60), 3, 3),
					Button("HEIGHT", xSpace(50), ySpace(50), 3, 3),
					Button("BACK", xSpace(50), ySpace(40), 3, 3), Button() };

Button m_Clr[12] = {Button("COLOR", xSpace(50), ySpace(80), 3, 3, clr_WHITE, CENTER, false),
					Button("BLUE", xSpace(25), ySpace(60), 3, 3, clr_BLUE),
					Button("RED", xSpace(25), ySpace(50), 3, 3, clr_RED),
					Button("BLACK", xSpace(25), ySpace(40), 3, 3, clr_BLACK),
					Button("CYAN", xSpace(25), ySpace(30), 3, 3, clr_CYAN),
					Button("MAGENTA", xSpace(75), ySpace(60), 3, 3, clr_MAGENTA),
					Button("YELLOW", xSpace(75), ySpace(50), 3, 3, clr_YELLOW),
					Button("GREEN", xSpace(75), ySpace(40), 3, 3, clr_GREEN),
					Button("WHITE", xSpace(75), ySpace(30), 3, 3),
					Button("BACK", xSpace(50), ySpace(10), 3, 3), 
					Button("CUSTOM", xSpace(50), ySpace(20), 3, 3, customColor), Button() };

Button m_End[3] = { Button("You Lost", xSpace(50), ySpace(80), 3, 3, clr_WHITE, CENTER, false),
					Button("Main Menu", xSpace(50), ySpace(60), 3, 3), Button() };

Button m_ChatBox[2] = { Button("\0", xSpace(5), ySpace(70), 3, 3, clr_WHITE, RIGHT, false), Button() };

Button m_Slider[8] = { Button("R", xSpace(35), ySpace(60), 5, 5, clr_RED),
					   Button("G", xSpace(45), ySpace(60), 5, 5, clr_GREEN),
					   Button("B", xSpace(55), ySpace(60), 5, 5, clr_BLUE),
					   Button("A", xSpace(65), ySpace(60), 5, 5, clr_BLACK),
					   Button("COLOR", xSpace(50), ySpace(80), 5, 5, clr_WHITE),
					   Button("APPLY", xSpace(25), ySpace(20), 3, 3, clr_WHITE),
					   Button("BACK", xSpace(65), ySpace(20), 3, 3), Button() };



int drawMenu(Button *b, int indexLimiter, bool constHighlight, bool makeLeftCarry)
{
	int choice = -1;
	int adjustment = 1;
	float mY = getMouseY();
	float mX = getMouseX();
	float height;
	float width;

	for (int a = 0; strcmp(b[a].getName(), "") != 0; a++)
	{
		drawText(b[a].getName(), b[a].getXPos(), b[a].getYPos(), b[a].getHeight(), b[a].getWidth(), false, b[a].getAllign(), b[a].getTint());
		height = 11 * b[a].getHeight();
		width = 9 * b[a].getWidth() * strlen(b[a].getName());

		switch (b[a].getAllign())
		{
		case CENTER:
			width /= 2;
			adjustment = width;
			break;
		case RIGHT:
			adjustment = 0;
			break;
		case LEFT:
			break;
		}

		if (constHighlight) { drawBox(b[a].getXPos() - adjustment, b[a].getYPos() + b[a].getHeight(), -height, width + adjustment, clr_MAGENTA); }

		if (mY <= b[a].getYPos() && mY >= b[a].getYPos() - height)
		{
			if (mX >= b[a].getXPos() - adjustment && mX <= b[a].getXPos() + width)
			{
				if (b[a].isClickable())
				{
					drawBox(b[a].getXPos() - adjustment, b[a].getYPos() + b[a].getHeight(), -height, width + adjustment, clr_MAGENTA);
					b[a].isHighlighted(true);
					if (checkMouse(makeLeftCarry) == MOUSE_BUTTON_LEFT)
					{
						choice = a;
					}
				}
			}
			else { b[a].isHighlighted(false); }
		}
		else { b[a].isHighlighted(false); }

		if (a == indexLimiter) { return choice; }
	}

	return choice;
}

void drawSelBox(Button b, unsigned forceTint)
{
	TextAlign ta = b.getAllign();
	float x = b.getXPos();
	float y = b.getYPos();
	float h = b.getHeight() * 11;
	float w = b.getWidth() * 9 * strlen(b.getName());
	unsigned tint;
	int adjustment;

	if (forceTint != clr_NONE) { tint = forceTint; }
	else { tint = clr_MAGENTA; }

	switch (b.getAllign())
	{
	case CENTER:
		w /= 2;
		adjustment = w;
		break;
	case RIGHT:
		adjustment = 0;
		break;
	case LEFT: break;
	}

	drawBox(x - adjustment, y + h, -h, w, tint);
}

int getSelectedColor()
{
	switch (playerColor)
	{
	case clr_WHITE: return 8; break;
	case clr_BLACK: return 3; break;
	case clr_CYAN: return 4; break;
	case clr_MAGENTA: return 5; break;
	case clr_YELLOW: return 6; break;
	case clr_RED: return 2; break;
	case clr_GREEN: return 7; break;
	case clr_BLUE: return 1; break;
	}
}