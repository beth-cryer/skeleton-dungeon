#include "header.h"
#include "EnemyDragonObject.h"

EnemyDragonObject::EnemyDragonObject(BaseEngine* pEngine)
	: EnemyObject(320, 256, pEngine, 63, 73, true,
		"Black Dragon", "A black-scaled dragon capable of breathing fire and slashing with its claws", 30, 6, 200)
{
	imgSprites = ImageManager::loadImage("sprites/chars/dragon.png", true);
}

EnemyDragonObject::~EnemyDragonObject()
{

}

void EnemyDragonObject::virtDraw()
{
	//Handles the animation for each enemy state
	switch (currentState) {
	case (CharState::stateIdle): animate(13, 81, 140, 0, 0); break;
	case(CharState::stateWalk): animate(3, 200, 140, 300, 140); break;
	case(CharState::stateAttack): animate(4, 80, 76, 0, 304); break;
	case(CharState::stateDeath): animate(4, 200, 140, 0, 980); break;
	}

	return AnimatedObject::virtDraw();

}