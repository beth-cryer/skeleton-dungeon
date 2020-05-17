#include "Weapon.h"
#include "ProjectileObject.h"
#include "CharObject.h"
#include "GameEngine.h"
#include "PlayerObject.h"

//WEAPON

Weapon::Weapon(GameEngine* pEngine, int iconId, std::string name, std::string desc, int damage, int range, WepType type, const char* sndAttack)
	: Item(pEngine, iconId, name, desc),
	damage(damage), range(range), type(type), sndAttack(sndAttack)
{

}

Weapon::~Weapon() {}

//Equip item
void Weapon::virtItemUse()
{
	pEngine->GetPlayer()->equipWep(shared_from_this());
}

//Default attack code (eg. melee swing)
void Weapon::attack(CharObject* origin, CharObject* target)
{
}


void Bow::attack(CharObject* origin, CharObject* target)
{
	//pEnemy->damage(damage);
	//pEngine->audio.playAudio(sndAttack, -1, 0);

	//Create Arrow object
	pEngine->appendObjectToArray(new ArrowProjectile(pEngine, origin, target));
}