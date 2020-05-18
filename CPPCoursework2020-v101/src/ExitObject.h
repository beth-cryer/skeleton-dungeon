#pragma once
#include "DisplayableObject.h"
#include "ImageManager.h"

class GameEngine;

class ExitObject :
	public DisplayableObject
{
public:
	ExitObject(BaseEngine* pEngine, int xStart, int yStart)
		: DisplayableObject(xStart, yStart, pEngine, TILE_SIZE, TILE_SIZE, true),
		pEngine((GameEngine*)pEngine)
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

};

