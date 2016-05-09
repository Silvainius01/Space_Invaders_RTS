#include "game.h"

int		 Player::getMoney()						{ return money; }
int		 Player::getPlayerID()					{ return playerID; }
int		 Player::getSteel()						{ return steel; }
int		 Player::getFood()						{ return food; }
unsigned Player::getPlayerColor()				{ return color; }
void	 Player::addMoney(int MONEY)			{ money += MONEY; }
void	 Player::addSteel(int s)				{ steel += s; }
void	 Player::addFodd(int f)					{ food += f; }
void	 Player::setPlayerID(int id)			{ playerID = id; }
void	 Player::setPlayerColor(unsigned tint)	{ color = tint; }

Player p_AllBase[playerIndex]{ Player(0, clr_BLUE), Player(1, clr_RED), Player() };