#include "stdafx.h"
#include "Mod.h"
#include "Offsets.h"
#include "ProcessMemoryWriter.h"
#include <string>
#include "Entity.h"
extern MemoryWriter memoryWriter;

using namespace std;

DWORD ammoAddress = Offsets::AmmoModifier;
char ammoBinNew[] = "\xFF\x06";
char ammoBinDef[] = "\xFF\x0E";

DWORD autoAddress = Offsets::AutoWeaponModifier;
char autoBinNew[] = "\xEB\x09";
char autoBinDef[] = "\x75\x09";

DWORD rapidAddress = Offsets::RapidFireModifier;
DWORD rapidAddress2 = Offsets::RapidFireModifier2;
char rapidBinNew[] = "\x90\x90";
char rapidBinDef[] = "\x89\x0A";

DWORD grenAddress = Offsets::GrenadeModifier;
char grenBinDef[] = "\xFF\x08";
char grenNewDef[] = "\xFF\x00";

DWORD baseOffsets[] = { 0x30 };
DWORD baseAddress = Offsets::ACBase;
extern playerent* localPlayer;


DWORD recoilAdresses[] = { 0x463781, 0x463783, 0x463786, 0x463787, 0x46378B, 0x46378C, 0x46378E };
std::string recoilBinNew[] = {
	"\x90\x90",
	"\x90\x90\x90",
	"\x90",
	"\x90\x90\x90\x90",
	"\x90",
	"\x90\x90",
	"\x90\x90",
};
std::string recoilBinDef[] = {
	"\x8B\x16",
	"\x8B\x52\x14",
	"\x50",
	"\x8D\x4C\x24\x1C",
	"\x51",
	"\x8B\xCE",
	"\xFF\xD2",
};
int recoilOpBytes[] = {
	2,
	3,
	1,
	4,
	1,
	2,
	2,
};


void toggleAimbot()
{
	isAimbot = !isAimbot;
}

void toggleRecoil()
{
	if (isRecoil)
	{
		for (int i = 0; i < 7; i++)
		{
			memoryWriter.writeMemory(recoilAdresses[i], recoilBinDef[i], recoilOpBytes[i]);
		}
	}
	else
	{
		for (int i = 0; i < 7; i++)
		{
			memoryWriter.writeMemory(recoilAdresses[i], recoilBinNew[i], recoilOpBytes[i]);
		}
	}
	isRecoil = !isRecoil;
}

void toggleGrenade()
{

	if (isGrenade)
		memoryWriter.writeMemory(grenAddress, grenBinDef, 2);
	else
		memoryWriter.writeMemory(grenAddress, grenNewDef, 2);
	isGrenade = !isGrenade;
}

void toggleAuto()
{
	if (!isAutoFire)
		memoryWriter.writeMemory(autoAddress, autoBinNew, 2);
	else
		memoryWriter.writeMemory(autoAddress, autoBinDef, 2);
}

void toggleRapidFire()
{
	if (!isRapid)
	{
		memoryWriter.writeMemory(rapidAddress, rapidBinNew, 2);
		memoryWriter.writeMemory(rapidAddress2, rapidBinNew, 2);
	}
	else
	{
		memoryWriter.writeMemory(rapidAddress, rapidBinDef, 2);
		memoryWriter.writeMemory(rapidAddress2, rapidBinDef, 2);
	}
}

void toggleESP()
{
	isEsp = !isEsp;
}


void toggleRage()
{
	toggleAimbot();
	isRage = !isRage;
    localPlayer->vViewAngle = vec(0, -180, 0);
}

void toggleTrigger()
{
	isTrigger = !isTrigger;
}

void toggleAmmo()
{
	if (!isAmmo)
		memoryWriter.writeMemory(ammoAddress, ammoBinNew, 2);
	else
		memoryWriter.writeMemory(ammoAddress, ammoBinDef, 2);
}