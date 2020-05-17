#include "header.h"
#include "GameEngine.h"

#include "BaseState.h"
#include "StateMenu.h"
#include "StateStart.h"
#include "StateRunning.h"

#include "CharObject.h"
#include "FloorManager.h"

GameEngine::GameEngine()
	: filterScaling(0, 0, this), filterTranslation(0, 0, &filterScaling), player(NULL),
	maxHealth(10), health(maxHealth),
	maxStamina(4), stamina(maxStamina),
	maxMagic(1), magic(maxMagic),
	maxAttacks(1), attacks(maxAttacks),
	strength(1), ranged(1), defence(1),
	exp(25), expNext(50), level(1), skillUps(0)
{
	notifyObjectsAboutMouse(true);

	gen = new FloorManager();

	objTilesSolid = std::make_shared<SolidTileManager>();
	objTilesBack = std::make_shared<BackgroundTileManager>();
	objInvTiles = std::make_shared<InventoryTileManager>(this);

	currentState = nullptr;

	//Create objects for each state
	stateMenu = new StateMenu(this);
	stateStart = new StateStart(this);
	stateRunning = new StateRunning(this);

	currentState = stateMenu;
	currentState->onStateEnter();
}

GameEngine::~GameEngine()
{
	
}


void GameEngine::setState(BaseState* state)
{
	if (currentState) currentState->onStateExit();

	currentState = state;
	lockAndSetupBackground();
	redrawDisplay();

	currentState->onStateEnter();
}

BaseState* GameEngine::getState()
{
	return currentState;
}


//VIRTUAL FUNCTIONS:

int GameEngine::virtInitialise()
{
	audio.audioInit();

	getBackgroundSurface()->setDrawPointsFilter(&filterTranslation);
	getForegroundSurface()->setDrawPointsFilter(&filterTranslation);

	createObjectArray(1000);

	return BaseEngine::virtInitialise();
}

void GameEngine::virtCleanUp()
{
	audio.closeAudio();

	delete stateMenu;
	delete stateStart;
	delete stateRunning;

	delete gen;

	clearObjects();
}

void GameEngine::clearObjects()
{
	//Free everything in object container
	destroyOldObjects(true);

	//THEN free everything in Floor
	for (const std::vector<Room*>& v : floor) {
		for (Room* x : v) {
			delete x;
		}
	}
}

void GameEngine::virtSetupBackgroundBuffer()
{
	currentState->virtSetupBackgroundBuffer();
}

void GameEngine::virtDrawStringsOnTop()
{
	currentState->virtDrawStringsOnTop();
}

void GameEngine::virtDrawStringsUnderneath()
{
	currentState->virtDrawStringsUnderneath();
}

void GameEngine::virtMouseDown(int iButton, int iX, int iY)
{
	currentState->virtMouseDown(iButton, iX, iY);
}

void GameEngine::virtMouseWheel(int x, int y, int which, int timestamp)
{
	currentState->virtMouseWheel(x, y, which, timestamp);
}

void GameEngine::virtKeyDown(int iKeyCode)
{
	currentState->virtKeyDown(iKeyCode);
}

//OBJECTS
int GameEngine::virtInitialiseObjects()
{
	return BaseEngine::virtInitialiseObjects();
}

void GameEngine::virtMainLoopPreUpdate()
{
	currentState->virtMainLoopPreUpdate();
}

void GameEngine::virtMainLoopPostUpdate()
{
	currentState->virtMainLoopPostUpdate();
}

void GameEngine::virtMainLoopDoBeforeUpdate()
{
	currentState->virtMainLoopDoBeforeUpdate();
}

void GameEngine::virtMainLoopDoAfterUpdate()
{
	currentState->virtMainLoopDoAfterUpdate();
}

//Check if we're at the edge of a map
void GameEngine::moveCamera(int offsetXIncrement, int offsetYIncrement) {
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

void GameEngine::drawBar(int x1, int y1, int y2, int maxWidth, std::string str, int value, int maxValue, int colBar, int colBack)
{
	int barWidth = std::floor((double)maxWidth * ((double)value / (double)maxValue));

	drawForegroundRectangle(x1, y1, x1 + maxWidth, y2, colBack); //bg
	drawForegroundRectangle(x1, y1, x1 + barWidth, y2, colBar); //bar

	drawForegroundString(x1 + 10, y1, str.c_str(), 0xffffff, NULL); //TEXT
}

//This will be called every time a CharObject moves in the y axis
//The object with the largest y value should be at the top, then in descending order
void GameEngine::orderCharsByHeight()
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

//Save function
void GameEngine::saveGame()
{
	auto sm = &saveManager; //shorter pointer for easier readability
	auto save = sm->openFile("saves/testSave.txt");

	sm->writeLine("name",playerName);
	sm->writeLine("level", level);
	sm->writeLine("exp", exp);
	sm->writeLine("expnext", expNext);
	sm->writeLine("skillups", skillUps);

	sm->writeLine("health", health);
	sm->writeLine("maxhealth", maxHealth);
	sm->writeLine("stamina", stamina);
	sm->writeLine("maxstamina", maxStamina);
	sm->writeLine("magic", magic);
	sm->writeLine("maxmagic", maxMagic);

	sm->writeLine("strength", strength);
	sm->writeLine("ranged", ranged);
	sm->writeLine("defence", defence);

	sm->closeFile();
}

void GameEngine::resetStats()
{
	playerName = "";
	maxHealth = 10, health = maxHealth;
	maxStamina = 4, stamina = maxStamina;
	maxMagic = 1, magic = maxMagic;
	strength= 1, ranged = 1, defence = 1;
	exp = 0, expNext = 50, level = 1, skillUps = 0;
	maxAttacks = 1, attacks = maxAttacks;
	skillUps = 5;
}