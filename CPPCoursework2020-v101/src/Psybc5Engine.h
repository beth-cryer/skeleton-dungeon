#pragma once
#include "BaseEngine.h"

#include "DrawingFilters.h" 

#include "Psybc5TileManager.h"
#include "ImageManager.h"

#include "AudioPlayer.h"

//Lets us store pointers of these objects
class PlayerObject;

class Psybc5Engine :
	public BaseEngine
{

private:
	PlayerObject* player;

public:
	Psybc5Engine();
	~Psybc5Engine();

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

	//RETURN PRIVATE POINTERS
	SolidTileManager& GetTilesSolid() { return objTilesSolid; }
	BackgroundTileManager& GetTilesBack() { return objTilesBack; }
	InventoryTileManager& GetTilesInv() { return objInvTiles; }
	PlayerObject* GetPlayer() { return player; }

	//GAME STATES
	enum State {stateStart, stateMain, statePause, stateEnemyTurns};

	//HELPER FUNCTIONS
	void moveCamera(int offsetXIncrement, int offsetYIncrement);

	//PLAYER STATS
	int maxHealth, health;
	int maxStamina, stamina;
	int maxMagic, magic;
	int strength, ranged, defence;
	int exp, expNext, level, skillUps;
	int maxAttacks, attacks;

	AudioPlayer audio;

private:
	State currentState;
	SolidTileManager objTilesSolid;
	BackgroundTileManager objTilesBack;
	InventoryTileManager objInvTiles;
	FontManager fonts;
	std::shared_ptr<Item> heldItem = NULL; //smart pointer to the Item currently held with the cursor

	//Gets smart pointer to background image
	SimpleImage bg = ImageManager::loadImage("images/background.png", true);

	//For scrolling and zooming the draw Surfaces
	FilterPointsScaling filterScaling;
	FilterPointsTranslation filterTranslation;

};

