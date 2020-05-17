#include "header.h"
#include "StateStart.h"

#include "StateRunning.h"

#include "PlayerObject.h"
#include "EnemyZombie.h"
#include "EnemyDragon.h"
#include "EnemyCultist.h"

#include "SaveManager.h"
#include "FloorManager.h"

StateStart::StateStart(GameEngine* pEngine) : BaseState(pEngine)
{

}

void StateStart::virtSetupBackgroundBuffer()
{
	/*
	//INIT OBJECT ARRAY
	pEngine->drawableObjectsChanged();

	//Destroy any existing objects
	pEngine->destroyOldObjects(true);

	pEngine->player = new PlayerObject(pEngine, std::shared_ptr<Weapon>(nullptr));

	pEngine->createObjectArray(100); //(need to leave one empty element at end of array)

	pEngine->appendObjectToArray(pEngine->player);
	pEngine->appendObjectToArray(new EnemyZombie(pEngine, 320, 256));
	pEngine->appendObjectToArray(new EnemyCultist(pEngine, 640, 448));
	pEngine->appendObjectToArray(new EnemyDragon(pEngine, 448, 256));

	pEngine->setAllObjectsVisible(true);
	*/

	//INIT TILE MANAGERS

	//SolidTileManager* solidTiles = pEngine->GetTilesSolid();
	//BackgroundTileManager* bgTiles = pEngine->GetTilesBack();
	InventoryTileManager* invTiles = pEngine->GetTilesInv();

	/*
	int w = 7, h = 7;
	solidTiles->setMapSize(w, h);
	bgTiles->setMapSize(8, 8);

	std::vector<std::vector<int>> level =
	{
			{11,11,11,11,11,11,11},
			{11,00,00,00,00,00,11},
			{11,00,00,00,00,00,11},
			{11,00,00,00,00,00,11},
			{11,00,00,00,00,00,11},
			{11,11,00,00,00,11,11},
			{00,00,00,00,00,00,00}
	};

	//Translate level data into values in a Tile Map
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			solidTiles->setMapValue(x, y, level[y][x]);
		}
	}

	//Floor example (all background tiles set to same value)
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			bgTiles->setMapValue(x, y, 1);
		}
	}

	//Set coordinates at which to place the Tile Map (anchored at top-left corner)
	solidTiles->setTopLeftPositionOnScreen(0, 0);
	bgTiles->setTopLeftPositionOnScreen(0, 0);

	*/

	//SETUP INVENTORY
	int inv[4][4] = {
		{1,-1,1,2},
		{0,3,-1,-1},
		{-1,2,2,2},
		{-1,-1,1,2}
	};

	int w = 4, h = 4;
	invTiles->setMapSize(w, h);
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++)
			invTiles->setMapValue(x, y, inv[y][x]);
	}

	invTiles->setTopLeftPositionOnScreen(WIN_CENTREX - (w * 64) / 2, WIN_CENTREY + 32 - (h * 64) / 2);


	//CREATE TEST FLOOR
	/*
	std::vector<std::vector<int>> temp = {
			{2,2,2,0,0,0,0,0,0},
			{2,2,2,0,0,0,0,0,0},
			{2,2,2,0,0,0,0,0,0},
			{2,2,2,0,0,0,0,0,0},
			{2,2,2,0,0,0,0,0,0},
			{2,2,2,0,0,0,0,0,0},
			{2,2,2,2,2,2,2,2,2},
			{2,2,2,2,2,2,2,2,2},
			{2,2,2,2,2,2,2,2,2}
	};

	auto floor = pEngine->GetFloorManager()->genFloor(temp, 9, 9, 3);

	//print
	for (const std::vector<int>& v : floor) {
		for (int x : v) std::cout << x << ',';
		std::cout << std::endl;
	}

	pEngine->floor = floor;
	*/


	//GENERATE FLOOR
	auto fman = pEngine->GetFloorManager();
	auto randomFloor = fman->genRandomFloor(pEngine->GetSaveManager());

	//Generate rooms in floor and pass to engine
	fman->genRooms(pEngine, randomFloor);
	pEngine->floor = fman->getFloor();

	//Pick a random room to start in
	Room* startRoom = nullptr;
	auto floor = pEngine->floor;

	int cols = floor[0].size();
	int rows = floor.size();

	//Keep generating till we get a valid room to start in
	while (startRoom == nullptr) {
		startRoom = floor[rand() % rows][rand() % cols];
	}

	pEngine->currentRoom = startRoom;

	startRoom->onEnter();

	//Transition to Running
	pEngine->setState(pEngine->stateRunning);
}


//All this state does is initialise everything for the Running state
//(aka. set up object array, generate level)
void StateStart::onStateEnter()
{

}

void StateStart::onStateExit()
{

}