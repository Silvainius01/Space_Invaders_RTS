#pragma once

extern unsigned playerColor;
extern char *profileName;
extern Button* profMenu;

extern void saveProfile();
void loadProfile(const char* _prof);
extern int makeProfMenu();
