#pragma once
#include "DisplayableObject.h"
#include "Item.h"

class ItemObject :
	public DisplayableObject
{
public:
	ItemObject(int xStart, int yStart, BaseEngine* pEngine, std::shared_ptr<Item> item)
		: DisplayableObject(xStart, yStart, pEngine, TILE_SIZE, TILE_SIZE, true), item(item)
	{

	}

	void virtDraw() override;
	void virtDoUpdate(int iCurrentTime) override;
	void virtMouseDown(int iButton, int iX, int iY) override;

	std::shared_ptr<Item> item;

};

