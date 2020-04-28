#pragma once
#include "BaseState.h"
#include "GameEngine.h"

class StateCharCreate :
	public BaseState
{
public:
	StateCharCreate(GameEngine* pEngine);
	~StateCharCreate();

	virtual void virtSetupBackgroundBuffer() override;
	virtual void virtDrawStringsOnTop() override;
	virtual void virtDrawStringsUnderneath() override;
	virtual void virtMouseDown(int iButton, int iX, int iY) override;
	virtual void virtMouseWheel(int x, int y, int which, int timestamp) override;
	virtual void virtKeyDown(int iKeyCode) override;

private:
	SimpleImage bg = ImageManager::loadImage("images/menuBG.png", true);
	Font* fntButtons;

	std::string* charInput;

};

