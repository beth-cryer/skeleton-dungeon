#pragma once
#include "BaseEngine.h"

#include "DrawingFilters.h" 

#include "Psybc5TileManager.h"
#include "ImageManager.h"

#include "AudioPlayer.h"
#include "SaveManager.h"


//This class controls transitions between states
//Also responsible for holding any data that must persist between states or be stored for later use, ie. game objects, inventory, tiles, etc.

class BaseState;
class StateMenu;
class StateCharCreate;
class StateStart;
class StateRunning;
class StatePaused;

class PlayerObject;

class GameEngine :
	public BaseEngine
{

public:

	GameEngine();
	~GameEngine();

	void setState(BaseState* state);
	BaseState* getState();

	int virtInitialise() override;
	void virtCleanUp() override;

	//GENERIC DRAW FUNCTIONS
	virtual void virtSetupBackgroundBuffer() override;
	void virtDrawStringsOnTop() override;
	void virtDrawStringsUnderneath() override;

	//OBJECTS
	virtual int virtInitialiseObjects() override;

	//CONTROLS
	virtual void virtMouseDown(int iButton, int iX, int iY) override;
	virtual void virtMouseWheel(int x, int y, int which, int timestamp) override;
	virtual void virtKeyDown(int iKeyCode) override;

	//UPDATE FUNCTIONS
	virtual void virtMainLoopPreUpdate() override;
	virtual void virtMainLoopPostUpdate() override;
	virtual void virtMainLoopDoBeforeUpdate() override;
	virtual void virtMainLoopDoAfterUpdate() override;

	//Helper functions
	void moveCamera(int offsetXIncrement, int offsetYIncrement);
	void drawBar(int x1, int y1, int y2, int maxWidth, std::string str, int value, int maxValue, int colBar, int colBack);
	void orderCharsByHeight();

	void saveGame();

	//RETURN PRIVATE POINTERS
	SolidTileManager* GetTilesSolid() { return &objTilesSolid; }
	BackgroundTileManager* GetTilesBack() { return &objTilesBack; }
	InventoryTileManager* GetTilesInv() { return &objInvTiles; }
	AudioPlayer* GetAudio() { return &audio; }
	SaveManager* GetSaveManager() { return &saveManager; }
	PlayerObject* GetPlayer() { return player; }

	PlayerObject* player;

	//For scrolling and zooming the draw Surfaces
	FilterPointsScaling filterScaling;
	FilterPointsTranslation filterTranslation;

	//PLAYER STATS
	void resetStats();
	
	std::string playerName;
	int maxHealth, health;
	int maxStamina, stamina;
	int maxMagic, magic;
	int strength, ranged, defence;
	int exp, expNext, level, skillUps;
	int maxAttacks, attacks;

	//Made menu state public, since any state needs to be able to get back to it
	StateMenu* stateMenu;
	StateCharCreate* stateCharCreate;
	StateStart* stateStart;
	StateRunning* stateRunning;
	StatePaused* statePaused;

private:
	BaseState* currentState;

	AudioPlayer audio;

	SolidTileManager objTilesSolid;
	BackgroundTileManager objTilesBack;
	InventoryTileManager objInvTiles;

	SaveManager saveManager;

};