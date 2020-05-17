#pragma once
#include "DisplayableObject.h"
#include "ImageManager.h"
#include "FloorManager.h"

class GameEngine;

class DoorObject :
	public DisplayableObject
{
public:
	DoorObject(BaseEngine* pEngine, Room* pRoom, int doorType, int tileID, int xStart, int yStart)
		: DisplayableObject(xStart, yStart, pEngine, TILE_SIZE, TILE_SIZE, true),
		pRoom(pRoom), doorType(doorType), tileID(tileID), pEngine((GameEngine*)pEngine)
	{
		setShouldDeleteOnRemoval(false);
	}

	~DoorObject() {}

	void virtDraw() override;
	void virtDoUpdate(int iCurrentTime) override;
	void virtMouseDown(int iButton, int iX, int iY) override;

	int getXPos() { return m_iCurrentScreenX; }
	int getYPos() { return m_iCurrentScreenY; }

	int doorType; //Determines which direction to go when you use the door

private:
	SimpleImage tileSprites = ImageManager::loadImage("sprites/tiles.png", true);
	bool locked = false; //(door locked until enemies killed)

	//Room we are contained in (used to get pointers to adjacent rooms)
	Room* pRoom;

	GameEngine* pEngine;

	int tileID; // id of the door tile to draw. add 10 to get the locked-off version

};

