#include "../ent/ent.h"
#include "../ui/ui.h"
#include "game.h"
#include "../math.h"
#include "../fileio/fileio.h"

void initGame()
{
	//Initialize player stats
	p_Player.setPlayerColor(playerColor);
	if (playerColor == clr_RED) { p_AI.setPlayerColor(clr_CYAN); }
	else { p_AI.setPlayerColor(clr_RED); }
	for (int a = 0; a < playerIndex; a++)
	{
		p_Current.addMoney(-(p_Current.getMoney()));
		p_Current.addSteel(-(p_Current.getSteel()));
		p_Current.addFodd (-(p_Current.getFood ()));
	} 

	//Despawn all entities
	for (int a = 0; a < buildSpawnIndex; a++) { b_Current = b_Empty; }
	for (int a = 0; a < unitSpawnIndex; a++) { u_Current = u_Empty; }
	for (int a = 0; a < bulletSpawnIndex; a++) { blt_Current = blt_Empty; }
	for (int a = 0; a < resourceSpawnIndex; a++) { r_Current = r_Empty; }

	//Place player bases
	spawnBuild(b_HumanTC, xSpace(5), ySpace(95));
	spawnBuild(b_HumanBarracks, xSpace(5), ySpace(88) - (hi_Barracks / 2));
	spawnBuild(b_HumanBarracks, xSpace(15) - (wi_Barracks / 2), ySpace(95));
	spawnBuild(b_HumanTower, xSpace(5), ySpace(80) + (hi_Tower / 2));
	spawnBuild(b_HumanTower, xSpace(15), ySpace(85));
	spawnBuild(b_HumanTower, xSpace(20) - (wi_Tower / 2), ySpace(95));
	spawnBuild(b_InvaderTower, xSpace(95), ySpace(40) - (hi_Tower / 2));
	spawnBuild(b_InvaderTower, xSpace(85), ySpace(35));
	spawnBuild(b_InvaderTower, xSpace(80) + (wi_Tower / 2), ySpace(25));
	spawnBuild(b_InvaderBarracks, xSpace(85) + (wi_Barracks / 2), ySpace(25));
	spawnBuild(b_InvaderBarracks, xSpace(95), ySpace(32) + (hi_Barracks / 2));
	spawnBuild(b_InvaderTC, xSpace(95), ySpace(25));

	//Resource generation
	for (int a = 0; a < 3; a++)
	{
		spawnResource(r_Steel, plusMinus<float>(xSpace(50), xSpace(std::rand() % 11)), plusMinus<float>(ySpace(60), ySpace(std::rand() % 11)));
		spawnResource(r_Food , plusMinus<float>(xSpace(50), xSpace(std::rand() % 11)), plusMinus<float>(ySpace(60), ySpace(std::rand() % 11)));
	}
}

void rRep()
{
	for (int a = 0, b = 0, sc = 0, fc = 0; a < resourceSpawnIndex; a++)
	{
		switch (b)
		{
		case 0:
			if (r_Current == r_Steel) { sc++; }
			else if (r_Current == r_Food) { fc++; }
			if (a == resourceSpawnIndex - 1) { b = 1, a = -1; }
			break;
		default:
			if (sc < 3) { spawnResource(r_Steel, plusMinus<float>(xSpace(50), xSpace(std::rand() % 46)), plusMinus<float>(ySpace(60), ySpace(std::rand() % 31))); sc++; }
			if (fc < 3) { spawnResource(r_Food , plusMinus<float>(xSpace(50), xSpace(std::rand() % 46)), plusMinus<float>(ySpace(60), ySpace(std::rand() % 31))); fc++; }
		}

	}
}

