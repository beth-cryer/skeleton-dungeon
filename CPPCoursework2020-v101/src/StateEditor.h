#pragma once
#include "BaseState.h"

class StateEditor :
	public BaseState
{
public:
	StateEditor(GameEngine* pEngine);

	virtual void onStateEnter() override;
	virtual void onStateExit() override;

	virtual void virtSetupBackgroundBuffer() override;
	virtual void virtDrawStringsOnTop() override;
	virtual void virtMouseDown(int iButton, int iX, int iY) override;
	virtual void virtMouseWheel(int x, int y, int which, int timestamp) override;
	virtual void virtKeyDown(int iKeyCode) override;
	virtual void virtMainLoopPreUpdate() override;
	virtual void virtMainLoopPostUpdate() override;

private:
	Psybc5TileManager* tileLayer; //points to the currently-active tile manager
	int tileId = 0;

	SimpleImage grid = ImageManager::loadImage("sprites/grid.png",true);

	//Gonna have our own tile managers for this state. They get wiped every time you exit the state, so you have to Save them to a text file
	SolidTileManager tilesSolid;
	BackgroundTileManager tilesBack;
	EditorTileManager tilesEditor;

};

