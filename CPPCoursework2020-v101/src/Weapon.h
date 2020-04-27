#pragma once
#include "header.h"
#include "Item.h"

class Weapon :
	public Item
{
public:

	void virtItemUse() override; //equip
	virtual void attack();

	int damage, range;

	enum class WepType { phys, mag };

	//Targeted enemies check if they are weak to this type
	WepType type;

};

