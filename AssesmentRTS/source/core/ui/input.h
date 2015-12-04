#pragma once
#include "../libraries/sfwdraw.h"

#define KEY_B 66
#define KEY_C 67
#define KEY_H 72
#define KEY_I 73
#define KEY_T 84
#define KEY_LSHIFT 340
#define KEY_RCTRL 345
#define KEY_LCTRL 341

#define SHF_B 1001
#define SHF_C 1002
#define SHF_T 1003

const int bindIndex = 9;
extern int bindings[bindIndex];

extern int checkBindings(bool retShf = false, bool carry = false);
extern int checkKeys();
extern int checkMouse(bool doesLeftCarry = true);
