#include "game.h"

int		Player::getMoney() { return money; }
void	Player::addMoney(int MONEY) { money += MONEY; }

Player p_AllBase[playerIndex]{ Player(), Player() };