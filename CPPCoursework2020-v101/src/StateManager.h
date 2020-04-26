#pragma once
#include "BaseEngine.h"

//This class controls transitions between states
//Also responsible for holding any data that must persist between states or be stored for later use, ie. game objects, inventory, tiles, etc.

class StateManager
{
public:

	StateManager()
	{
		currentState = nullptr;
	}

	void setState(BaseEngine* state)
	{
		this->currentState = state;
	}

	BaseEngine* getState()
	{
		return currentState;
	}

private:
	BaseEngine* currentState;
};

