#include "header.h"
#include "Weapon.h"

//WEAPON

Weapon::Weapon(Psybc5Engine* pEngine, int iconId, std::string name, std::string desc, int damage, int range, WepType type, const char* sndAttack)
	: Item(pEngine, iconId, name, desc),
	damage(damage), range(range), type(type), sndAttack(sndAttack)
{

}

//Equip item
void Weapon::virtItemUse()
{
	pEngine->equipItem(this);
}

//Default attack code (eg. melee swing)
void Weapon::attack(EnemyObject* pEnemy)
{
	pEnemy->damage(damage);
	pEngine->audio.playAudio(sndAttack, -1, 0);
}