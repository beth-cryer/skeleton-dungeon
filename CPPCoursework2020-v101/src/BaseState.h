#pragma once
#include <list>
#include "Button.h"

class GameEngine;
class DisplayableObject;

class BaseState
{

public:
	BaseState(GameEngine* pEngine);
	~BaseState();

	//These are all virtual functions that State subclasses can override, hence they're mostly empty here
	virtual void onStateEnter();
	virtual void onStateExit();
	virtual void virtSetupBackgroundBuffer();
	virtual void virtDrawStringsOnTop();
	virtual void virtDrawStringsUnderneath();
	virtual void virtMouseDown(int iButton, int iX, int iY);
	virtual void virtMouseWheel(int x, int y, int which, int timestamp);
	virtual void virtKeyDown(int iKeyCode);
	virtual void virtMainLoopPreUpdate();
	virtual void virtMainLoopPostUpdate();
	virtual void virtMainLoopDoBeforeUpdate();
	virtual void virtMainLoopDoAfterUpdate();

protected:
	GameEngine* pEngine;

	//A list that can be used to store any DisplayableObjects that are created in this state
	//Then, the individual state can choose to either leave the objects they created or destroy them on state transition
	//(eg. when going from main menu to game, delete all UI elements from the menu)
	std::list<DisplayableObject*> objects;

	//List of buttons in the state's UI
	std::list<Button*> buttons;

};

