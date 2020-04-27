#include "Weapon.h"

//WEAPON

Weapon::Weapon(int iconId, std::string name, std::string desc, int damage, int range, WepType type, const char* sndAttack)
	: Item(iconId, name, desc),
	damage(damage), range(range), type(type), sndAttack(sndAttack)
{

}

Weapon::~Weapon() {}

//Equip item
void Weapon::virtItemUse()
{

}

//Default attack code (eg. melee swing)
void Weapon::attack()
{

}