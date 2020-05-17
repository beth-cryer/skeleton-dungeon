#pragma once
#include "DisplayableObject.h"
#include "ImageManager.h"
#include "FloorManager.h"

class DoorObject :
	public DisplayableObject
{
public:
	DoorObject(BaseEngine* pEngine, Room* pRoom, int doorType, int tileID, int xStart, int yStart)
		: DisplayableObject(xStart, yStart, pEngine, TILE_SIZE, TILE_SIZE, true),
		pRoom(pRoom), doorType(doorType), tileID(tileID)
	{

	}

	~DoorObject() {}

	void virtDraw() override;
	void virtDoUpdate(int iCurrentTime) override;
	void virtMouseDown(int iButton, int iX, int iY) override;

private:
	SimpleImage tileSprites = ImageManager::loadImage("sprites/tiles.png", true);
	bool locked = false; //(door locked until enemies killed)

	//Room we are contained in (used to get pointers to adjacent rooms)
	Room* pRoom;

	int doorType; //Determines which direction to go when you use the door
	int tileID; // id of the door tile to draw. add 10 to get the locked-off version

};

