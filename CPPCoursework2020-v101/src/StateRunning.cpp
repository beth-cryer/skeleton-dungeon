#include "header.h"
#include "StateRunning.h"
#include "StateMenu.h"

#include "AudioPlayer.h"
#include "PlayerObject.h"
#include "CharObject.h"
#include "EnemyObject.h"

StateRunning::StateRunning(GameEngine* pEngine) : BaseState(pEngine)
{

}

StateRunning::~StateRunning()
{
	delete statePaused;
	delete stateEnemyTurn;
}

void StateRunning::onStateEnter()
{
	//Create state objects
	if (statePaused == nullptr)
	{
		statePaused = new StatePaused(pEngine);
		stateEnemyTurn = new StateEnemyTurn(pEngine);
	}

	auto audio = pEngine->GetAudio();
	auto path = "music/Exploration_1.ogg";
	if (!audio->isSongPlaying(path)) audio->playMusic(path, -1);

	pEngine->stamina = pEngine->maxStamina;
	pEngine->attacks = pEngine->maxAttacks;
}

void StateRunning::onStateExit()
{

}

void StateRunning::virtSetupBackgroundBuffer()
{
	pEngine->fillBackground(0);

	//Draw all tiles from the Tile Map to the background surface
	pEngine->GetTilesBack()->drawAllTiles(pEngine, pEngine->getBackgroundSurface());
	pEngine->GetTilesSolid()->drawAllTiles(pEngine, pEngine->getBackgroundSurface());
}

void StateRunning::virtDrawStringsOnTop()
{
	BaseState::virtDrawStringsOnTop();

	//INGAME UI

	//EXP Bar
	std::string printExp = "EXP: " + std::to_string(pEngine->exp) + " / " + std::to_string(pEngine->expNext);
	pEngine->drawBar(10, 10, 40, 300, printExp, pEngine->exp, pEngine->expNext, 0xfcdf03, 0xffee96);

	//Health Bar
	std::string printHealth = "Health: " + std::to_string(pEngine->health) + " / " + std::to_string(pEngine->maxHealth);
	pEngine->drawBar(10, 660, 690, 300, printHealth, pEngine->health, pEngine->maxHealth, 0xff0303, 0xff8585);

	//Stamina Orbs
	pEngine->drawForegroundString(20, 700, "Stamina: ", 0xffffff, NULL);
	for (int i = 0; i < pEngine->maxStamina; i++) {
		//Draw circle OR greyed-out circle
		int col = 0x0ffa5e;
		if (i >= pEngine->stamina) col = 0x91ffb6;

		pEngine->drawForegroundOval(130 + (i * 40), 700, 160 + (i * 40), 730, col);
	}


	//Magic Orbs
	pEngine->drawForegroundString(20, 730, "Magic: ", 0xffffff, NULL);
	for (int i = 0; i < pEngine->maxMagic; i++) {
		//Draw circle OR greyed-out circle
		int col = 0x0398fc;
		if (i >= pEngine->magic) col = 0x87d0ff;

		pEngine->drawForegroundOval(100 + (i * 40), 740, 130 + (i * 40), 770, col);
	}
}

void StateRunning::virtDrawStringsUnderneath()
{

}

void StateRunning::virtMouseDown(int iButton, int iX, int iY)
{
	BaseState::virtMouseDown(iButton, iX, iY);
}

void StateRunning::virtMouseWheel(int x, int y, int which, int timestamp)
{
	//Get position of the centre of the screen before the zoom
	int iOldCentreX = pEngine->convertClickedToVirtualPixelXPosition(WIN_WIDTH / 2);
	int iOldCentreY = pEngine->convertClickedToVirtualPixelYPosition(WIN_HEIGHT / 2);

	if (y < 0) {
		if (pEngine->filterScaling.getZoomX() <= 0) return;
		pEngine->filterScaling.compress();
	}
	else if (y > 0) {
		pEngine->filterScaling.stretch();
	}

	//Get position of the centre after the zoom
	int iNewCentreX = pEngine->convertClickedToVirtualPixelXPosition(WIN_WIDTH / 2);
	int iNewCentreY = pEngine->convertClickedToVirtualPixelYPosition(WIN_HEIGHT / 2);

	//Apply a translation to offset so it appears to have zoomed on the centre by moving the old centre back to the centre of the screen
	pEngine->filterTranslation.changeOffset(iNewCentreX - iOldCentreX, iNewCentreY - iOldCentreY);

	//Redraw the background
	pEngine->lockAndSetupBackground();
	pEngine->redrawDisplay();
}

void StateRunning::virtKeyDown(int iKeyCode)
{
	switch (iKeyCode) {

		//NEXT TURN
	case(SDLK_SPACE):
		pEngine->setState(stateEnemyTurn);
		
		break;

		//PAUSE
	case(SDLK_RETURN):
		pEngine->setState(statePaused);
		break;

		//BACK TO MENU
	case SDLK_ESCAPE:
		pEngine->setState(pEngine->stateMenu);
		break;

		//MOVING CAMERA
	case SDLK_a:
		pEngine->moveCamera(50, 0);
		break;
	case SDLK_d:
		pEngine->moveCamera(-50, 0);
		break;
	case SDLK_w:
		pEngine->moveCamera(0, 50);
		break;
	case SDLK_s:
		pEngine->moveCamera(0, -50);
		break;
	}
}

void StateRunning::virtMainLoopPreUpdate()
{

}

void StateRunning::virtMainLoopPostUpdate()
{

}

//This will be called every time a CharObject moves in the y axis
//The object with the largest y value should be at the top, then in descending order
void StateRunning::orderCharsByHeight()
{
	//Iterate through all DisplayableObjects and check we clicked one
	DisplayableObject* pObj;
	for (int i = 0; i < pEngine->getContentCount(); i++) {
		//skip null objects
		if ((pObj = pEngine->getDisplayableObject(i)) == NULL) continue;

		CharObject* pChar = dynamic_cast<CharObject*> (pObj);

		//Check the object currently at the top, place pChar on top if it's lower on the screen
		if (pChar) {
			DisplayableObject* pTopObj = pEngine->getContentItem(pEngine->getContentCount() - 1);
			if (pTopObj->getYCentre() < pChar->getYCentre()) pEngine->moveToLast(pChar);
		}
	}
}


//PAUSE STATE

StatePaused::StatePaused(GameEngine* pEngine) : StateRunning(pEngine)
{

}

void StatePaused::onStateEnter()
{
	pEngine->pause();
}

void StatePaused::onStateExit()
{
	pEngine->unpause();
}

void StatePaused::virtDrawStringsOnTop()
{
	//Keep drawing UI elements
	StateRunning::virtDrawStringsOnTop();

	//Draw pause menu UI
	pEngine->drawForegroundRectangle(WIN_WIDTH / 2 - 256, WIN_HEIGHT / 2 - 256, WIN_WIDTH / 2 + 256, WIN_HEIGHT / 2 + 256, 0x000000);

	pEngine->drawForegroundString(WIN_WIDTH / 2, 128, "PAUSED", 0xffffff, NULL);

	std::string printStrength = "STR: " + std::to_string(pEngine->strength);
	std::string printRanged = "RANGE: " + std::to_string(pEngine->ranged);
	std::string printMagic = "MAG: " + std::to_string(pEngine->maxMagic);
	std::string printDefence = "DEF: " + std::to_string(pEngine->defence);
	pEngine->drawForegroundString(WIN_WIDTH / 2 - 128, 128 + 30, printStrength.c_str(), 0xffffff, NULL);
	pEngine->drawForegroundString(WIN_WIDTH / 2 - 128, 128 + 50, printRanged.c_str(), 0xffffff, NULL);
	pEngine->drawForegroundString(WIN_WIDTH / 2 - 128, 128 + 70, printMagic.c_str(), 0xffffff, NULL);
	pEngine->drawForegroundString(WIN_WIDTH / 2 - 128, 128 + 90, printDefence.c_str(), 0xffffff, NULL);

	//Drawing the Inventory here
	pEngine->GetTilesInv()->drawAllTiles(pEngine, pEngine->getForegroundSurface());
}

void StatePaused::virtKeyDown(int iKeyCode)
{
	switch (iKeyCode) {

		//Unpause
	case(SDLK_RETURN):
		pEngine->setState(pEngine->stateRunning);
		break;

	}
}

void StatePaused::virtMouseDown(int iButton, int iX, int iY)
{
	auto objInvTiles = pEngine->GetTilesInv();

	/*
	//Check if clicked on an inventory Tile
	if (objInvTiles->isValidTilePosition(iX, iY))
	{
		int mapX = objInvTiles->getMapXForScreenX(iX);
		int mapY = objInvTiles->getMapYForScreenY(iY);
		int value = objInvTiles->getMapValue(mapX, mapY);

		//If holding an object, drop it
		if (pEngine->heldItem == NULL) {
			pEngine->heldItem = objInvTiles->getItemAt(value);
		}

		//If not holding an object, pick it up
		else {
			objInvTiles->setItemAt(value, pEngine->heldItem);
		}
	}
	*/

}

void StatePaused::virtMainLoopPreUpdate()
{
	//Update currently moused-over item

}

//Overridding zoom mechanism
void StatePaused::virtMouseWheel(int x, int y, int which, int timestamp)
{

}


		//ENEMY TURN STATE\\
//Pretty similar to Running, but the player can't do anything

StateEnemyTurn::StateEnemyTurn(GameEngine* pEngine) : StateRunning(pEngine)
{

}

void StateEnemyTurn::onStateEnter()
{
	enemyTurns.clear();

	//Get list of all enemy objects
	DisplayableObject* pObj;
	for (int i = 0; i < pEngine->getContentCount(); i++) {
		if ((pObj = pEngine->getDisplayableObject(i)) == NULL) continue; //skip null objects

		EnemyObject* pEnemy = dynamic_cast<EnemyObject*> (pObj);
		if (pEnemy) enemyTurns.push_back(pEnemy); //add to list
	}

	//Trigger first enemy turn
	triggerNextEnemy();
}

void StateEnemyTurn::onStateExit()
{

}

void StateEnemyTurn::virtDrawStringsOnTop()
{
	//Keep drawing UI elements
	StateRunning::virtDrawStringsOnTop();

	pEngine->drawForegroundString(0,0,"ENEMY TURN",0,NULL);
}


void StateEnemyTurn::virtKeyDown(int iKeyCode)
{

}


//Called by an enemy at the end of its turn to signal that the next enemy should take its turn
//Continues until enemyTurns is empty, then back to Running state
void StateEnemyTurn::triggerNextEnemy()
{
	if (enemyTurns.size() > 0) {

		//Trigger turn on first enemy, handing it control until its turn is finished
		enemyTurns.front()->turnStart();
		enemyTurns.pop_front(); //and remove it from the list

	}
	else {
		pEngine->setState(pEngine->stateRunning);
	}
}