#pragma once
#include "DisplayableObject.h"
#include "ImageManager.h"

class GameEngine;
class Item;

class ItemObject :
	public DisplayableObject
{
public:
	ItemObject(int xStart, int yStart, BaseEngine* pEngine, std::shared_ptr<Item> item)
		: DisplayableObject(xStart, yStart, pEngine, TILE_SIZE, TILE_SIZE, true),
		item(item), pEngine((GameEngine*)pEngine)
	{
		setShouldDeleteOnRemoval(false);
	}

	void virtDraw() override;
	void virtDoUpdate(int iCurrentTime) override;
	void virtMouseDown(int iButton, int iX, int iY) override;

	//Pointer to Item represented by this object
	std::shared_ptr<Item> item;

private:
	SimpleImage invSprites = ImageManager::loadImage("sprites/items.png", true);
	GameEngine* pEngine;

	bool picked_up = false;

};

