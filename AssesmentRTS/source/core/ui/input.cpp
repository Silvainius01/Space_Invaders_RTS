#include "input.h";
#include "ui.h"
#include <iostream>
using namespace std;

const int keyIndex = 62;
/*
A B C D E F G H I J K L M N O P Q R S T U V W X Y Z (0 - 25)
1 2 3 4 5 6 7 8 9 0 (26 - 35)
- = [ ] \ ; ' , . / ` Space(36 - 47)
Left Shift, Right Shift (48, 49)
Left Ctrl, Right Ctrl (50, 51)
U, D, L, R (52 - 55)
Escape, Enter/Return, Tab, Backspace (56 - 59)
*/
int keys[keyIndex] = { 65, KEY_B, KEY_C, 68, KEY_E, 70, 71, KEY_H, KEY_I, 74, 75, 76, 77, 78, KEY_O, KEY_P, 81, KEY_R, KEY_S, KEY_T, KEY_U, 86, 87, 88, 89, 90, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 45, 61, 91, 93, 92, 59, 39, 44, 46, 47, 96, 32, KEY_LSHIFT, KEY_RSHIFT, KEY_LCTRL, KEY_RCTRL, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ESCAPE, KEY_ENTER, KEY_TAB, KEY_BACKSPACE };
bool keyPress[keyIndex];
bool hotPress[keyIndex];

int bindings[bindIndex] = { KEY_U, KEY_M, KEY_BACKSPACE, KEY_B, KEY_C, KEY_T, KEY_RCTRL, KEY_LCTRL, KEY_ENTER, KEY_ESCAPE, KEY_R, KEY_O, KEY_P, KEY_E };
bool bindPress[bindIndex];

const int mouseIndex = 2;
int mouse[mouseIndex] = { MOUSE_BUTTON_LEFT, MOUSE_BUTTON_RIGHT };
bool mouseClick[mouseIndex] = { 0, 0 };

bool startDCT = false;
float tmr_DoubleClick = 0.0f;

void resetBinds()
{
	for (int a = 0; a < bindIndex; a++)
	{
		if (!getKey(bindings[a]) && bindPress[a])
		{
			bindPress[a] = false;
		}
	}
}
int checkBindings(bool retShf, bool carry)
{
	bool shift = false;

	for (int a = 0; a < bindIndex; a++)
	{
		if (getKey(bindings[a]) && !bindPress[a])
		{
			if (!carry) { bindPress[a] = true; }
			return bindings[a];
		}
	}

	resetBinds();
	return -1;
}

void resetMouse()
{
	for (int a = 0; a < mouseIndex; a++)
	{
		if (!getMouseButton(mouse[a]) && mouseClick[a])
		{
			mouseClick[a] = false;
		}
	}
}
int checkMouse(bool doesLeftCarry)
{
	for (int a = 0; a < mouseIndex; a++)
	{
		if (getMouseButton(mouse[a]) && !mouseClick[a])
		{
			mouseClick[a] = true;
			if (mouse[a] == MOUSE_BUTTON_LEFT && doesLeftCarry) { mouseClick[a] = false;
			}
			return mouse[a];
		}
	}

	resetMouse();
	return -1;
}

void resetKeys()
{
	for (int a = 0, b = 0; a < keyIndex; a++)
	{
		if (!getKey(keys[a]))
		{
			if (keyPress[a]) keyPress[a] = false;
			if (hotPress[a]) hotPress[a] = false;
		}
	}
}
int checkKeys()
{
	for (int a = 0; a < keyIndex; a++)
	{
		if (getKey(keys[a]) && !keyPress[a])
		{
			keyPress[a] = true;
			return a;
		}
	}

	resetKeys();
	return -1;
}
int checkHotKeys()
{
	for (int a = 0; a < keyIndex; a++)
	{
		if (getKey(keys[a]) && !hotPress[a])
		{
			hotPress[a] = true;
			return keys[a];
		}
	}

	resetKeys();
	return -1;
}