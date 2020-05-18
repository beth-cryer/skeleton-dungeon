#pragma once
#include "Item.h"

class Consumable :
	public Item, public std::enable_shared_from_this<Consumable>
{
public:
	Consumable(GameEngine* pEngine, int iconId, std::string name, std::string desc);
	~Consumable();

	void virtItemUse() override; //consume
};


class LesserHealthPotion :
	public Consumable
{
public:
	LesserHealthPotion(GameEngine* pEngine) : Consumable(pEngine, 20, "Lesser Healing Potion", "Restores 25 percent of your HP.") {}

	void virtItemUse() override;

};