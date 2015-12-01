
#include "../ent/ent.h"
#include "../ui/ui.h"

void initGame()
{
	spawnBuild(b_HumanTower, xSpace(5), ySpace(80));
	spawnBuild(b_HumanTower, xSpace(20), ySpace(80));
	spawnBuild(b_HumanTower, xSpace(20), ySpace(95));
	spawnBuild(b_HumanBarracks, xSpace(18.5), ySpace(90));
	spawnBuild(b_HumanBarracks, xSpace(10.5), ySpace(81));
	spawnBuild(b_HumanTC, xSpace(7), ySpace(90));

	spawnBuild(b_InvaderTower, xSpace(95), ySpace(40));
	spawnBuild(b_InvaderTower, xSpace(80), ySpace(40));
	spawnBuild(b_InvaderTower, xSpace(80), ySpace(25));
	spawnBuild(b_InvaderBarracks, xSpace(81.5), ySpace(30));
	spawnBuild(b_InvaderBarracks, xSpace(89.5), ySpace(38));
	spawnBuild(b_InvaderTC, xSpace(93), ySpace(30));
}

//Map generation for resources will go here eventually