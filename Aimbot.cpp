#include "stdafx.h"
#include "Aimbot.h"
#include "Entity.h"
#include <cmath>

extern Player pList[32];
extern playerent* localPlayer;
extern int EntArrPtr;
extern int smoothbot;
extern int* pCount;
extern float getDistance(vec to, vec from);
extern bool IsVisible(vec from, vec to, float* tracer);
extern bool isRage;
extern int scalebox;
int oldID = -1;
vec oldAngle = vec(0,0,0);
Player target;
int oldPCount = -1;



void findNewTarget(int EntArrPtr, int* pCount, bool rage)
{
    target.setPlayerEnt(NULL);
    if(*pCount != oldPCount)
    {
        target.setPlayerEnt(NULL);
        oldPCount = *pCount;
        oldID = -1;
    }
	if (rage)
	{
		for (int i = 0; i < (*pCount); i++)
		{
			if (!pList[i].getPlayerEnt()) continue;
			if (!localPlayer) continue;
			if (target.getPlayerEnt())
				if (!IsVisible(localPlayer->vLocationHead, target.getPlayerEnt()->vLocationHead) || target.getPlayerEnt()->state == State::CS_DEAD || (!IsVisible(localPlayer->vLocationHead, target.getPlayerEnt()->vLocationHead)))
					target.setPlayerEnt(NULL);
			if ((IsVisible(localPlayer->vLocationHead, pList[i].getPlayerEnt()->vLocationHead)) && pList[i].getPlayerEnt()->state == State::CS_ALIVE && !checkTeam(localPlayer, pList[i].getPlayerEnt()))
			{
                float PI = 3.14159265358979F;
                float yaw = -(float) atan2(pList[i].getPlayerEnt()->vLocation.x - localPlayer->vLocation.x,
                                           pList[i].getPlayerEnt()->vLocation.y - localPlayer->vLocation.y) / PI * 180 + 180;
                float pitch = acos((pList[i].getPlayerEnt()->vLocationHead.z - localPlayer->vLocationHead.z) /
                                   getDistance(pList[i].getPlayerEnt()->vLocationHead, localPlayer->vLocationHead)) * -180 / PI + 90;

                vec aimAngles = vec(yaw, pitch, 0);
				if (!target.getPlayerEnt())
                {
                    target.setID(i);
                    target.setPlayerEnt(pList[i].getPlayerEnt());
					target.setAngles(aimAngles);
                }

				if (getDistance(pList[i].getPlayerEnt()->vLocation, localPlayer->vLocation) < getDistance(target.getPlayerEnt()->vLocation, localPlayer->vLocation))
				{
                    target.setID(i);
                    target.setPlayerEnt(pList[i].getPlayerEnt());
                    target.setAngles(aimAngles);
				}
			}

		}
	}
	else
	{
		for (int i = 0; i < (*pCount); i++)
		{
			if (!pList[i].getPlayerEnt()) {
                continue;
            }
            if(!localPlayer)
                continue;

			if(target.getPlayerEnt())
			    if (target.getPlayerEnt()->state == State::CS_DEAD || (!IsVisible(localPlayer->vLocationHead, target.getPlayerEnt()->vLocationHead)))
				    target.setPlayerEnt(NULL);

			if ((IsVisible(localPlayer->vLocationHead, pList[i].getPlayerEnt()->vLocationHead)) && pList[i].getPlayerEnt()->state == State::CS_ALIVE && !checkTeam(localPlayer, pList[i].getPlayerEnt()))
			{

				float PI = 3.14159265358979F;
				float yaw = -(float) atan2(pList[i].getPlayerEnt()->vLocation.x - localPlayer->vLocation.x,
										   pList[i].getPlayerEnt()->vLocation.y - localPlayer->vLocation.y) / PI * 180 + 180;
				float pitch = acos((pList[i].getPlayerEnt()->vLocationHead.z - localPlayer->vLocationHead.z) /
								   getDistance(pList[i].getPlayerEnt()->vLocationHead, localPlayer->vLocationHead)) * -180 / PI + 90;

                vec aimAngles = vec(yaw, pitch, 0);


				if (!target.getPlayerEnt()) {
                    target.setID(i);
                    target.setPlayerEnt(pList[i].getPlayerEnt());
                    target.setAngles(aimAngles);
				}

				if (getDistance(aimAngles, localPlayer->vViewAngle) < getDistance(target.getAngles(), localPlayer->vViewAngle))
				{
                    target.setID(i);
                    target.setPlayerEnt(pList[i].getPlayerEnt());
                    target.setAngles(aimAngles);
				}
			} else{
                oldID = -1;
            }

		}
	}
}



void doAimbot() {
    findNewTarget(EntArrPtr, pCount, isRage);
    if (!isRage && target.getPlayerEnt() != NULL) {
        if (getDistance(target.getAngles(), localPlayer->vViewAngle) < scalebox) {
			if(target.getAngles().x != localPlayer->vViewAngle.x || target.getAngles().y != localPlayer->vViewAngle.y) {
				float yawDiff = target.getAngles().x - localPlayer->vViewAngle.x;
				float pitchDiff = target.getAngles().y - localPlayer->vViewAngle.y;

				float smoothYaw = ((yawDiff) / smoothbot);
				float smoothPitch = ((pitchDiff) / (smoothbot / 2));

				float upYaw = localPlayer->vViewAngle.x + smoothYaw;
				float upPitch = localPlayer->vViewAngle.y + smoothPitch;

				localPlayer->vViewAngle = vec(upYaw, upPitch, 0);
			}

        }
    } else if (isRage && target.getPlayerEnt()) {

		if(target.getID() != oldID) {
			oldID = target.getID();
			oldAngle = localPlayer->vViewAngle;
		}

       // if(oldAngle != vec(0,0,0))
           // setViewAngle(localPlayer, oldAngle);


        if (localPlayer->vViewAngle.x != target.getAngles().x && localPlayer->vViewAngle.y != target.getAngles().y)
		{
			//localPlayer->vViewAngle = target->angles;
			localPlayer->vViewAngle = target.getAngles();
		}

    }
}