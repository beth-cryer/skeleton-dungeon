#include "header.h"
#include "PlayerObject.h"

#include <list>


PlayerObject::PlayerObject(BaseEngine* pEngine, Psybc5TileManager* pTiles)
	: CharObject(64, 64, pEngine, 64, 64, true),
	tileManager(pTiles)
{
	imgSprites = ImageManager::loadImage("sprites/chars/skeleton.png", true);
}

PlayerObject::~PlayerObject()
{
}

//Player is an Image - update position every X frames
void PlayerObject::virtDraw()
{
	//Handles the animation for each player state
	//Set max frames in animation depending on which animation state we're in
	switch (currentState) {
	case (CharState::stateIdle): animate(4, 96, 100, 0, 0, 16, 36); break;
	case(CharState::stateWalk): animate(8, 96, 100, 0, 100, 16, 36); break;
	}

	return AnimatedObject::virtDraw();
}


void PlayerObject::virtDoUpdate(int iCurrentTime)
{
	//Don't do update if invisible or paused
	if (!isVisible() || getEngine()->isPaused())
		return;

	Psybc5Engine* engine = (Psybc5Engine*)getEngine();

	//Check if we have leveled up
	int exp = engine->exp;
	int expNext = engine->expNext;
	if (exp >= expNext) {
		//LEVEL UP!

		//Exp required increases by 4% every level
		engine->expNext += expNext * 1.04;

		//Increase level, automatically increment health, stamina and skillUps
		engine->level++;
		engine->maxHealth++;
		engine->skillUps+=2;
		//Every 3 levels, increase stamina
		if (engine->level % 3 == 0) engine->maxStamina++;
	}

	//REQ 4. (2/2) Keyboard input handled
	switch (currentState) {
	case(CharState::stateIdle):
		if (engine->stamina > 0) {
			if (getEngine()->isKeyPressed(SDLK_UP)) move(0, -64, iCurrentTime, 400);
			if (getEngine()->isKeyPressed(SDLK_DOWN)) move(0, 64, iCurrentTime, 400);
			if (getEngine()->isKeyPressed(SDLK_LEFT)) move(-64, 0, iCurrentTime, 400);
			if (getEngine()->isKeyPressed(SDLK_RIGHT)) move(64, 0, iCurrentTime, 400);
		}
		break;

	case(CharState::stateWalk):
		//Calculate current position based on movement calculator
		objMovement.calculate(iCurrentTime);
		m_iCurrentScreenX = objMovement.getX();
		m_iCurrentScreenY = objMovement.getY();

		//If movement has reached end point, exit walk state
		if (objMovement.hasMovementFinished(iCurrentTime)) {
			currentState = CharState::stateIdle;

			m_iCurrentScreenX = objMovement.getX();
			m_iCurrentScreenY = objMovement.getY();

			//Snap to grid in case we strayed a couple pixels somehow
			m_iCurrentScreenX = std::round(m_iCurrentScreenX / TILE_SIZE) * TILE_SIZE;
			m_iCurrentScreenY = std::round(m_iCurrentScreenY / TILE_SIZE) * TILE_SIZE;

			//Code ran at the end of a move:
		}
		break;

	}

	//Ensure that the objects get redrawn on the display
	redrawDisplay();
}

void PlayerObject::move(int xmove, int ymove, int currentTime, int time)
{
	//Check for collision in the Tile we want to move into
	if (tileManager->isValidTilePosition(m_iCurrentScreenX + xmove, m_iCurrentScreenY + ymove)) {
		int tileX = tileManager->getMapXForScreenX(m_iCurrentScreenX + xmove);
		int tileY = tileManager->getMapYForScreenY(m_iCurrentScreenY + ymove);
		int tileCollide = tileManager->getMapValue(tileX, tileY);

		//Collision with Foreground Tile
		if (tileCollide != 0) return;

	}

	//set flipX accordingly (only change if we switch directions)
	if (xmove > 0) flipX = false; else if (xmove < 0) flipX = true;

	((Psybc5Engine*)getEngine())->audio.playAudio("sfx/combat/fst_conc_solid_run_01.wav", -1, 0);
	((Psybc5Engine*)getEngine())->stamina--;

	CharObject::move(xmove, ymove, currentTime, time);
}

//Check if the position is within a certain radius of tiles from the Player
//(basically checks a box around the player that stretches 'squareSize' tiles from the player in each direction)
bool PlayerObject::adjacentTo(int x, int y, int squareSize) {
	return (x > m_iCurrentScreenX - squareSize*64 &&
		x < m_iCurrentScreenX + squareSize * 64 &&
		y > m_iCurrentScreenY - squareSize * 64 &&
		y < m_iCurrentScreenY + squareSize * 64);
}