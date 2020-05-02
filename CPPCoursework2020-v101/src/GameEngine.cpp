#include "header.h"
#include "GameEngine.h"

#include "BaseState.h"
#include "StateMenu.h"
#include "StateStart.h"
#include "StateRunning.h"

#include "CharObject.h"

GameEngine::GameEngine()
	: filterScaling(0, 0, this), filterTranslation(0, 0, &filterScaling), player(NULL),
	maxHealth(10), health(maxHealth),
	maxStamina(4), stamina(maxStamina),
	maxMagic(1), magic(maxMagic),
	maxAttacks(1), attacks(maxAttacks),
	strength(1), ranged(1), defence(1),
	exp(25), expNext(50), level(1), skillUps(0)
{
	currentState = nullptr;

	//Create objects for each state
	stateMenu = new StateMenu(this);
	stateCharCreate = new StateCharCreate(this);
	stateStart = new StateStart(this);
	stateRunning = new StateRunning(this);

	currentState = stateMenu;
	currentState->onStateEnter();
}

GameEngine::~GameEngine()
{
	delete stateMenu;
	delete stateCharCreate;
	delete stateStart;
	delete stateRunning;
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
	audio.closeAudio();
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
	drawForegroundRectangle(x1, y1, x1 + maxWidth, y2, colBack); //bg
	drawForegroundRectangle(x1, y1, x1 + (maxWidth * (value / maxValue)), y2, colBar); //bar
	drawForegroundString(x1 + 10, y1, str.c_str(), 0xffffff, NULL); //text
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
	auto sm = &saveManager;
	auto save = sm->openFile("saves/testSave.txt");

	sm->writeLine("name",playerName);
	sm->writeLine("level", std::to_string(level));
	sm->writeLine("exp", std::to_string(exp));
	sm->writeLine("expnext", std::to_string(expNext));
	sm->writeLine("skillups", std::to_string(skillUps));

	sm->writeLine("health", std::to_string(health));
	sm->writeLine("maxhealth", std::to_string(maxHealth));
	sm->writeLine("stamina", std::to_string(stamina));
	sm->writeLine("maxstamina", std::to_string(maxStamina));
	sm->writeLine("magic", std::to_string(magic));
	sm->writeLine("maxmagic", std::to_string(maxMagic));

	sm->writeLine("strength", std::to_string(strength));
	sm->writeLine("ranged", std::to_string(ranged));
	sm->writeLine("defence", std::to_string(defence));

	sm->closeFile();
}