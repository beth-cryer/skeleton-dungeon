#include "header.h"
#include "EnemyElf.h"

EnemyElf::EnemyElf(BaseEngine* pEngine, Room* room, int xStart, int yStart)
	: EnemyObject(xStart, yStart, pEngine, room, TILE_SIZE, TILE_SIZE, true,
		std::make_shared<Bow>((GameEngine*)pEngine), "Elf", "A corrupted elvish imp that wields a bow and arrow", 8, 3, 1, 3, 20, 1,
		NULL, "sfx/combat/Run1.wav")
{
	imgSprites = ImageManager::loadImage("sprites/chars/elf.png", true);
}

EnemyElf::~EnemyElf()
{

}

void EnemyElf::virtDraw()
{
	//Handles the animation for each enemy state
	switch (currentState) {
	case (CharState::stateIdle): animate(6, 64, 83, 0, 0, 0, 16); break;
	case(CharState::stateWalk): animate(9, 64, 83, 0, 83, 0, 16); break;
	case(CharState::stateAttack): animate(9, 64, 83, 0, 166, 0, 16); break;
	case(CharState::stateDeath): animate(5, 64, 83, 0, 664, 0, 16, false); break;
	}

	return AnimatedObject::virtDraw();

}