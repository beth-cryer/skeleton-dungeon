#pragma once
#include "header.h"
#include "Item.h"

class GameEngine;
class CharObject;

class Weapon :
	public Item, public std::enable_shared_from_this<Weapon>
{
public:
	enum class WepType { phys, mag };

	Weapon(GameEngine* pEngine, int iconId, std::string name, std::string desc, int damage, int range, WepType type, const char* sndAttack);
	~Weapon();

	void virtItemUse() override; //equip
	virtual void attack(CharObject* origin, CharObject* target);

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
	WoodSword(GameEngine* pEngine) : Weapon(pEngine, 1, "Wooden Sword", "A sword of wood.", 2, 2, WepType::phys, "sfx/combat/Slash2.ogg") {}

};

class Bow :
	public Weapon
{
public:
	Bow(GameEngine* pEngine) : Weapon(pEngine, 2, "Bow", "A bent twig with a string attached - deadlier than it looks.", 1, 5, WepType::phys, "sfx/combat/Slash2.ogg") {}
	void attack(CharObject* origin, CharObject* target) override;

};