#include "header.h"
#include "EnemyElf.h"

EnemyElf::EnemyElf(BaseEngine* pEngine, Room* room, int xStart, int yStart)
	: EnemyObject(xStart, yStart, pEngine, room, TILE_SIZE, TILE_SIZE, true,
		std::make_shared<Bow>((GameEngine*)pEngine), "Elf", "A corrupted elvish imp that wields a bow and arrow", 8, 4, 2, 4, 30, 1,
		NULL, "sfx/combat/fst_conc_solid_run_01.wav")
{
	imgSprites = ImageManager::loadImage("sprites/chars/zombie.png", true);
}

EnemyElf::~EnemyElf()
{

}

void EnemyElf::virtDraw()
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