#pragma once
#include "BaseState.h"
#include "GameEngine.h"
#include "CoordinateMapping.h"
#include "FloorManager.h"

class StateInfo;
class StateCharCreate;
class StateEditor;

class StateMenu :
	public BaseState
{
public:
	StateMenu(GameEngine* pEngine);
	~StateMenu();

	virtual void onStateEnter() override;
	virtual void virtSetupBackgroundBuffer() override;
	virtual void virtDrawStringsOnTop() override;
	virtual void virtDrawStringsUnderneath() override;
	virtual void virtMouseDown(int iButton, int iX, int iY) override;
	virtual void virtMouseWheel(int x, int y, int which, int timestamp) override;
	virtual void virtKeyDown(int iKeyCode) override;
	virtual void virtMainLoopDoBeforeUpdate() override;

	StateInfo* stateInfo = nullptr;
	StateCharCreate* stateCharCreate = nullptr;
	StateEditor* stateEditor = nullptr;

protected:
	SimpleImage bg = ImageManager::loadImage("sprites/menuBG.png", true);
	SimpleImage skeleton = ImageManager::loadImage("sprites/skeletonBG.png", true);

	Font* fntTitle;
	Font* fntButtons;

	CoordinateMappingShiftXY scrollMap;

	FloorManager gentest;

};


class StateCharCreate :
	public StateMenu
{
public:
	StateCharCreate(GameEngine* pEngine);
	~StateCharCreate();

	virtual void virtSetupBackgroundBuffer() override;
	virtual void onStateEnter() override;
	virtual void onStateExit() override;
	virtual void virtDrawStringsOnTop() override;
	virtual void virtMouseDown(int iButton, int iX, int iY) override;
	virtual void virtKeyDown(int iKeyCode) override;
	virtual void virtMainLoopDoBeforeUpdate() override;


private:
	std::string charInput;
	bool typing = true;

};


class StateInfo :
	public StateMenu
{
public:
	StateInfo(GameEngine* pEngine);

	virtual void onStateEnter() override;
	virtual void virtSetupBackgroundBuffer() override;
	virtual void virtDrawStringsOnTop() override;
	virtual void virtKeyDown(int iKeyCode) override;

private:
	Font* fntInfo;

};


