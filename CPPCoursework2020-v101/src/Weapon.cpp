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
	//pEngine->equipItem(this);
}

//Default attack code (eg. melee swing)
void Weapon::attack()
{
	//pEnemy->damage(damage);
	//pEngine->audio.playAudio(sndAttack, -1, 0);
}