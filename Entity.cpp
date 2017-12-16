#include "stdafx.h"
#include "Entity.h"
#include "Offsets.h"

bool checkTeam(playerent* a, playerent* b)
{
	int * gameMode = (int*)Offsets::GameMode;
	if (*gameMode == 0 || *gameMode == 4 || *gameMode == 5 || *gameMode == 7 || *gameMode == 13 || *gameMode == 11 || *gameMode == 14 || *gameMode == 17 || *gameMode == 16 || *gameMode == 20 || *gameMode == 21) {
		if (a->team == b->team)
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
};