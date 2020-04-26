#pragma once
#include "DisplayableObject.h"
#include "MovementPosition.h"
#include "Psybc5TileManager.h"

#include "Psybc5Engine.h"

class FireballObject :
	public DisplayableObject
{

public:
	FireballObject(BaseEngine* pEngine, Psybc5TileManager* pTileManager);
	~FireballObject();

	void setMovement(int iStartTime, int iEndTime, int iCurrentTime, int iStartX, int iStartY, int iEndX, int iEndY);

	void virtDraw() override;
	void virtDoUpdate(int iCurrentTime) override;

private:
	int last_frame = 0;
	int anim_frame = 0;
	SimpleImage imgSprites = ImageManager::loadImage("sprites/fireball.png", true);

	MovementPosition objMovement;
	Psybc5TileManager* tileManager; //reference to main Tile Manager

	int originX = 0, originY = 0, endX = 0, endY = 0, time = 0; //remembers settings from last setMovement call, as the object repeats its path and so has to reset later

};

