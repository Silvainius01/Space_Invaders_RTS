#pragma once
#include <cmath>

template<typename T>
T frac(const T div, const T num, const T den)
{
	if (den == 0) { return 0; }
	return ((div / den) * num);
}
template<typename T>
T expo(const T base, const T power)
{
	float ans = base;
	if (power >= 2)
	{
		for (int a = 1; a < power; a++) { ans *= base; }
		return ans;
	}
	else if (power == 1) { return ans; }
	else if (power == 0) { return 1; }
	else if (power < 0)
	{
		for (int a = -1; a > power; a--) { ans *= base; }
		return 1 / ans;
	}
}

extern int getTensPlace(int num);

extern int getHundredsPlace(int num);