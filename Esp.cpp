#include "stdafx.h"
#include "Esp.h"
#include "Math.h"
#include "Offsets.h"
#include "glut.h"
#include "Draw.h"
#include "Entity.h"

extern Draw drawClass;
extern int* pCount;
extern int EntArrPtr;
extern Player pList[32];
extern playerent* localPlayer;
extern float getDistance(vec to, vec from);
extern bool IsVisible(vec from, vec to, float* tracer);

glmatrixf* GetViewMatrix() {
	return (glmatrixf*)0x00501AE8;
};

vec W2S(glmatrixf *mvpmatrix, vec vPlayerLoc)
{
	//GuidedHacking.com : AnomanderRake

	//Matrix-Vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
	vec4 clipCoords;
	clipCoords.x = vPlayerLoc.x*mvpmatrix->v[0] + vPlayerLoc.y*mvpmatrix->v[4] + vPlayerLoc.z*mvpmatrix->v[8] + mvpmatrix->v[12];
	clipCoords.y= vPlayerLoc.x*mvpmatrix->v[1] + vPlayerLoc.y*mvpmatrix->v[5] + vPlayerLoc.z*mvpmatrix->v[9] + mvpmatrix->v[13];
	clipCoords.z = vPlayerLoc.x*mvpmatrix->v[2] + vPlayerLoc.y*mvpmatrix->v[6] + vPlayerLoc.z*mvpmatrix->v[10] + mvpmatrix->v[14];
	clipCoords.w = vPlayerLoc.x*mvpmatrix->v[3] + vPlayerLoc.y*mvpmatrix->v[7] + vPlayerLoc.z*mvpmatrix->v[11] + mvpmatrix->v[15];

	if (clipCoords.w < 0) return vec(-1, -1, -1);

	//perspective division, dividing by clip.W = NDC
	vec normalizedDeviceCoordinates;
	normalizedDeviceCoordinates.x = clipCoords.x / clipCoords.w;
	normalizedDeviceCoordinates.y = clipCoords.y / clipCoords.w;
	normalizedDeviceCoordinates.z = clipCoords.z / clipCoords.w;

	//viewport tranform to screenCooords
	int  viewport[4] = { 0, 0 , *(int*)0x510c94, *(int*)0x510c98 };

	vec playerScreenCoords;
	playerScreenCoords.x = (viewport[2] / 2 * normalizedDeviceCoordinates.x) + (normalizedDeviceCoordinates.x + viewport[2] / 2);
	playerScreenCoords.y = -(viewport[3] / 2 * normalizedDeviceCoordinates.y) + (normalizedDeviceCoordinates.y + viewport[3] / 2);
	playerScreenCoords.z = 0;

	return playerScreenCoords;
};


void RenderESP(playerent* ent, char name[16], int heal, vec DrawPos, float dist, int r, int g, int b)
{
	int width = *(DWORD*)Offsets::SCRWidth;
	int height = *(DWORD*)Offsets::SCHeight;
	drawClass.stringD(DrawPos.x - (width / 2.5) / dist, (DrawPos.y - (height / 2) / dist), 255, 255, 255, GLUT_BITMAP_TIMES_ROMAN_10, "%s", name);
	glmatrixf * glmvpmatrix = (glmatrixf*)(Offsets::GLMatrix);
	vec footPos = W2S(glmvpmatrix, ent->vLocation);
	vec headPosistion = W2S(glmvpmatrix, ent->vLocationHead);
		int heightesp = footPos.y - (headPosistion.y);
		int widthesp = heightesp / 2;
		heightesp *= 1.17;
		widthesp *= 1.17;
		int x = headPosistion.x - widthesp / 2;
		int y = headPosistion.y;
		if (heal > 100)
			heal = 100;
		if (heal < 1)
			heal = 1;
		drawClass.stringD(x, y-15, 255, 255, 255, GLUT_BITMAP_HELVETICA_12, "%s", name);
		int strHeal = (int)heightesp - heal * (int)heightesp / 100;
		drawClass.boxLine(2.0f, x - 12, y - 16, 6, heightesp + 4, 0, 0, 0);
		drawClass.box(x - 10, y + strHeal - 15, 4, heightesp - strHeal + 2, 153, 255, 0);
		drawClass.boxLine(1.0f, x, y-15, widthesp, heightesp, r, g, b);
	    glPopMatrix();
}

void DoEsp()
{
	for (int i = 0; i < (*pCount); i++)
	{
		if (!pList[i].getPlayerEnt()) continue;
		glmatrixf * glmvpmatrix = (glmatrixf*)(Offsets::GLMatrix);
		pList[i].setPlayerEnt( (playerent*)*(int*)(EntArrPtr + (0x4 * i)));
		if (!pList[i].getPlayerEnt() || !localPlayer) continue;
		vec DrawPos;
		if (pList[i].getPlayerEnt()->state != CS_DEAD)
		{
			vec footPos = W2S(glmvpmatrix, pList[i].getPlayerEnt()->vLocation);
			vec headPosistion = W2S(glmvpmatrix, pList[i].getPlayerEnt()->vLocationHead);
			if (footPos.x > 0 && headPosistion.x > 0)
			{
				float dist = getDistance(pList[i].getPlayerEnt()->vLocationHead, localPlayer->vLocationHead);

				float r = 0, g = 0, b = 0;

				if (IsVisible(localPlayer->vLocationHead, pList[i].getPlayerEnt()->vLocationHead))
				{
					r = 0; g = 255; b = 0;
				}
				else {
					r = 255; g = 0; b = 0;
				}

				if (checkTeam(localPlayer, pList[i].getPlayerEnt())) {
					r = 0; g = 0; b = 255;
				}
				RenderESP(pList[i].getPlayerEnt(), pList[i].getPlayerEnt()->name, pList[i].getPlayerEnt()->health, DrawPos, dist, r, g, b);
			}
		}
	}
}
