#pragma once

class GameEngine;

class Item
{

public:

	Item(GameEngine* pEngine, int iconId, std::string name, std::string desc);
	~Item();

	//Function that is executed when the Item is used
	virtual void virtItemUse();
	virtual void virtItemExamine();
	virtual void virtItemDrop();

	int itemId, iconId;
	std::string name, desc;

protected:
	GameEngine* pEngine;

};