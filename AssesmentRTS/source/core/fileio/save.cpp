#include "../ui/input.h"
#include "../ui/ui.h"
#include <fstream>
#include <iostream>
#include <cstring>

#define CALC_GRN 0x01000000
#define CALC_BLU 0x00010000
#define CALC_APH 0x00000100
/*
	#define CALC_GRN 0x01 00 00 00
	#define CALC_BLU 0x00 01 00 00
	#define CALC_APH 0x00 00 01 00
*/

#define PROFILE_LIST_READ "_pl.prof", ios_base::in
#define PROFILE_LIST_WRITE "_pl.prof", ios_base::app
#define PROFILE_LIST_CREATE "_pl.prof", ios_base::out
using namespace std;

char *profileName =	 "boobtita";
unsigned playerColor = clr_BLUE;
unsigned customColor = playerColor;
Button *profMenu;

void initRGBA()
{
	RGBA[3] =  playerColor % CALC_APH; //Extract Alpha
	RGBA[2] = (playerColor - RGBA[3]) % CALC_BLU; //Extract Blue
	RGBA[1] = (playerColor - (RGBA[3] + RGBA[2])) % CALC_GRN; //Extract Green
	RGBA[0] = (playerColor - (RGBA[3] + RGBA[2] + RGBA[1]));
}

void calcColor()
{
	customColor = 0; 
	for (int a = 0; a < 4; a++) { customColor += RGBA[a]; }
}

char ctn(char num)
{
	switch (num)
	{
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	case '0':  return 0;
	}
	return -1;
}

void saveProfile()
{
	//This works, reads perfectly
	bool profExists = false;
	bool tc = false;
	fstream save;
	save.open(PROFILE_LIST_READ);
	if (save.is_open())
	{
		char pn[30] = {'\0'};// pn[0] = '0';
		int debug = 0;
		string prof = profileName;
		string comp;
		TRY_CREATE:
		while (!save.eof())
		{
			debug++;
			save.getline(pn, 30);
			//for (int a = 0; a < 30; a++) { cout << pn[a]; } cout << endl;
			comp = pn;
			if (comp == prof) { profExists = true; break; }
		}
		if (profExists)
		{
			save.close();
			string file = profileName + string(".txt");
			save.open(file, ios_base::out);
			if (save.is_open())
			{
				/*
				 * FORMAT:
				 *		NAME
				 *		COLOR
				 */

				save << profileName << " " << playerColor;
				cout << "Succesfully saved your profile\n";
				save.close();
			}
		}
		else
		{
			save.close();
			save.open(PROFILE_LIST_WRITE);
			if (save.is_open())
			{
				save << profileName << endl;
				cout << "Succesfully added " << profileName << " to existing profiles.\n";
			}
			else
				cout << "ERROR: Failed to add profile to profile list" << endl;
		}
	}
	else
	{
		save.open(PROFILE_LIST_CREATE);
		if (save.is_open())
		{
			save << profileName << endl;
			cout << "Succesfully created profile list.\n";
			cout << "Succesfully added " << profileName << " to existing profiles.\n";
			save.close();
		}
		else
			cout << "ERROR: Failed to create profile list" << endl;
	}

	if (save.is_open()) save.close();
}

void finalizePM(int maxIndex)
{

	fstream proflist;
	profMenu = new Button[maxIndex];
	maxIndex = 2;

	profMenu[0] = Button("PROFILES", xSpace(50), ySpace(80), 3, 3, clr_WHITE, CENTER, false);
	profMenu[1] = Button("BACK", xSpace(10), ySpace(70), 3, 3);

	char pn[30] = {0};
	proflist.open(PROFILE_LIST_READ);
	if (proflist.is_open())
	{
		float  x = 50, y = 60;
		while (!proflist.eof())
		{
			proflist.getline(pn, 30);
			//for (int a = 0; a < 30; a++) { cout << pn[a]; } cout << endl;
			profMenu[maxIndex] = Button(pn, xSpace(x), ySpace(y), 3, 3);
			y -= 10;
			maxIndex++;
		}

		profMenu[maxIndex-1] = Button();
		proflist.close();
	}

	if (proflist.is_open()) proflist.close();
}

int makeProfMenu()
{
	int place = 2;

	//Count number of profiles first
	char pn[30] = { 0 };
	fstream proflist;
	proflist.open(PROFILE_LIST_READ);
	if (proflist.is_open())
	{
		while (!proflist.eof())
		{
			proflist.getline(pn, 30);
			//for (int a = 0; a < 30; a++) { cout << pn[a]; } cout << endl << endl;
			place++;
		}
		proflist.close();
	}
	else { cout << "ERROR: Could not find profile list. Aborting."; return -1; }

	finalizePM(place);

	return 0;
}

void loadProfile(const char* _prof)
{
	fstream load;
	string prof = _prof + string(".txt");
	int place = 0;

	load.open(prof);
	if (load.is_open())
	{
		int a = 0;
		unsigned clr = 0;
		char name[30] = {};
		char color[10] = {};

		{
			char data[41] = {};
			load.getline(data, 41);
			for (int b = 0; b < 41; b++) { cout << data[b]; } cout << endl << endl;
			for (; a < 30 && data[a] != ' '; a++) { name[a] = data[a]; cout << data[a]; } cout << " A: " << a << endl; a += 1;
			for (int b = 0; a < 41 && data[a] != '\0'; a++, b++) { color[b] = data[a]; cout << data[a]; } cout << " A: " << a << endl;
		}

		int place = -1;
		int b = -1;
		for (int A = 9; A > -1; A--)
		{
			b = ctn(color[A]);

			if (b == -1) { continue; }
			else { place++; } //Which is why place starts at -1 ;)

			if (place == 0) { clr = b; }
			else { clr += (b * (int)std::pow(10, place)); }
		}

		profileName = (char*)_prof;
		playerColor = clr;
		load.close();
	}
	else { cout << "ERROR: Could not locate file '" << prof << "'." << endl; }
}