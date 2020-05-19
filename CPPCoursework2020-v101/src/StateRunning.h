#pragma once
#include "BaseState.h"

class EnemyObject;

class StatePaused;
class StateEnemyTurn;

class StateRunning :
	public BaseState
{
public:
	StateRunning(GameEngine* pEngine);
	~StateRunning();

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

protected:
	Font* fntText;
	bool enemyTurn = false;

private:
	StatePaused* statePaused = nullptr;
	StateEnemyTurn* stateEnemyTurn = nullptr;

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
	virtual void virtMainLoopPreUpdate() override;

private:
	int mousedItemID = 0;
	int heldItemID = -1;
	SimpleImage invSprites = ImageManager::loadImage("sprites/items.png", true);

};

class StateEnemyTurn :
	public StateRunning
{
public:
	StateEnemyTurn(GameEngine* pEngine);

	virtual void onStateEnter() override;
	virtual void onStateExit() override;

	virtual void virtDrawStringsOnTop() override;
	//virtual void virtMouseDown(int iButton, int iX, int iY) override;
	virtual void virtKeyDown(int iKeyCode) override;
	//virtual void virtMouseWheel(int x, int y, int which, int timestamp) override;

	void triggerNextEnemy();
	
private:
	//A list of enemies that haven't yet had their turn
	std::list<EnemyObject*> enemyTurns;

};