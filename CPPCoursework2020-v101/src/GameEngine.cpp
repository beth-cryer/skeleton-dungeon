#include "header.h"
#include "GameEngine.h"

#include "BaseState.h"
#include "StateMenu.h"
#include "StateStart.h"
#include "StateRunning.h"

#include "PlayerObject.h"
#include "CharObject.h"
#include "FloorManager.h"

GameEngine::GameEngine()
	: filterScaling(0, 0, this), filterTranslation(0, 0, &filterScaling),
	maxHealth(10), health(maxHealth),
	maxStamina(4), stamina(maxStamina),
	maxMagic(1), magic(maxMagic),
	maxAttacks(1), attacks(maxAttacks),
	strength(1), ranged(1), defence(1),
	exp(0), expNext(50), level(1), skillUps(0)
{
	notifyObjectsAboutMouse(true);

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

	return BaseEngine::virtInitialise();
}

void GameEngine::virtCleanUp()
{
	//If the player hits X while ingame, clear objects. If we're in the menu then the objects have already been cleared
	if (currentState != stateMenu && currentState != stateMenu->stateCharCreate && currentState != stateMenu->stateInfo)
		clearObjects();

	audio.closeAudio();

	delete stateMenu;
	delete stateStart;
	delete stateRunning;
}

//A reusable functions that deletes a lot of objects. Called before each new floor, also called on entering the main menu
void GameEngine::clearObjects()
{
	//Free everything in object container
	destroyOldObjects(true);

	delete player;

	//THEN free everything in Floor
	for (const std::vector<Room*>& v : floor) {
		for (Room* x : v) {
			delete x;
		}
	}

	delete gen;
}

//A reusable function that creates new objects. Called after each floor to reset things
void GameEngine::createObjects()
{
	gen = new FloorManager();
}

//Transitions to next level, saving the game and then deleting relevant objects first
void GameEngine::nextLevel()
{
	saveGame();
	clearObjects();
	setState(stateStart);
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

//Ensures stats don't go over their maximums
//Can be called by objects when they change a player stat
void GameEngine::limitStats()
{
	//Make sure stats don't go over their max
	if (health > maxHealth) health = maxHealth;
	if (stamina > maxStamina) stamina = maxStamina;
	if (magic > maxMagic) magic = maxMagic;
}

//Save function
void GameEngine::saveGame()
{
	auto sm = &saveManager; //shorter pointer for easier readability
	auto save = sm->openFile("saves/save.txt");

	sm->writeLine("name", playerName);
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

	sm->writeLine("floor", currentFloor);

	sm->closeFile();
}

void GameEngine::loadGame()
{
	//Load save
	auto loader = GetSaveManager();
	loader->loadFileContents("saves/save.txt");

	playerName = loader->getSaveData("name");
	level = std::stoi(loader->getSaveData("level"));
	exp = std::stoi(loader->getSaveData("exp"));
	expNext = std::stoi(loader->getSaveData("expnext"));
	skillUps = std::stoi(loader->getSaveData("skillups"));

	maxHealth = std::stoi(loader->getSaveData("maxhealth"));
	health = std::stoi(loader->getSaveData("health"));
	maxStamina = std::stoi(loader->getSaveData("maxstamina"));
	stamina = std::stoi(loader->getSaveData("stamina"));
	maxMagic = std::stoi(loader->getSaveData("maxmagic"));
	magic = std::stoi(loader->getSaveData("magic"));

	strength = std::stoi(loader->getSaveData("strength"));
	ranged = std::stoi(loader->getSaveData("ranged"));
	defence = std::stoi(loader->getSaveData("defence"));

	currentFloor = std::stoi(loader->getSaveData("floor"));

	//std::string solidTiles = loader->getSaveData("solid");
	//std::string solidBack = loader->getSaveData("back");
}

void GameEngine::resetStats()
{
	playerName.assign("Skeleton");
	maxHealth = 20, health = maxHealth;
	maxStamina = 4, stamina = maxStamina;
	maxMagic = 1, magic = maxMagic;
	strength= 1, ranged = 1, defence = 1;
	exp = 0, expNext = 50, level = 1, skillUps = 0;
	maxAttacks = 1, attacks = maxAttacks;
	skillUps = 3;
	currentFloor = 1;
}