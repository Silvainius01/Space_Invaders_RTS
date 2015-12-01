#include "math.h"

int getTensPlace(int num) //1245
{
	num %= 100;
	num -= (num % 10);
	num /= 10;
	return num;
}

int getHundredsPlace(int num)
{
	num %= 1000;
	num -= (num % 100);
	num /= 100;
	return num;
}