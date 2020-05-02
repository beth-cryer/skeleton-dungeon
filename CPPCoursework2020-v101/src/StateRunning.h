#pragma once
#include "BaseState.h"

class StateRunning :
	public BaseState
{
public:
	StateRunning(GameEngine* pEngine);

	virtual void onStateEnter() override;
	virtual void onStateExit() override;

	virtual void virtSetupBackgroundBuffer() override;
	virtual void virtDrawStringsOnTop() override;
	virtual void virtDrawStringsUnderneath() override;
	virtual void virtMouseDown(int iButton, int iX, int iY) override;
	virtual void virtMouseWheel(int x, int y, int which, int timestamp) override;
	virtual void virtKeyDown(int iKeyCode) override;
	virtual void virtMainLoopPreUpdate() override;
	virtual void virtMainLoopPostUpdate() override;

	void orderCharsByHeight();

};

class StatePaused :
	public StateRunning
{
public:
	StatePaused(GameEngine* pEngine);

	virtual void onStateEnter() override;
	virtual void onStateExit() override;

	virtual void virtDrawStringsOnTop() override;
	virtual void virtMouseDown(int iButton, int iX, int iY) override;
	virtual void virtKeyDown(int iKeyCode) override;
	virtual void virtMouseWheel(int x, int y, int which, int timestamp) override;

};

