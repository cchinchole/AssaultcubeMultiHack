#include "stdafx.h"
#include "Math.h"
#include "Offsets.h"
#include "Entity.h"


extern playerent* localPlayer;

float getDistance(vec to, vec from)
{
    return (float)
            (sqrt(
                    powf(to.x - from.x, 2.0) +
                    powf(to.y - from.y, 2.0) +
                    powf(to.z - from.z, 2.0)
            ));

}

bool IsVisible(vec from, vec to, float* tracer)
{
    DWORD traceLine = Offsets::TraceLine;
    traceresult_s traceresult;
    traceresult.collided = false;
    from.z -= 0.2f; //weaponBelowEye
    __asm
    {
    push 0; bSkipTags   //try with 1
    push 0; bCheckPlayers
    push localPlayer
    push to.z
    push to.y
    push to.x
    push from.z
    push from.y
    push from.x
    lea eax, [traceresult]
    call traceLine;
    add esp, 36
    }
    return !traceresult.collided;
};