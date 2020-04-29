#include "header.h"
#include "GameEngine.h"

#include "BaseState.h"
#include "StateMenu.h"
#include "StateCharCreate.h"

GameEngine::GameEngine()
	: filterScaling(0, 0, this), filterTranslation(0, 0, &filterScaling)
{
	//currentState = nullptr;

	stateMenu = new StateMenu(this);
	stateCharCreate = new StateCharCreate(this);

	currentState = stateMenu;
}

void GameEngine::setState(BaseState* state)
{
	currentState->onStateExit();

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