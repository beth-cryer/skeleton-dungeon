#pragma once
#include "BaseState.h"
#include "GameEngine.h"

class StateStart :
	public BaseState
{
public:
	StateStart(GameEngine* pEngine);

	virtual void virtSetupBackgroundBuffer() override;
	virtual void onStateEnter() override;
	virtual void onStateExit() override;

};

