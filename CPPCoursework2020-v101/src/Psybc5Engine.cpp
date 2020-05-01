#include "header.h"
#include "Psybc5Engine.h"

#include "PlayerObject.h"
#include "EnemyZombieObject.h"

Psybc5Engine::Psybc5Engine()
	: currentState(GameState::stateStart) //Start in Initial state
	, player(NULL),
	filterScaling(0, 0, this), filterTranslation(0, 0, &filterScaling),
	maxHealth(10), health(maxHealth),
	maxStamina(4), stamina(maxStamina),
	maxMagic(1), magic(maxMagic),
	maxAttacks(1), attacks(maxAttacks),
	strength(1), ranged(1), defence(1),
	exp(25), expNext(50), level(1), skillUps(0)
{
	audio.audioInit();
	fonts.loadFonts();
}

Psybc5Engine::~Psybc5Engine() {

}

int Psybc5Engine::virtInitialise()
{
	audio.playMusic("music/Mystic-Dungeon.ogg", -1);

	getBackgroundSurface()->setDrawPointsFilter(&filterTranslation);
	getForegroundSurface()->setDrawPointsFilter(&filterTranslation);

	//notifyObjectsAboutMouse(true);

	//Call BaseEngine init
	return BaseEngine::virtInitialise();
}

//Frees any memory needed and exits the game
void Psybc5Engine::virtCleanUp() {
	audio.closeAudio();
}

void Psybc5Engine::virtSetupBackgroundBuffer() {

	//Sets up the Tiles if game is starting, otherwise just draws the background
	switch (currentState) {
	case (GameState::stateStart):
		
		{

			//Specify how many tiles wide and high
			int w = 8, h = 6;
			objTilesSolid.setMapSize(w, h);
			objTilesBack.setMapSize(24, 16);

			int level[6][8] =
			{
				{11,11,11,11,11,11,11},
				{11,00,00,00,00,00,11},
				{11,00,00,00,00,00,11},
				{11,00,00,00,00,00,11},
				{11,00,00,00,00,00,11},
				{11,11,00,00,00,11,11},
			};

			//Translate level data into values in a Tile Map
			for (int x = 0; x < w; x++) {
				for (int y = 0; y < h; y++) {
					objTilesSolid.setMapValue(x, y, level[y][x]);
				}
			}

			//Floor example (all background tiles set to same value)
			for (int x = 0; x < 24; x++) {
				for (int y = 0; y < 16; y++) {
					objTilesBack.setMapValue(x, y, 1);
				}
			}

			//Set coordinates at which to place the Tile Map (anchored at top-left corner)
			objTilesSolid.setTopLeftPositionOnScreen(256, 64);
			objTilesBack.setTopLeftPositionOnScreen(0, 0);

			//SETUP INVENTORY
			int inv[4][4] = {
				{1,0,1,2},
				{0,3,0,0},
				{0,2,2,2},
				{0,0,1,2}
			};

			w = 4; h = 4;
			objInvTiles.setMapSize(w, h);
			for (int x = 0; x < w; x++) {
				for (int y = 0; y < h; y++)
					objInvTiles.setMapValue(x, y, y + x);
			}

			objInvTiles.setTopLeftPositionOnScreen(m_iWindowWidth / 2 - (w * 64) / 2, m_iWindowHeight / 2 - (h * 64) / 2 );

			//Transition to regular game state
			currentState = GameState::stateMain;

			lockAndSetupBackground();
		}
	break;

	case (GameState::stateMain):

		//Draw all tiles from the Tile Map to the background surface
		objTilesBack.drawAllTiles(this, getBackgroundSurface());
		objTilesSolid.drawAllTiles(this, getBackgroundSurface());
		
	break;

	}
}


void Psybc5Engine::virtDrawStringsUnderneath() {
	
}

//REQ 6. Draw text to foreground
void Psybc5Engine::virtDrawStringsOnTop() {
	switch (currentState) {
		case (GameState::statePause):

			//PAUSE MENU

			drawForegroundRectangle(
				m_iWindowWidth / 2 - 256,
				m_iWindowHeight / 2 - 256,
				m_iWindowWidth / 2 + 256,
				m_iWindowHeight / 2 + 256,
				0x816096);

			drawForegroundString(m_iWindowWidth/2, 128, "PAUSED", 0xffffff, NULL);

			std::string printStrength = "STR: " + std::to_string(strength);
			std::string printRanged = "RANGE: " + std::to_string(ranged);
			std::string printMagic = "MAG: " + std::to_string(magic);
			std::string printDefence = "DEF: " + std::to_string(defence);
			drawForegroundString(m_iWindowWidth / 2 - 128, 128 + 30, printStrength.c_str(), 0xffffff, NULL);
			drawForegroundString(m_iWindowWidth / 2 - 128, 128 + 50, printRanged.c_str(), 0xffffff, NULL);
			drawForegroundString(m_iWindowWidth / 2 - 128, 128 + 70, printMagic.c_str(), 0xffffff, NULL);
			drawForegroundString(m_iWindowWidth / 2 - 128, 128 + 90, printDefence.c_str(), 0xffffff, NULL);

			//Drawing the Inventory here
			objInvTiles.drawAllTiles(this, getForegroundSurface());

			break;
	}


	//INGAME UI

	//EXP Bar
	std::string printExp = "EXP: " + std::to_string(exp) + " / " + std::to_string(expNext);
	drawBar(10, 10, 40, 300, printExp, exp, expNext, 0xfcdf03, 0xffee96);

	//Health Bar
	std::string printHealth = "Health: " + std::to_string(health) + " / " + std::to_string(maxHealth);
	drawBar(10, 660, 690, 300, printHealth, health, maxHealth, 0xff0303, 0xff8585);

	//Stamina Orbs
	drawForegroundString(20, 700, "Stamina: ", 0xffffff, NULL);
	for (int i = 0; i < maxStamina; i++) {
		//Draw circle OR greyed-out circle
		int col = 0x0ffa5e;
		if (i >= stamina) col = 0x91ffb6;

		drawForegroundOval( 130 + (i * 40), 700, 160 + (i * 40), 730, col);
	}


	//Magic Orbs
	drawForegroundString(20, 730, "Magic: ", 0xffffff, NULL);
	for (int i = 0; i < maxMagic; i++) {
		//Draw circle OR greyed-out circle
		int col = 0x0398fc;
		if (i >= magic) col = 0x87d0ff;

		drawForegroundOval( 100 + (i*40), 740, 130 + (i*40), 770, col);
	}

}


void Psybc5Engine::virtMouseDown(int iButton, int iX, int iY) {

	switch (currentState) {

	case(GameState::stateMain):
		//LEFT CLICK
		if (iButton == SDL_BUTTON_LEFT)
		{

			//If Tile clicked on:
			if (objTilesSolid.isValidTilePosition(iX, iY))
			{
				//Get column, row, current value
				int mapX = objTilesSolid.getMapXForScreenX(iX);
				int mapY = objTilesSolid.getMapYForScreenY(iY);
				int value = objTilesSolid.getMapValue(mapX, mapY);
				objTilesSolid.setAndRedrawMapValueAt(mapX, mapY, 0, this, getBackgroundSurface()); //Delete tile (by changing to 0, the default empty tile)
			}

			//Force redraw screen
			lockAndSetupBackground();
			redrawDisplay();
		}

		if (iButton == SDL_BUTTON_RIGHT)
		{
			//Iterate through all DisplayableObjects and check we clicked one
			DisplayableObject* pObj;
			for (int i = 0; i < getContentCount(); i++)
			{
				//skip null objects
				if ((pObj = getDisplayableObject(i)) == NULL) continue;

				if (pObj->virtIsPositionWithinObject(iX, iY)) {

					//std::cout << "DisplayableObject clicked";

					//ENEMY CLICKED?
					EnemyObject* pEnemy = dynamic_cast<EnemyObject*> (pObj);
					if (pEnemy) {
						//std::cout << "Enemy clicked";

						if (attacks > 0) {

							//Check line-of-sight from player to enemy
							if (player->lineOfSight(player->getXPos(), player->getYPos(), pEnemy->getXPos(), pEnemy->getYPos(), 0)) {
								audio.playAudio("sfx/combat/Slash2.ogg", -1, 0);
								pEnemy->damage(strength);
								attacks--;
							}
						}
					}
				}

			}
		}

		break;

	case(GameState::statePause):

		//Check if clicked on an inventory Tile
		if (objInvTiles.isValidTilePosition(iX, iY))
		{
			int mapX = objTilesSolid.getMapXForScreenX(iX);
			int mapY = objTilesSolid.getMapYForScreenY(iY);
			int value = objTilesSolid.getMapValue(mapX, mapY);

			//If holding an object, drop it
			if (heldItem == NULL) {
				heldItem = objInvTiles.getItemAt(value);
			}

			//If not holding an object, pick it up
			else {
				objInvTiles.setItemAt(value, heldItem);
			}
		}

		break;

	}

}

//Scroll to zoom in and out
void Psybc5Engine::virtMouseWheel(int x, int y, int which, int timestamp)
{
	// We grab the position of the centre of the screen before the zoom
	int iOldCentreX = convertClickedToVirtualPixelXPosition(this->getWindowWidth() / 2);
	int iOldCentreY = convertClickedToVirtualPixelYPosition(this->getWindowHeight() / 2);

	if (y < 0)
		filterScaling.compress();
	else if (y > 0)
		filterScaling.stretch();

	// Now we grab the position after the zoom
	int iNewCentreX = convertClickedToVirtualPixelXPosition(this->getWindowWidth() / 2);
	int iNewCentreY = convertClickedToVirtualPixelYPosition(this->getWindowHeight() / 2);

	//Apply a translation to offset so it appears to have zoomed on the centre by moving the old centre back to the centre of the screen
	filterTranslation.changeOffset(iNewCentreX - iOldCentreX, iNewCentreY - iOldCentreY);

	//Redraw the background
	lockAndSetupBackground();
	redrawDisplay();
}


//REQ 4. Keypress Handling
void Psybc5Engine::virtKeyDown(int iKeyCode) {

	//Using SDL Key Codes
	switch (iKeyCode) {

	//PAUSE / UNPAUSE
	case(SDLK_RETURN):

		audio.playAudio("sfx/doot.wav", -1, 0);

		switch (currentState) {

		//Main -> Pause
		case (GameState::stateMain):
			pause();
			currentState = GameState::statePause;
			//Force redraw screen
			redrawDisplay();
		break;

		//Pause -> Main
		case (GameState::statePause):
			unpause();
			currentState = GameState::stateMain;
			//Force redraw screen
			redrawDisplay();
		break;
		}
			
	break;

	case(SDLK_SPACE):
		//RESET TURN
		stamina = maxStamina;
		attacks = maxAttacks;
		break;

	case SDLK_ESCAPE:
		//EXIT GAME
		setExitWithCode(0);
		break;


	//MOVING CAMERA
	case SDLK_a:
		moveCamera(50, 0);
		break;
	case SDLK_d:
		moveCamera(-50, 0);
		break;
	case SDLK_w:
		moveCamera(0, 50);
		break;
	case SDLK_s:
		moveCamera(0, -50);
		break;

	}

}

int Psybc5Engine::virtInitialiseObjects()
{
	//Prevents other functions from trying to use destroyed objects
	drawableObjectsChanged();

	//Destroy any existing objects
	destroyOldObjects(true);

	player = new PlayerObject(this,&objTilesSolid);

	createObjectArray(3); //(need to leave one empty element at end of array)
	appendObjectToArray(player);
	appendObjectToArray(new EnemyZombieObject(this));

	setAllObjectsVisible(true);

	return 0;
}

//Check if we're at the edge of a map
void Psybc5Engine::moveCamera(int offsetXIncrement, int offsetYIncrement) {
	/*if (filterTranslation.getXOffset() > 0 &&
		filterTranslation.getYOffset() > 0 &&
		filterTranslation.getXOffset() < 2000 &&
		filterTranslation.getYOffset() < 1000) {
		*/

		filterTranslation.changeOffset(offsetXIncrement, offsetYIncrement);
		lockAndSetupBackground();

		redrawRectangle(0, 0, WIN_WIDTH, WIN_HEIGHT); //only redraw within the current screen bounds

		//redrawDisplay();
	//}
}

void Psybc5Engine::drawBar(int x1, int y1, int y2, int maxWidth, std::string str, int value, int maxValue, int colBar, int colBack)
{
	drawForegroundRectangle(x1, y1, x1 + maxWidth, y2, colBack); //bg
	drawForegroundRectangle(x1, y1, x1 + (maxWidth * (value / maxValue)), y2, colBar); //bar
	drawForegroundString(x1+10, y1, str.c_str(), 0xffffff, NULL); //text
}

//This will be called every time a CharObject moves in the y axis
//The object with the largest y value should be at the top, then in descending order
void Psybc5Engine::orderCharsByHeight()
{
	//Iterate through all DisplayableObjects and check we clicked one
	DisplayableObject* pObj;
	for (int i = 0; i < getContentCount(); i++) {
		//skip null objects
		if ((pObj = getDisplayableObject(i)) == NULL) continue;

		CharObject* pChar = dynamic_cast<CharObject*> (pObj);

		//Check the object currently at the top, place pChar on top if it's lower on the screen
		if (pChar) {
			DisplayableObject* pTopObj = getContentItem(getContentCount() - 1);
			if (pTopObj->getYCentre() < pChar->getYCentre()) moveToLast(pChar);
		}
	}
}