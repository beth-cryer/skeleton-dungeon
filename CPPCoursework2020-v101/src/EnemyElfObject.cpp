#include "header.h"
#include "EnemyElfObject.h"

EnemyElfObject::EnemyElfObject(BaseEngine* pEngine)
	: EnemyObject(320, 256, pEngine, 64, 64, true,
		"Elf", "A corrupted elvish imp that wields a bow and arrow", 15, 4, 30, 1)
{
	imgSprites = ImageManager::loadImage("sprites/chars/zombie.png", true);
}

EnemyElfObject::~EnemyElfObject()
{

}

void EnemyElfObject::virtDraw()
{
	//Handles the animation for each enemy state
	switch (currentState) {
	case (CharState::stateIdle): animate(6, 63, 83, 0, 0); break;
	case(CharState::stateWalk): animate(9, 63, 83, 0, 83); break;
	case(CharState::stateAttack): animate(3, 63, 83, 332, 0); break;
	case(CharState::stateDeath): animate(5, 63, 83, 664, 0); break;
	}

	return AnimatedObject::virtDraw();

}