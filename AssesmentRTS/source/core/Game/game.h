#pragma once

class Player
{
private:
	int money = 0;
public:
	int getMoney();

	void addMoney(int MONEY);

	Player(void)
	{
		addMoney(0);
	}
};
const int playerIndex = 2;
extern Player p_AllBase[playerIndex ];
#define p_Player p_AllBase[0]
#define p_AI p_AllBase[1]
#define p_Current p_AllBase[a]

/* In Genereate.cpp */

extern void initGame();

/* In AI.cpp */

extern float ai_TimerStart;

//Run the ai
extern void ai_Run(float updateTime = 2.0f);