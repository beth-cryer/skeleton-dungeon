#pragma once

class Item
{

public:

	Item();
	~Item();

	//Function that is executed when the Item is used
	virtual void virtItemUse();
	virtual void virtItemExamine();
	virtual void virtItemDrop();

	int itemId, iconId;
	std::string name, desc;
	enum itemType { weapon, armour, consumable };

};

