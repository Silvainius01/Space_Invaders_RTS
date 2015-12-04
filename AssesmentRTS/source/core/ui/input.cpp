#include "input.h";
#include "ui.h"
#include <iostream>
using namespace std;

const int keyIndex = 62;
/*
A B C D E F G H I J K L M N O P Q R S T U V W X Y Z (0 - 25)
1 2 3 4 5 6 7 8 9 0 (26 - 35)
- = [ ] \ ; ' , . / ` Space(36 - 48)
Left Shift, Right Shift (49, 50)
Left Ctrl, Right Ctrl (51, 52)
U, D, L, R (53 - 56)
Escape, Enter/Return, Tab, Backspace (56 - 59)
*/
int keys[keyIndex] = { 65, KEY_B, KEY_C, 68, 69, 70, 71, KEY_H, KEY_I, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, KEY_T, 85, 86, 87, 88, 89, 90, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 45, 61, 91, 93, 92, 59, 39, 44, 46, 47, 96, 32, KEY_LSHIFT, 344, KEY_LCTRL, KEY_RCTRL, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ESCAPE, KEY_ENTER, KEY_TAB, KEY_BACKSPACE };
bool keyPress[keyIndex];

int bindings[bindIndex] = { KEY_LSHIFT, KEY_I, KEY_H, KEY_BACKSPACE, KEY_B, KEY_C, KEY_T, KEY_RCTRL, KEY_LCTRL };
bool bindPress[bindIndex];

const int mouseIndex = 2;
int mouse[mouseIndex] = { MOUSE_BUTTON_LEFT, MOUSE_BUTTON_RIGHT };
bool mouseClick[mouseIndex] = { 0, 0 };


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
			if(shift) 
			{
 				if (bindings[a] == KEY_B) { return SHF_B; }
				if (bindings[a] == KEY_C) { return SHF_C; }
				if (bindings[a] == KEY_T) { return SHF_T; }
			}
			else if (bindings[a] == KEY_LSHIFT) 
			{
				if (retShf) { return bindings[a]; }
				shift = true; 
				bindPress[a] = false;
				continue; 
			}
			else { return bindings[a]; }
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
		if (!getKey(keys[a]) && keyPress[a])
		{
			keyPress[a] = false;
		}
	}
}
int checkKeys()
{
	for (int a = 0; a < keyIndex; a++)
	{
		if (getKey(bindings[a]) && !keyPress[a])
		{
			keyPress[a] = true;
			return bindings[a];
		}
	}

	resetKeys();
	return -1;
}