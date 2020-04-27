#pragma once

class Item
{

public:

	Item(int iconId, std::string name, std::string desc);
	~Item();

	//Function that is executed when the Item is used
	virtual void virtItemUse();
	virtual void virtItemExamine();
	virtual void virtItemDrop();

	int itemId, iconId;
	std::string name, desc;

};

