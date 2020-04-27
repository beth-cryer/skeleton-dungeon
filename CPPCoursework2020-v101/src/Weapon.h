#pragma once
#include "header.h"
#include "Item.h"
#include "EnemyObject.h"

class Weapon :
	public Item
{
public:
	enum class WepType { phys, mag };

	Weapon(Psybc5Engine* pEngine, int iconId, std::string name, std::string desc, int damage, int range, WepType type, const char* sndAttack);
	~Weapon() {}

	void virtItemUse() override; //equip
	virtual void attack(EnemyObject* pEnemy);

	int damage, range;

	//Targeted enemies check if they are weak to this type
	WepType type;

protected:
	const char* sndAttack;

};


//I'm just gonna put all my weapons in here for now
//If I were making a proper big game I'd probly store all my item data externally, maybe make a scripting system for different weapon effects. keeping it simple here though

class WoodSword :
	public Weapon
{
public:
	WoodSword(Psybc5Engine* engine) : Weapon(pEngine, 1, "Wooden Sword", "A sword of wood.", 5, 4, WepType::phys, "sfx/combat/Slash2.ogg") {}

};