#pragma once
#include "../libraries/sfwdraw.h"

class Player
{
private:
	int money = 0, steel = 0, food = 0;
	int playerID = -1;
	unsigned color;
public:
	int getMoney();
	int getSteel();
	int getFood();
	int getPlayerID();
	unsigned getPlayerColor();

	void addMoney(int MONEY);
	void addSteel(int s);
	void addFodd(int f);
	void setPlayerID(int id);
	void setPlayerColor(unsigned tint);

	Player(void)
	{
		addMoney(0);
		setPlayerID(-1);
		color = clr_WHITE;
	}
	Player(int id, unsigned color)
	{
		setPlayerID(id);
		setPlayerColor(color);
	}
};
const int playerIndex = 3;
extern Player p_AllBase[playerIndex];
#define p_Player p_AllBase[0]
#define p_AI p_AllBase[1]
#define p_Neutral p_AllBase[2]
#define p_Current p_AllBase[a]

/* In Genereate.cpp */

extern void initGame();
extern void rRep();

/* In AI.cpp */

extern void ai_Run(float updateTime = 2.0f);
extern void aih_Run(float updateTime = 2.0f);

/* In cheats.cpp */

void cheatChecker(char *cheat);