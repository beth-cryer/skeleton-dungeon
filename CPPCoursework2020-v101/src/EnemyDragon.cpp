#include "header.h"
#include "EnemyDragon.h"

EnemyDragon::EnemyDragon(BaseEngine* pEngine, int xStart, int yStart)
	: EnemyObject(xStart, yStart, pEngine, TILE_SIZE, TILE_SIZE, true,
		std::shared_ptr<Weapon>(new WoodSword((GameEngine*)pEngine)), "Black Dragon", "A black-scaled dragon that sears the flesh from its prey", 30, 4, 200, 2)
{
	imgSprites = ImageManager::loadImage("sprites/chars/dragon.png", true);
}

EnemyDragon::~EnemyDragon()
{

}

void EnemyDragon::turnStart()
{
	pEngine->GetAudio()->playAudio("sfx/monsters/Monster4.ogg", -1, 0);

	EnemyObject::turnStart();
}

void EnemyDragon::virtDraw()
{
	CharObject::virtDraw();

	//Handles the animation for each enemy state
	switch (currentState) {
	case (CharState::stateIdle): animate(13, 82, 140, 0, 0, 12, 64); break;
	case(CharState::stateWalk): animate(3, 200, 140, 600, 140, 80, 64); break;
	case(CharState::stateAttack): animate(5, 250, 140, 0, 560, 80, 70); break;
	case(CharState::stateDeath): animate(4, 200, 140, 0, 980); break;
	}

	AnimatedObject::virtDraw();
}