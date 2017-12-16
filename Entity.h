#include "Math.h"
enum State{
	CS_ALIVE = 0,
	CS_DEAD,
};

class weapon {
public:
	char _0x0000[4];
	BYTE ID; //0x0004
	char _0x0005[59];
};

class playerent
{
public:
	char _0x0000[4];
	vec vLocationHead; //0x0004
	char _0x0010[36];
	vec vLocation; //0x0034
	vec vViewAngle; //0x0040
	char _0x004C[37];
	BYTE bScoping; //0x0071
	char _0x0072[134];
	__int32 health; //0x00F8
	__int32 armor; //0x00FC
	char _0x0100[292];
	BYTE bAttacking; //0x0224
	char name[16]; //0x0225
	char _0x0235[247];
	BYTE team; //0x032C
	char _0x032D[11];
	BYTE state; //0x0338
	char _0x0339[59];
	weapon* currentWeapon; //0x0374
	char _0x0378[520];
};

class Player {
	private:
		int id;
		playerent* play;
		vec angles;
	public:
		vec getAngles()
		{
			return angles;
		}
		playerent* getPlayerEnt()
		{
			return play;
		}
		int getID()
		{
			return id;
		}
		void setAngles(vec angle)
		{
			angles = angle;
		}
		void setID(int num)
		{
			id = num;
		}
		void setPlayerEnt(playerent* playerent)
		{
			play = playerent;
		}
};



bool checkTeam(playerent* a, playerent* b);