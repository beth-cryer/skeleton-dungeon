#pragma once
#include "DisplayableObject.h"
#include "ImageManager.h"

class GameEngine;
class Room;

class ExitObject :
	public DisplayableObject
{
public:
	ExitObject(BaseEngine* pEngine, Room* pRoom, int xStart, int yStart)
		: DisplayableObject(xStart, yStart, pEngine, TILE_SIZE, TILE_SIZE, true),
		pEngine((GameEngine*)pEngine), pRoom(pRoom)
	{
		setShouldDeleteOnRemoval(false);
	}

	~ExitObject() {}

	void virtDraw() override;
	void virtDoUpdate(int iCurrentTime) override;
	void virtMouseDown(int iButton, int iX, int iY) override;

private:
	SimpleImage img = ImageManager::loadImage("sprites/objects/exit.png", true);
	int locks = 0; //Number of keys needed to open
	bool activate = false;

	GameEngine* pEngine;
	Room* pRoom;
};

