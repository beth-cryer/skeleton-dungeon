#include "header.h"
#include "EnemyZombieObject.h"

EnemyZombieObject::EnemyZombieObject(BaseEngine* pEngine)
	: EnemyObject(320, 256, pEngine, 63, 73, true,
		"Zombie", "A shambling corpse, reanimated", 10, 2, 10)
{
	imgSprites = ImageManager::loadImage("sprites/chars/zombie.png", true);
}

EnemyZombieObject::~EnemyZombieObject()
{

}

void EnemyZombieObject::virtDraw()
{
	//Handles the animation for each enemy state
	switch (currentState) {
	case (stateIdle): animate(5, 63, 76, 0, 152); break;
	case(stateWalk): animate(11, 63, 76, 0, 76); break;
	//case(stateAttack): animate(4, 80, 76, 0, 304); break;
	}

	return AnimatedObject::virtDraw();

}