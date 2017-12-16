#include "stdafx.h"
#include "Menu.h"
#include "Draw.h"
#include "glut.h"
#include <string>

extern bool isMenu;
extern int scalebox;
extern Draw drawClass;
extern int version;
extern int height;
extern int width;
float menu_height, menu_width, scroll_width, scroll_height, scroll_distance, scrollX, scrollY;
int scrollIndex, menuIndex;

extern void toggleRapidFire();
extern void toggleRecoil();
extern void toggleGrenade();
extern void toggleAuto();
extern void toggleESP();
extern void toggleTrigger();
extern void toggleRage();
extern void toggleAmmo();
extern void toggleAimbot();

void setFunctionToggle(string check)
{
	for(int i = 0; i < menuPages; i++)
		for (int x = 0; x < maxScroll[i]; x++)
		{
			if (menuOptions[i][x] == (check))
			{
				menuOn[i][x] = !menuOn[i][x];
			}
		}
}

void DrawMenu()
{
	int propX = 1920+675;
	int propY = 1080;

	menu_width = (400*width)/propX;
	menu_height = (800 * height) / propY;
	scroll_width = (320 * width) / propX;
	scroll_height = (40 * height) / propY;
	scroll_distance = (menu_height / ((6.666 * height) / propY));
	scrollX = ((90 * width) / propX);

	drawClass.stringD(((220 * width) / propX), ((70 * height) / propY), 255, 0, 0, GLUT_BITMAP_9_BY_15, "%s%d", "Maverick V", version);
	drawClass.stringD(((220 * width) / propX), ((100 * height) / propY), 255, 255, 255, GLUT_BITMAP_9_BY_15, "%s", menus[menuIndex].c_str());

	for (int i = 0; i <= maxScroll[menuIndex]; i++)
	{
		if (menuOn[menuIndex][i])
		{
			if(scrollIndex == i)
				drawClass.stringD((menu_width / ((3.4 * width) / propX)), (menu_height / ((4.2 * height) / propY)) + (scroll_distance * (i)), 0, 255, 0, GLUT_BITMAP_HELVETICA_18, "%s", menuOptions[menuIndex][i].c_str());
			else
				drawClass.stringD((menu_width / ((3.4 * width) / propX)), (menu_height / ((4.2 * height) / propY)) + (scroll_distance * (i)), 0, 255, 0, GLUT_BITMAP_HELVETICA_12, "%s", menuOptions[menuIndex][i].c_str());
		
		}
		else
		{
			if(scrollIndex == i)		
			drawClass.stringD((menu_width / ((3.4 * width) / propX)), (menu_height / ((4.2 * height) / propY)) + (scroll_distance * (i)), 255, 0, 0, GLUT_BITMAP_HELVETICA_18, "%s", menuOptions[menuIndex][i].c_str());
			else
			drawClass.stringD((menu_width / ((3.4 * width) / propX)), (menu_height / ((4.2 * height) / propY)) + (scroll_distance * (i)), 255, 0, 0, GLUT_BITMAP_HELVETICA_12, "%s", menuOptions[menuIndex][i].c_str());
		}
	}
		scrollY = (height / ((7*(height))/propY) ) + ((scrollIndex)* scroll_distance);


	//Scroll Bar
	//drawClass.box(scrollX, scrollY, scroll_width, scroll_height, 255, 255, 255);
		drawClass.line(1.0f, scrollX, scrollY, scrollX + (15*width)/propX, scrollY+(15*height)/propY, 255, 255, 255);
		drawClass.line(1.0f, scrollX, scrollY, scrollX, scrollY + (30*height)/propY, 255, 255, 255);
	    drawClass.line(1.0f, scrollX, scrollY+(30*height)/propY, scrollX + (15*width)/propX, scrollY + (15*height)/propY, 255, 255, 255);


	//Menu Border
	drawClass.boxLine(((4 * width) / propX), ((50 * width) / propX), ((50 * height) / propY), menu_width, menu_height, 255, 0, 0);

	//Menu Title Center
	drawClass.line(((4 * width) / propX), ((90 * width) / propX), ((80 * height) / propY), (((50 * width) / propX) + menu_width) - ((40 * width) / propX), ((80 * height) / propY), 255, 0, 0);

	//Menu Title Angle
	drawClass.line(((4 * width) / propX), ((50 * width) / propX) + menu_width, (50 * height) / propY, (((50 * width) / propX) + menu_width) - ((40 * width) / propX), (80 * height) / propY, 255, 0, 0);

	//Menu Title Angle
	drawClass.line(((4 * width) / propX), ((50 * width) / propX), ((50 * height) / propY), ((90 * width) / propX), ((80 * height) / propY), 255, 0, 0);


	//Menu Background
	drawClass.box((50 * width) / propX, (50 * height) / propY, menu_width, menu_height, 0, 0, 0, 200);

}

void KeyMenu()
{
	if (GetAsyncKeyState(VK_RSHIFT))
	{
		isMenu = !isMenu;
		if (!isMenu)
		{
			menuIndex = 0;
			scrollIndex = 0;
		}
	}

	if (GetAsyncKeyState(VK_INSERT))
	{
		((void(__thiscall*)(const char* string))0x408E80)(("Aimbot distance level is now : " + std::to_string(scalebox) + " !").c_str());
		scalebox += 2;
	}
		if (GetAsyncKeyState(VK_DELETE)) {
			((void(__thiscall*)(const char* string))0x408E80)(("Aimbot distance level is now : " + std::to_string(scalebox) + " !").c_str());
			scalebox -= 2;
		}
	if (GetAsyncKeyState(VK_CONTROL) && isMenu)
	{
		menuOn[menuIndex][scrollIndex] = !menuOn[menuIndex][scrollIndex];
		switch (menuIndex)
		{
		case 0:
			switch (scrollIndex)
			{
			case 0:
				toggleRecoil();
				break;
			case 1:
				toggleGrenade();
				break;
			case 2:
				toggleAuto();
				break;
			case 3:
				toggleESP();
				break;
			case 4:
				toggleTrigger();
				break;
			case 5:
				toggleAimbot();
				break;
			case 6:
				toggleRage();
				break;
			}
			break;
		case 1:
			switch (scrollIndex)
			{
			case 0:
				toggleRapidFire();
				break;
			case 1:
				toggleAmmo();
				break;
			}
			break;
		}
	}

	if (GetAsyncKeyState(VK_UP))
	{
		scrollIndex--;
		if (scrollIndex < 0)
		{
			scrollIndex = maxScroll[menuIndex];
		}
	}

	if (GetAsyncKeyState(VK_DOWN))
	{

		scrollIndex++;

		if (scrollIndex > maxScroll[menuIndex])
		{
			scrollIndex = 0;
		}
	}
	if (GetAsyncKeyState(VK_LEFT))
	{

		scrollIndex = 0;
		menuIndex--;

		if (menuIndex < 0)
		{
			menuIndex = menuPages-1;
		}
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{

		scrollIndex = 0;
		menuIndex++;

		if (menuIndex > menuPages-1)
		{
			menuIndex = 0;
		}
	}
}