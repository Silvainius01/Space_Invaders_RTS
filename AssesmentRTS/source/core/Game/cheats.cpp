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
	else if (!strcmp(cheat, "Ill never beat the Witcher"))
	{
		p_Player.addMoney(500);
	}
	else if (!strcmp(cheat, "You should add Battle Axes"))
	{
		p_Player.addSteel(500);
	}
	else if (!strcmp(cheat, "Mushroom and Swiss please"))
	{
		p_Player.addFodd(500);
	}
	else if (!strcmp(cheat, "You made Skynet"))
	{
		cout << "AI is now in maximum overdrive!\n";
	}
	else if (!strcmp(cheat, "Im gonna lose"))
	{
		for (int a = 0; a < buildSpawnIndex; a++)
		{
			if (b_Current == b_InvaderTC) { b_Current.setHP(0); }
		}
	}
	else if (!strcmp(cheat, "I hate matricies"))
	{
		rotation += 90;
		for (int a = 0; a < unitSpawnIndex; a++)	 { u_Current.setPosDim({ u_Current.getX, u_Current.getY, u_Current.getW, u_Current.getH }); }
		for (int a = 0; a < buildSpawnIndex; a++)	 { b_Current.setPosDim({ b_Current.getX, b_Current.getY, b_Current.getW, b_Current.getH }); }
		for (int a = 0; a < resourceSpawnIndex; a++) { r_Current.setPosDim({ r_Current.getX, r_Current.getY, r_Current.getW, r_Current.getH }); }
		for (int a = 0; a < unitIndex; a++)			 { u_AllBase[a].setPosDim({ u_AllBase[a].getX, u_AllBase[a].getY, u_AllBase[a].getW, u_AllBase[a].getH }); }
		for (int a = 0; a < buildingIndex; a++)		 { b_AllBase[a].setPosDim({ b_AllBase[a].getX, b_AllBase[a].getY, b_AllBase[a].getW, b_AllBase[a].getH }); }
		for (int a = 0; a < resourceIndex; a++)		 { r_AllBase[a].setPosDim({ r_AllBase[a].getX, r_AllBase[a].getY, r_AllBase[a].getW, r_AllBase[a].getH }); }
	}
	else
	{
		cout << "Not a cheat.\n";
	}
}