#include "header.h"
#include "PlayerObject.h"

#include "EnemyObject.h"

#include <list>


PlayerObject::PlayerObject(BaseEngine* pEngine, std::shared_ptr<Weapon> wep)
	: CharObject(64, 64, pEngine, 64, 64, true, wep)
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
	case(CharState::stateAttack): animate(3, 96, 100, 0, 300, 16, 36); break;
	case(CharState::stateWalk): animate(8, 96, 100, 0, 100, 16, 36); break;
	case(CharState::stateDeath): animate(6, 96, 100, 0, 500, 16, 36); break;
	}

	return AnimatedObject::virtDraw();
}


void PlayerObject::virtDoUpdate(int iCurrentTime)
{
	//Don't do update if invisible or paused
	if (!isVisible() || getEngine()->isPaused())
		return;

	//Check if we have leveled up
	int exp = pEngine->exp;
	int expNext = pEngine->expNext;
	if (exp >= expNext) {
		//LEVEL UP!

		//Exp required increases by 4% every level
		pEngine->expNext += expNext * 1.04;

		//Increase level, automatically increment health, stamina and skillUps
		pEngine->level++;
		pEngine->maxHealth+=2;
		pEngine->skillUps+=2;
		//Every 3 levels, increase stamina by 1
		if (pEngine->level % 3 == 0) pEngine->maxStamina++;
	}

	//REQ 4. (2/2) Keyboard input handled
	switch (currentState) {
	case(CharState::stateIdle):
		if (pEngine->stamina > 0) {
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

			//Redo order of depth for charobjects by height
			pEngine->orderCharsByHeight();

		}
		break;

	case(CharState::stateAttack):
		if (anim_end) currentState = CharState::stateIdle; //back to idle once attack animation done
		break;

	}

	//Ensure that the objects get redrawn on the display
	redrawDisplay();
}

void PlayerObject::virtMouseDown(int iButton, int iX, int iY)
{
	if (iButton == SDL_BUTTON_RIGHT) {
		//Iterate through all DisplayableObjects and check we clicked one
		DisplayableObject* pObj;
		for (int i = 0; i < pEngine->getContentCount(); i++) {
			//skip null objects
			if ((pObj = pEngine->getDisplayableObject(i)) == NULL) continue;

			if (pObj->virtIsPositionWithinObject(iX, iY)) {
				//ENEMY CLICKED?
				EnemyObject* pEnemy = dynamic_cast<EnemyObject*> (pObj);
				if (pEnemy && pEngine->attacks > 0) {
					//Check line-of-sight from player to enemy
					//if (lineOfSight(pEngine->GetPlayer()->getXPos(), pEngine->GetPlayer()->getYPos(), pEnemy->getXPos(), pEnemy->getYPos(), 0)) {

					if (std::abs(m_iCurrentScreenX - pEnemy->getXPos()) + std::abs(m_iCurrentScreenY - pEnemy->getYPos()) < (wep->range) * TILE_SIZE) {
						//Attack if in range
						currentState = CharState::stateAttack;
						anim_frame = 0;

						pEngine->GetAudio()->playAudio("sfx/combat/Slash2.ogg", -1, 0);
						pEngine->attacks--;

						//For melee, just do the attack immediately
						attack(pEnemy);

						//else, fire projectile, which will trigger attack() on hit
					}
				}
			}

		}
	}
}

//Applies attack damage
void PlayerObject::attack(EnemyObject* pEnemy)
{
	pEnemy->damage(pEngine->strength);
}

//Called by ProjectileObject at the end of its movement - this just implements CharObject's virtual function, telling it to call our attack() function on the target
void PlayerObject::onProjectileHit(CharObject* target)
{
	attack((EnemyObject*)target);
}

void PlayerObject::move(int xmove, int ymove, int currentTime, int time)
{
	//SolidTileManager tileManager = ((Psybc5Engine*)getEngine())->GetTilesSolid();
	auto tileManager = pEngine->GetTilesSolid();

	//Check for collision in the Tile we want to move into
	if (tileManager->isValidTilePosition(m_iCurrentScreenX + xmove, m_iCurrentScreenY + ymove)) {
		int tileX = tileManager->getMapXForScreenX(m_iCurrentScreenX + xmove);
		int tileY = tileManager->getMapYForScreenY(m_iCurrentScreenY + ymove);
		int tileCollide = tileManager->getMapValue(tileX, tileY);

		//Collision with Foreground Tile
		if (tileCollide != 0) return;

	}

	//Can't move onto other CharObjects' occupied tiles
	DisplayableObject* pObj;
	for (int i = 0; i < pEngine->getContentCount(); i++) {
		if ((pObj = pEngine->getDisplayableObject(i)) == NULL) continue; //skip null objects
		CharObject* pChar = dynamic_cast<CharObject*> (pObj);
		if (pChar && pChar->getXPos() == m_iCurrentScreenX + xmove && pChar->getYPos() == m_iCurrentScreenY + ymove) return; //check if cast worked, then check if object is at the position we want to move to
	}

	//set flipX accordingly (only change if we switch directions)
	if (xmove > 0) flipX = false; else if (xmove < 0) flipX = true;

	pEngine->GetAudio()->playAudio("sfx/combat/fst_conc_solid_run_01.wav", -1, 0);
	pEngine->stamina--;

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