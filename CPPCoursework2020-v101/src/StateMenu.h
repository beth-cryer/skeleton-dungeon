#pragma once
#include "BaseState.h"
#include "GameEngine.h"
#include "CoordinateMapping.h"
#include "FloorGenerator.h"

class StateMenu :
	public BaseState
{
public:
	StateMenu(GameEngine* pEngine);

	virtual void onStateEnter() override;
	virtual void virtSetupBackgroundBuffer() override;
	virtual void virtDrawStringsOnTop() override;
	virtual void virtDrawStringsUnderneath() override;
	virtual void virtMouseDown(int iButton, int iX, int iY) override;
	virtual void virtMouseWheel(int x, int y, int which, int timestamp) override;
	virtual void virtKeyDown(int iKeyCode) override;
	virtual void virtMainLoopDoBeforeUpdate() override;

protected:
	SimpleImage bg = ImageManager::loadImage("images/menuBG.png", true);
	SimpleImage skeleton = ImageManager::loadImage("images/skeletonBG.png", true);

	Font* fntTitle;
	Font* fntButtons;

	CoordinateMappingShiftXY scrollMap;

	FloorGenerator gentest;

};


class StateCharCreate :
	public StateMenu
{
public:
	StateCharCreate(GameEngine* pEngine);
	~StateCharCreate();

	virtual void virtSetupBackgroundBuffer() override;
	virtual void onStateEnter() override;
	virtual void virtDrawStringsOnTop() override;
	virtual void virtMouseDown(int iButton, int iX, int iY) override;
	virtual void virtKeyDown(int iKeyCode) override;
	virtual void virtMainLoopDoBeforeUpdate() override;

private:
	std::string* charInput;
	bool typing = true;

};


