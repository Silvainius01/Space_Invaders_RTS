#pragma once
#include "../libraries/sfwdraw.h"

class Player
{
private:
	int money = 0;
	unsigned color;
public:
	int getMoney();
	unsigned getPlayerColor();

	void addMoney(int MONEY);
	void setPlayerColor(unsigned tint);

	Player(void)
	{
		addMoney(0);
		color = WHITE;
	}
};
const int playerIndex = 3;
extern Player p_AllBase[playerIndex];
#define p_Player p_AllBase[0]
#define p_AI p_AllBase[1]
#define p_Current p_AllBase[a]

/* In Genereate.cpp */

extern void initGame();

/* In AI.cpp */

extern void ai_Run(float updateTime = 2.0f);
extern void aih_Run(float updateTime = 2.0f);

/* In cheats.cpp */

extern void cheatChecker(char cheat[30]);