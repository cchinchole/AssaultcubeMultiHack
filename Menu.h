#ifndef MENU_H
#define MENU_H
#include <iostream>


using namespace std;

const int menuPages = 2;
string menus[menuPages] = {"Online", "Offline", };
string menuOptions[menuPages][8] = { { "Recoil", "Grenade", "AutoFire", "Esp", "Trigger", "Aimbot", "Rage + Trigger" },
								  { "Rapid Fire", "Unlimited Ammo", "AutoFire", "Esp", "Trigger", "Aimbot", "Rage + Trigger" } };
bool menuOn[menuPages][8] = { 
	0, 0, 0, 0,0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
};
int maxScroll[menuPages] = {6, 1,};

void DrawMenu();
void KeyMenu();

#endif