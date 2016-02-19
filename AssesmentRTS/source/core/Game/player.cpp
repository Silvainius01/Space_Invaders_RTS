#include "game.h"

int		Player::getMoney() { return money; }
unsigned Player::getPlayerColor() { return color; }
void	Player::addMoney(int MONEY) { money += MONEY; }
void	Player::setPlayerColor(unsigned tint) { color = tint; }

Player p_AllBase[playerIndex]{ Player(), Player() };