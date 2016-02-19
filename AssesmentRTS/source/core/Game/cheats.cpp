#include "game.h"
#include "../ent/ent.h"
#include <iostream>
#include <cstring>
using namespace std;

void cheatChecker(char *cheat)
{
	cout << cheat << "; ";

	if (!strcmp(cheat, "Hotdog"))
	{
		cout << "Summoned Hotdog\n";
		spawnUnit(u_Hotdog, xSpace(50), ySpace(50));
	}
	else if (!strcmp(cheat, "Are there orcs in LOTR"))
	{
		cout << "Yes.\n";
	}
	else if (!strcmp(cheat, "meow"))
	{
		cout << "Ate all units\n";
		for (int a = 0; a < unitSpawnIndex; a++) { u_Current.setHP(0); }
	}
	else
	{
		cout << "Not a cheat.\n";
	}
}