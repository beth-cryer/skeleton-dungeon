#include "header.h"
#include "EnemyElf.h"

EnemyElf::EnemyElf(BaseEngine* pEngine, Room* room, int xStart, int yStart)
	: EnemyObject(xStart, yStart, pEngine, room, TILE_SIZE, TILE_SIZE, true,
		std::make_shared<Bow>((GameEngine*)pEngine), "Elf", "A corrupted elvish imp that wields a bow and arrow", 8, 3, 1, 2, 30, 1,
		NULL, "sfx/combat/fst_conc_solid_run_01.wav")
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
	case (CharState::stateIdle): animate(6, 64, 83, 0, 0, 0, 8); break;
	case(CharState::stateWalk): animate(9, 64, 83, 0, 83, 0, 8); break;
	case(CharState::stateAttack): animate(9, 64, 83, 0, 166, 0, 8); break;
	case(CharState::stateDeath): animate(5, 64, 83, 0, 664, 0, 8, false); break;
	}

	return AnimatedObject::virtDraw();

}