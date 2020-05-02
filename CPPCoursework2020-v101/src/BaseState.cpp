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
	//Draw Buttons
	for (auto it = buttons.begin(); it != buttons.end(); it++) {
		(*it)->drawButton(true);
	}
}

void BaseState::virtDrawStringsUnderneath()
{
}

void BaseState::virtMouseDown(int iButton, int iX, int iY)
{
	//Check Buttons for clicks
	if (iButton == SDL_BUTTON_LEFT) {
		for (auto it = buttons.begin(); it != buttons.end(); it++) {
			if ((*it)->isClicked(iX, iY)) (*it)->onClick();
		}
	}
}

void BaseState::virtMouseWheel(int x, int y, int which, int timestamp)
{
}

void BaseState::virtKeyDown(int iKeyCode)
{
}

void BaseState::virtMainLoopPreUpdate()
{
}

void BaseState::virtMainLoopPostUpdate()
{
}

void BaseState::virtMainLoopDoBeforeUpdate()
{
}

void BaseState::virtMainLoopDoAfterUpdate()
{
}