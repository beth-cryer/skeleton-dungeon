#pragma once
#include "BaseState.h"
#include "GameEngine.h"

class StateMenu :
	public BaseState
{
public:
	StateMenu(GameEngine* pEngine);

	virtual void virtSetupBackgroundBuffer() override;
	virtual void virtDrawStringsOnTop() override;
	virtual void virtDrawStringsUnderneath() override;
	virtual void virtMouseDown(int iButton, int iX, int iY) override;
	virtual void virtMouseWheel(int x, int y, int which, int timestamp) override;
	virtual void virtKeyDown(int iKeyCode) override;

protected:
	SimpleImage bg = ImageManager::loadImage("images/menuBG.png", true);
	SimpleImage skeleton = ImageManager::loadImage("images/skeletonBG.png", true);

	Font* fntTitle;
	Font* fntButtons;

	std::string *charInput;

};

