#pragma once
#include "Psybc5Engine.h"

class Item
{
protected:
	Psybc5Engine* pEngine;

public:
	Item(Psybc5Engine* pEngine, int iconId, std::string name, std::string desc);
	~Item();

	//Function that is executed when the Item is used
	virtual void virtItemUse();
	virtual void virtItemExamine();
	virtual void virtItemDrop();

	int itemId, iconId;
	std::string name, desc;

};

