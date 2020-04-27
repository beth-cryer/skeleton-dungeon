#include "header.h"
#include "BaseState.h"
#include "GameEngine.h"
#include "DisplayableObject.h"

BaseState::BaseState(GameEngine* pEngine) : pEngine(pEngine) {}

BaseState::~BaseState()
{

}

void BaseState::onStateEnter()
{

}

void BaseState::onStateExit()
{

}

void BaseState::virtSetupBackgroundBuffer()
{

}

void BaseState::virtDrawStringsOnTop()
{

}

void BaseState::virtDrawStringsUnderneath()
{

}

void BaseState::virtMouseDown(int iButton, int iX, int iY)
{

}

void BaseState::virtMouseWheel(int x, int y, int which, int timestamp)
{

}

void BaseState::virtKeyDown(int iKeyCode)
{

}