#include "../ent/ent.h"
#include "../ui/ui.h"
#include "game.h"

void initGame()
{
	for (int a = 0; a < buildSpawnIndex; a++) { b_Current = b_Empty; }
	for (int a = 0; a < unitSpawnIndex; a++) { u_Current = u_Empty; }
	for (int a = 0; a < bulletSpawnIndex; a++) { blt_Current = blt_Empty; }
	for (int a = 0; a < playerIndex; a++) { p_Current.addMoney(-(p_Current.getMoney())); }

	spawnBuild(b_HumanTower, xSpace(5), ySpace(80) + (hi_Tower / 2));
	spawnBuild(b_HumanTower, xSpace(15), ySpace(85));
	spawnBuild(b_HumanTower, xSpace(20) - (wi_Tower  / 2), ySpace(95));
	spawnBuild(b_HumanBarracks, xSpace(5), ySpace(88) - (hi_Barracks / 2));
	spawnBuild(b_HumanBarracks, xSpace(15) - (wi_Barracks / 2), ySpace(95));
	spawnBuild(b_HumanTC, xSpace(5), ySpace(95));
	spawnBuild(b_InvaderTower, xSpace(95), ySpace(40) - (hi_Tower / 2));
	spawnBuild(b_InvaderTower, xSpace(85), ySpace(35));
	spawnBuild(b_InvaderTower, xSpace(80) + (wi_Tower / 2), ySpace(25));
	spawnBuild(b_InvaderBarracks, xSpace(85) + (wi_Barracks / 2), ySpace(25));
	spawnBuild(b_InvaderBarracks, xSpace(95), ySpace(32) + (hi_Barracks / 2));
	spawnBuild(b_InvaderTC, xSpace(95), ySpace(25));
}

//Map generation for resources will go here eventually