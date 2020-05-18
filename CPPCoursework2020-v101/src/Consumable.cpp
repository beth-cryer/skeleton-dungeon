#include "header.h"
#include "Consumable.h"
#include "GameEngine.h"

Consumable::Consumable(GameEngine* pEngine, int iconId, std::string name, std::string desc)
	: Item(pEngine, iconId, name, desc)
{

}

Consumable::~Consumable() {}

//Consume item
void Consumable::virtItemUse()
{
	//Subclass will do it's own thing, then call this
	pEngine->limitStats(); //(make sure any stat changes haven't put us over the max)
	pEngine->GetAudio()->playAudio("sfx/objects/Heal7.ogg", -1, 0);

	//Delete from inventory
	auto inv = pEngine->GetTilesInv();
	inv->removeItem(shared_from_this());
}


	//CONSUMABLES

//Lesser Health
void LesserHealthPotion::virtItemUse()
{
	pEngine->health += (int)(pEngine->maxHealth * 0.25);

	Consumable::virtItemUse();
}