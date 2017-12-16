// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <iostream>
#include <detours.h>
#include <gl\gl.h>
#include <strsafe.h>
#include "Draw.h";
#include "Offsets.h"
#include "ProcessMemoryWriter.h";
#include "Entity.h"
#include "Esp.h"
#include "Aimbot.h"
#pragma comment( lib, "Ws2_32.lib" )
#pragma comment( lib, "detours.lib" )
#pragma comment(lib, "user32.lib")

Draw drawClass;
MemoryWriter memoryWriter;
int version = 2;
int EntArrPtr;
int* pCount = NULL;
int width = NULL;
int height = NULL;
bool isMenu = false;
int smoothbot = 14;
int scalebox = 10;
extern bool isRage;
extern bool isEsp;
extern bool isTrigger;
extern bool isAimbot;
extern void DrawMenu();
extern void KeyMenu();
extern int oldID;
extern vec oldAngle;
Player pList[32];
playerent* localPlayer;
bool isRunning = true;

void ReadVariables()
{
	pCount = (int*)Offsets::PlayerCount;
	width = *(DWORD*)Offsets::SCRWidth;
	height = *(DWORD*)Offsets::SCHeight;
	localPlayer = (playerent*)*(int*)(Offsets::ACBase);
	EntArrPtr = *(int*)Offsets::EntityArray;
	for (int i = 0; i < (*pCount); i++)
	{
		pList[i].setID(i);
		pList[i].setPlayerEnt((playerent*)*(int*)(EntArrPtr + 0x4 * i));
        if(i == oldID)
        {
            if(pList[i].getPlayerEnt())
                if(pList[i].getPlayerEnt()->state == CS_DEAD) {
                    oldID = -1;
                    localPlayer->vViewAngle = oldAngle;
                }
        }
	}
}

using namespace std;

typedef BOOL(WINAPI * twglSwapBuffers) (_In_ HDC hDc);
//Original
twglSwapBuffers owglSwapBuffers;
//Hooked

BOOL __stdcall hwglSwapBuffers(_In_ HDC hDc)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (isEsp)
		DoEsp();

	if (isMenu)
		DrawMenu();

	return owglSwapBuffers(hDc);
}


HMODULE hMod;
void HookSwapBuffers()
{
	hMod = GetModuleHandle("opengl32.dll");

	if (hMod)
	{
		owglSwapBuffers = (twglSwapBuffers)(DWORD)GetProcAddress(hMod, "wglSwapBuffers");
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID &)owglSwapBuffers, hwglSwapBuffers);
		DetourTransactionCommit();
	}
	HDC currentHDC = wglGetCurrentDC();
	if (!drawClass.bFontBuilt || currentHDC != drawClass.fontHDC)
	{
		drawClass.BuildFonts();
	}
}

DWORD WINAPI readvars(__in LPVOID lpParam)
{
	while (isRunning)
	{
		ReadVariables();
		Sleep(10);
	}
	return 0;
}
DWORD WINAPI trigger(__in LPVOID lpParam)
{
	while (isRunning)
	{
		if (isTrigger)
		{
			if (*(DWORD*)Offsets::CrosshairName > 0)
			{
				/* This trigger is best with single shot rifles*/
				playerent* a = (playerent*)(Offsets::CrosshairName);
				if (!checkTeam(localPlayer, a))
				{
					localPlayer->bAttacking = 1;
					*(DWORD*)Offsets::CrosshairName = 0;
				}
			}
			else
				localPlayer->bAttacking = 0;
		}
		Sleep(10);
	}
	return 0;
}
extern void setFunctionToggle(string check);
DWORD WINAPI aimloop(__in LPVOID lpParam)
{

	while (isRunning)
	{
		if (isAimbot)
		{
			if (isRage)
			{
				if (!isTrigger) {
					isTrigger = true;
					setFunctionToggle("Trigger");
				}
			}
			doAimbot();
		}
		Sleep(10);
	}
	return 0;
}

DWORD WINAPI mainloop(__in LPVOID lpParam)
{
	
	while (isRunning)
	{	
		KeyMenu();
		Sleep(100);
	}
	return 0;
}

extern "C" __declspec(dllexport) void start_thread()
{
	DWORD MainThreadID;
	HANDLE MainThread;

	MainThread = CreateThread(NULL, 0, mainloop, 0, 0, &MainThreadID);
	if (!(MainThread == NULL))
		CloseHandle(MainThread);

}
DWORD healthab;
BOOL WINAPI DllMain(HMODULE hModule, DWORD  reason, LPVOID)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		MessageBoxA(NULL, "Loaded!", "Loaded!", MB_OK);
		memoryWriter.setupWindowHandles("AssaultCube");
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HookSwapBuffers, 0, 0, 0);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)readvars, 0, 0, 0);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)trigger, 0, 0, 0);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)aimloop, 0, 0, 0);
		start_thread();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		memoryWriter.closeHandles();
		isRunning = false;
		break;
	}
	return TRUE;
}

