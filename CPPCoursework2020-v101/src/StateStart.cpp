#include "header.h"
#include "StateStart.h"
#include "PlayerObject.h"
#include "EnemyZombieObject.h"

StateStart::StateStart(GameEngine* pEngine) : BaseState(pEngine)
{

}

//All this state does is initialise everything for the Running state
//(aka. set up object array, generate level)
void StateStart::onStateEnter()
{

	//INIT OBJECT ARRAY
	pEngine->drawableObjectsChanged();

	//Destroy any existing objects
	pEngine->destroyOldObjects(true);

	pEngine->player = new PlayerObject(pEngine, pEngine->GetTilesSolid());

	pEngine->createObjectArray(3); //(need to leave one empty element at end of array)
	pEngine->appendObjectToArray(pEngine->player);
	pEngine->appendObjectToArray(new EnemyZombieObject(pEngine));

	pEngine->setAllObjectsVisible(true);


	//INIT TILE MANAGERS

	SolidTileManager* solidTiles = pEngine->GetTilesSolid();
	BackgroundTileManager* bgTiles = pEngine->GetTilesBack();
	InventoryTileManager* invTiles = pEngine->GetTilesInv();

	int w = 8, h = 6;
	solidTiles->setMapSize(w, h);
	bgTiles->setMapSize(24, 16);

	int level[6][8] =
	{
		{11,11,11,11,11,11,11},
		{11,00,00,00,00,00,11},
		{11,00,00,00,00,00,11},
		{11,00,00,00,00,00,11},
		{11,00,00,00,00,00,11},
		{11,11,00,00,00,11,11},
	};

	//Translate level data into values in a Tile Map
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			solidTiles->setMapValue(x, y, level[y][x]);
		}
	}

	//Floor example (all background tiles set to same value)
	for (int x = 0; x < 24; x++) {
		for (int y = 0; y < 16; y++) {
			bgTiles->setMapValue(x, y, 1);
		}
	}

	//Set coordinates at which to place the Tile Map (anchored at top-left corner)
	solidTiles->setTopLeftPositionOnScreen(256, 64);
	bgTiles->setTopLeftPositionOnScreen(0, 0);

	//SETUP INVENTORY
	int inv[4][4] = {
		{1,0,1,2},
		{0,3,0,0},
		{0,2,2,2},
		{0,0,1,2}
	};

	w = 4; h = 4;
	invTiles->setMapSize(w, h);
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++)
			invTiles->setMapValue(x, y, y + x);
	}

	invTiles->setTopLeftPositionOnScreen(WIN_WIDTH / 2 - (w * 64) / 2, WIN_HEIGHT / 2 - (h * 64) / 2);

}

void StateStart::onStateExit()
{

}