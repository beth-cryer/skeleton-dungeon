#include "header.h"
#include "EnemyCultist.h"

EnemyCultist::EnemyCultist(BaseEngine* pEngine, Room* room, int xStart, int yStart)
	: EnemyObject(xStart, yStart, pEngine, room, TILE_SIZE, TILE_SIZE, true,
		std::make_shared<Bow>((GameEngine*)pEngine), "Cultist", "A mage that wields occult magicks", 12, 3, 1, 3, 25, 1,
		"sfx/monsters/Monster2.ogg", "sfx/combat/fst_conc_solid_run_01.wav")
{
	imgSprites = ImageManager::loadImage("sprites/chars/cultist.png", true);
}

EnemyCultist::~EnemyCultist()
{

}

void EnemyCultist::virtDraw()
{
	//Handles the animation for each enemy state
	switch (currentState) {
	case (CharState::stateIdle): animate(4, 64, 90, 0, 0, 0, 32); break;
	case(CharState::stateWalk): animate(11, 64, 90, 0, 90, 0, 32); break;
	case(CharState::stateAttack): animate(6, 64, 90, 0, 270, 0, 32); break;
	case(CharState::stateDeath): animate(7, 64, 90, 256, 180, 0, 32); break;
	}

	return AnimatedObject::virtDraw();

}