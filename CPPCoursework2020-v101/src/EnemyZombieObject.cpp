#include "header.h"
#include "EnemyZombieObject.h"

EnemyZombieObject::EnemyZombieObject(BaseEngine* pEngine, int xStart, int yStart)
	: EnemyObject(xStart, yStart, pEngine, TILE_SIZE, TILE_SIZE, true,
		"Zombie", "A shambling corpse, reanimated", 10, 2, 10, 1)
{
	imgSprites = ImageManager::loadImage("sprites/chars/zombie.png", true);
}

EnemyZombieObject::~EnemyZombieObject()
{

}

void EnemyZombieObject::turnStart()
{
	pEngine->GetAudio()->playAudio("sfx/monsters/Monster2.ogg", -1, 0);

	EnemyObject::turnStart();
}

void EnemyZombieObject::virtDraw()
{
	//Handles the animation for each enemy state
	switch (currentState) {
	case (CharState::stateIdle): animate(5, 63, 76, 0, 152, 0, 8); break;
	case(CharState::stateWalk): animate(11, 63, 76, 0, 76, 0, 8); break;
	case(CharState::stateAttack): animate(4, 80, 76, 0, 304, 0, 8); break;
	case(CharState::stateDeath): animate(7, 63, 76, 0, 456, 0, 8); break;
	}

	return AnimatedObject::virtDraw();

}