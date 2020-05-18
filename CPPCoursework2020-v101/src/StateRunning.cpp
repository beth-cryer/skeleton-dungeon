#include "header.h"
#include "StateRunning.h"
#include "StateMenu.h"

#include "AudioPlayer.h"
#include "PlayerObject.h"
#include "CharObject.h"
#include "EnemyObject.h"

StateRunning::StateRunning(GameEngine* pEngine) : BaseState(pEngine)
{
	fntText = pEngine->getFont("fonts/gameplay.ttf", 15);
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

	enemyTurn = false;
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

	//NEXT TURN prompt
	if (pEngine->stamina == 0 && !enemyTurn) pEngine->drawForegroundString(WIN_CENTREX-128,WIN_HEIGHT-32,"Next Turn (SPACE)",0xFFFFFF,NULL);
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
	int iOldCentreX = pEngine->convertClickedToVirtualPixelXPosition(WIN_CENTREX);
	int iOldCentreY = pEngine->convertClickedToVirtualPixelYPosition(WIN_CENTREY);

	if (y < 0) {
		if (pEngine->filterScaling.getZoomX() <= 0) return;
		pEngine->filterScaling.compress();
	}
	else if (y > 0) {
		pEngine->filterScaling.stretch();
	}

	//Get position of the centre after the zoom
	int iNewCentreX = pEngine->convertClickedToVirtualPixelXPosition(WIN_CENTREX);
	int iNewCentreY = pEngine->convertClickedToVirtualPixelYPosition(WIN_CENTREY);

	//Apply a translation to offset so it appears to have zoomed on the centre by moving the old centre back to the centre of the screen
	pEngine->filterTranslation.changeOffset(iNewCentreX - iOldCentreX, iNewCentreY - iOldCentreY);

	//Redraw the background
	pEngine->lockAndSetupBackground();
	pEngine->redrawDisplay();
}

void StateRunning::virtKeyDown(int iKeyCode)
{
	PlayerObject* pl = pEngine->GetPlayer();

	switch (iKeyCode) {

		//NEXT TURN
	case(SDLK_SPACE):
		enemyTurn = true;
		pEngine->setState(stateEnemyTurn);
		break;

		//PAUSE
	case(SDLK_RETURN):
		pEngine->setState(statePaused);
		break;

		//BACK TO MENU
	case SDLK_ESCAPE:
		pEngine->clearObjects();
		pEngine->setState(pEngine->stateMenu);
		break;

		//Testing line-of-sight
	case SDLK_l:
		pl->lineOfSight(64, 64, 512, 128, 0);
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
	//Attribute change buttons
	auto skillUps = &(pEngine->skillUps);
	buttons.push_back(std::unique_ptr<Button>(new ButtonAllocateSkill<int>(pEngine, WIN_CENTREX + 10, 128 + 30, 20, 20, 0x2159ff, 0xd4e4ff, "+", fntText, &(pEngine->strength), 1, skillUps)));
	buttons.push_back(std::unique_ptr<Button>(new ButtonAllocateSkill<int>(pEngine, WIN_CENTREX + 10, 128 + 50, 20, 20, 0x2159ff, 0xd4e4ff, "+", fntText, &(pEngine->ranged), 1, skillUps)));
	buttons.push_back(std::unique_ptr<Button>(new ButtonAllocateSkill<int>(pEngine, WIN_CENTREX + 10, 128 + 70, 20, 20, 0x2159ff, 0xd4e4ff, "+", fntText, &(pEngine->maxMagic), 1, skillUps)));
	buttons.push_back(std::unique_ptr<Button>(new ButtonAllocateSkill<int>(pEngine, WIN_CENTREX + 10, 128 + 90, 20, 20, 0x2159ff, 0xd4e4ff, "+", fntText, &(pEngine->defence), 1, skillUps)));
}

void StatePaused::onStateEnter()
{
	pEngine->pause();

	pEngine->GetAudio()->playAudio("sfx/objects/Book1.ogg", -1, 0);
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
	pEngine->drawForegroundRectangle(0, WIN_CENTREY - 256, WIN_WIDTH, WIN_CENTREY + 256, 0x000000); //Background panel

	//STATS
	pEngine->drawForegroundRectangle(WIN_CENTREX - 128, WIN_CENTREY - 256, WIN_CENTREX + 128, WIN_CENTREY - 96, 0xffffff); //Stats panel

	pEngine->drawForegroundString(WIN_CENTREX - 120, 128, pEngine->playerName.c_str(), 0x000000, NULL);
	std::string printSkillUps = "PTS: " + std::to_string(pEngine->skillUps);
	std::string printStrength = "STR: " + std::to_string(pEngine->strength);
	std::string printRanged = "RANGE: " + std::to_string(pEngine->ranged);
	std::string printMagic = "MAG: " + std::to_string(pEngine->maxMagic);
	std::string printDefence = "DEF: " + std::to_string(pEngine->defence);
	pEngine->drawForegroundString(WIN_CENTREX - 120, 128 + 30, printStrength.c_str(), 0x000000, fntText);
	pEngine->drawForegroundString(WIN_CENTREX - 120, 128 + 50, printRanged.c_str(), 0x000000, fntText);
	pEngine->drawForegroundString(WIN_CENTREX - 120, 128 + 70, printMagic.c_str(), 0x000000, fntText);
	pEngine->drawForegroundString(WIN_CENTREX - 120, 128 + 90, printDefence.c_str(), 0x000000, fntText);

	pEngine->drawForegroundString(WIN_CENTREX - 120, 128 + 130, printSkillUps.c_str(), 0x000000, fntText);


	//INVENTORY TILES
	pEngine->GetTilesInv()->drawAllTiles(pEngine, pEngine->getForegroundSurface());

	//EQUIPPED WEAPON DETAILS
	auto e_wep = pEngine->GetPlayer()->wep;
	if (e_wep != nullptr) {
		pEngine->drawForegroundRectangle(WIN_CENTREX + 192, WIN_CENTREY -256, WIN_CENTREX + 432, WIN_CENTREY - 112, 0xFFFFFF);

		//Details
		pEngine->drawForegroundString(WIN_CENTREX + 198, WIN_CENTREY - 252, "[EQUIPPED WEAPON]", 0xff0000, pEngine->getFont("fonts/gameplay.ttf", 18));
		pEngine->drawForegroundString(WIN_CENTREX + 195, WIN_CENTREY - 256 + 32, e_wep->name.c_str(), 0x000000, fntText);
		pEngine->drawForegroundString(WIN_CENTREX + 195, WIN_CENTREY - 256 + 48, e_wep->desc.c_str(), 0x000000, pEngine->getFont("fonts/gameplay.ttf", 13));

		//Icon
		int iconVal = e_wep->iconId;
		invSprites.renderImageWithMask(pEngine->getForegroundSurface(), TILE_SIZE * (iconVal % 21), TILE_SIZE * (int)std::floor(iconVal / 20),
			WIN_CENTREX + 205, WIN_CENTREX + 368, TILE_SIZE, TILE_SIZE, 0xFF00FF);

		//Stats
		std::string printDamage = "Damage: " + std::to_string(e_wep->damage);
		std::string printRange = "Range: " + std::to_string(e_wep->range);
		pEngine->drawForegroundString(WIN_CENTREX + 195, WIN_CENTREY - 256 + 64, printDamage.c_str(), 0x000000, fntText);
		pEngine->drawForegroundString(WIN_CENTREX + 195, WIN_CENTREY - 256 + 80, printRange.c_str(), 0x000000, fntText);
	}

	//MOUSED-OVER ITEM DETAILS
	pEngine->drawForegroundRectangle(WIN_CENTREX + 192, WIN_CENTREY + 32 - (4 * 64) / 2, WIN_CENTREX + 432, WIN_CENTREY + 256 - (4 * 64) / 2, 0xFFFFFF);
	//Get currently moused-over item
	auto inv = pEngine->GetTilesInv();
	if (mousedItemID != -1) {
		auto item = inv->getItemAt(mousedItemID);

		//item shouldn't be null, but check just in case
		if (item != nullptr) {
			pEngine->drawForegroundString(WIN_CENTREX + 195, WIN_CENTREY + 32 - (4 * 64) / 2, item->name.c_str(), 0x000000, fntText);
			pEngine->drawForegroundString(WIN_CENTREX + 195, WIN_CENTREY + 32 - (4 * 64) / 2 + 32, item->desc.c_str(), 0x000000, pEngine->getFont("fonts/gameplay.ttf", 13));

			//Weapon stats
			Weapon* wep = dynamic_cast<Weapon*> (item.get());
			if(wep) {
				std::string printDamage = "Damage: " + std::to_string(wep->damage);
				std::string printRange = "Range: " + std::to_string(wep->range);
				pEngine->drawForegroundString(WIN_CENTREX + 195, WIN_CENTREY + 32 - (4 * 64) / 2 + 64, printDamage.c_str(), 0x000000, fntText);
				pEngine->drawForegroundString(WIN_CENTREX + 195, WIN_CENTREY + 32 - (4 * 64) / 2 + 80, printRange.c_str(), 0x000000, fntText);
			}

		}
	}

	//MAP
	std::vector<std::vector<Room*>> map = pEngine->floor;

	int ystart = WIN_CENTREY - 256 + 32;
	int xstart = 32;
	int roomSize = 32;

	int cols = map[0].size();
	int rows = map.size();

	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			//Draw rect for each valid room
			if (map[y][x] != nullptr) {
				int xpos = xstart + (roomSize * x);
				int ypos = ystart + (roomSize * y);
				int roomCol = 0xFFFFFF;

				//In current room?
				if (pEngine->currentRoom != nullptr && pEngine->currentRoom == map[y][x])
					roomCol = 0xFF00FF;

				//Highlight exit room
				if (map[y][x]->exit)
					roomCol = 0xFF0000;

				pEngine->drawForegroundRectangle(xpos, ypos, xpos + roomSize - 4, ypos + roomSize - 4, roomCol);
			}
				
		}
	}

	//We wanna draw buttons on top
	if (pEngine->skillUps > 0)
		BaseState::virtDrawStringsOnTop();

	//Draw currently picked-up item
	if (heldItemID != -1) {
		auto item = inv->getItemAt(heldItemID);

		//If item array points to valid item:
		if (item != nullptr) {
			int mouseX = pEngine->getCurrentMouseX();
			int mouseY = pEngine->getCurrentMouseY();
			int iconID = item->iconId;
			invSprites.renderImageWithMask(pEngine->getForegroundSurface(), TILE_SIZE * (iconID % 21), TILE_SIZE * (int)std::floor(iconID / 20), mouseX - TILE_SIZE/2, mouseY - TILE_SIZE/2, TILE_SIZE, TILE_SIZE, 0xFF00FF);
		}

	}
}

void StatePaused::virtKeyDown(int iKeyCode)
{
	switch (iKeyCode) {

		//Unpause
	case(SDLK_RETURN):
		pEngine->setState(pEngine->stateRunning);
		break;

		//BACK TO MENU
	case SDLK_ESCAPE:
		pEngine->clearObjects();
		pEngine->setState(pEngine->stateMenu);
		break;

	}
}

void StatePaused::virtMouseDown(int iButton, int iX, int iY)
{
	//Allow button clicking
	if (pEngine->skillUps > 0)
		BaseState::virtMouseDown(iButton, iX, iY);


	auto inv = pEngine->GetTilesInv();
	int mapX = 0, mapY = 0, value = 0;
	bool moused = false;

	//Check if clicked on an inventory Tile
	if (inv->isValidTilePosition(iX, iY))
	{
		mapX = inv->getMapXForScreenX(iX);
		mapY = inv->getMapYForScreenY(iY);
		value = inv->getMapValue(mapX, mapY);
		moused = true;
	}

	if (moused) {

		//Left-click to move item
		if (iButton == SDL_BUTTON_LEFT) {
			//If holding an object, drop it
			if (heldItemID != -1) {
				int tempItemID = inv->getMapValue(mapX, mapY); //Get current map value at selection

				//Set moused tile to held item
				inv->setMapValue(mapX, mapY, heldItemID);

				//Pick up existing item if necessary, else set held item to empty
				if (tempItemID != -1) {
					heldItemID = tempItemID;
				}
				else {
					heldItemID = -1;
				}

			}
			//If not holding an object, pick it up
			else {
				heldItemID = inv->getMapValue(mapX, mapY);
				inv->setMapValue(mapX, mapY, -1);
			}
		}

		//Right click to use item
		if (iButton == SDL_BUTTON_RIGHT) {
			//Execute useItem on moused-over item
			int itemID = inv->getMapValue(mapX, mapY);

			if (itemID != -1) {
				auto item = inv->getItemAt(itemID);
				if (item != nullptr) item->virtItemUse();
			}

		}

	}
}

void StatePaused::virtMainLoopPreUpdate()
{
	//Update currently moused-over item
	int mouseX = pEngine->getCurrentMouseX();
	int mouseY = pEngine->getCurrentMouseY();
	auto inv = pEngine->GetTilesInv();

	//Get tile at mouse position
	if (inv->isValidTilePosition(mouseX, mouseY)) {
		int clickedX = inv->getMapXForScreenX(mouseX);
		int clickedY = inv->getMapYForScreenY(mouseY);

		mousedItemID = inv->getMapValue(clickedX, clickedY);
	}

	//Redraw the background
	pEngine->lockAndSetupBackground();
	pEngine->redrawDisplay();
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
	//Restore player stamina
	pEngine->stamina = pEngine->maxStamina;
	pEngine->attacks = pEngine->maxAttacks;
}

void StateEnemyTurn::virtDrawStringsOnTop()
{
	//Keep drawing UI elements
	StateRunning::virtDrawStringsOnTop();

	pEngine->drawForegroundString(WIN_CENTREX - 64, 0, "ENEMY TURN", 0xFFFFFF, NULL);
}


void StateEnemyTurn::virtKeyDown(int iKeyCode)
{

}


//Called by an enemy at the end of its turn to signal that the next enemy should take its turn
//Continues until enemyTurns is empty, then back to Running state
void StateEnemyTurn::triggerNextEnemy()
{
	if (enemyTurns.size() > 0) {

		auto enemy = enemyTurns.front();
		auto player = pEngine->GetPlayer();

		enemyTurns.pop_front(); //and remove it from the list

		//Trigger turn on first enemy, handing it control until its turn is finished
		//(only if aggroed OR player is in line of sight. enemies stay aggroed once their attention is caught)
		if (enemy->aggroed == true || enemy->lineOfSight(player->getXCentre(), player->getYCentre(), enemy->getXCentre(), enemy->getYCentre(), 8)) //range is 8 tiles
		{
			enemy->turnStart();
		}
		else {
			triggerNextEnemy();
		}

		

	}
	else {
		pEngine->setState(pEngine->stateRunning);
	}
}