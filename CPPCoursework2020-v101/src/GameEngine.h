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
class StateStart;
class StateRunning;

class PlayerObject;
class FloorManager;
class Room;

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
	void limitStats();

	//Save/Load
	void saveGame();
	void loadGame();

	//Memory management functions, called between level generations
	void nextLevel();
	void clearObjects();
	void createObjects();

	//RETURN PRIVATE POINTERS
	std::shared_ptr<SolidTileManager> GetTilesSolid() { return objTilesSolid; }
	std::shared_ptr<BackgroundTileManager> GetTilesBack() { return objTilesBack; }
	std::shared_ptr<InventoryTileManager> GetTilesInv() { return objInvTiles; }
	AudioPlayer* GetAudio() { return &audio; }
	SaveManager* GetSaveManager() { return &saveManager; }
	FloorManager* GetFloorManager() { return gen; }
	PlayerObject* GetPlayer() { return player; }

	//SETTERS
	void SetTilesSolid(std::shared_ptr<SolidTileManager> newTiles) { objTilesSolid = newTiles; }
	void SetTilesBack(std::shared_ptr<BackgroundTileManager> newTiles) { objTilesBack = newTiles; }
	void SetTilesInv(std::shared_ptr<InventoryTileManager> newTiles) { objInvTiles = newTiles; }
	void SetPlayer(PlayerObject* newPlayer) { player = newPlayer; }

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
	int currentFloor;

	//Current floor
	std::vector<std::vector<Room*>> floor;
	Room* currentRoom = nullptr;

	//Made menu state public, since any state needs to be able to get back to it
	StateMenu* stateMenu;
	StateStart* stateStart;
	StateRunning* stateRunning;

private:
	BaseState* currentState;
	AudioPlayer audio;

	PlayerObject* player = nullptr;

	std::shared_ptr<SolidTileManager> objTilesSolid;
	std::shared_ptr<BackgroundTileManager> objTilesBack;
	std::shared_ptr<InventoryTileManager> objInvTiles;

	SaveManager saveManager;
	FloorManager* gen;

};