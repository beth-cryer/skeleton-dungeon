#include "header.h"
#include "EnemyZombie.h"

EnemyZombie::EnemyZombie(BaseEngine* pEngine, Room* room, int xStart, int yStart)
	: EnemyObject(xStart, yStart, pEngine, room, TILE_SIZE, TILE_SIZE, true,
		std::shared_ptr<Weapon>(new WoodSword((GameEngine*)pEngine)), "Zombie", "A shambling corpse, reanimated", 10, 2, 4, 1, 10, 1)
{
	imgSprites = ImageManager::loadImage("sprites/chars/zombie.png", true);
}

EnemyZombie::~EnemyZombie()
{

}

void EnemyZombie::turnStart()
{
	pEngine->GetAudio()->playAudio("sfx/monsters/Monster2.ogg", -1, 0);

	EnemyObject::turnStart();
}

void EnemyZombie::virtDraw()
{
	//Handles the animation for each enemy state
	switch (currentState) {
	case (CharState::stateIdle): animate(5, 63, 76, 0, 152, 0, 16); break;
	case(CharState::stateWalk): animate(11, 63, 76, 0, 76, 0, 16); break;
	case(CharState::stateAttack): animate(4, 80, 76, 0, 304, 0, 16); break;
	case(CharState::stateDeath): animate(7, 63, 76, 0, 456, 0, 16); break;
	}

	return AnimatedObject::virtDraw();

}