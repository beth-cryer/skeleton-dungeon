#include "header.h"
#include "StateStart.h"

#include "StateRunning.h"

#include "PlayerObject.h"
#include "EnemyZombie.h"
#include "EnemyDragon.h"
#include "EnemyCultist.h"

#include "ExitObject.h"

#include "SaveManager.h"
#include "FloorManager.h"

StateStart::StateStart(GameEngine* pEngine) : BaseState(pEngine)
{

}

//All this state does is initialise everything for the Running state
//(aka. set up object array, generate level)
void StateStart::virtSetupBackgroundBuffer()
{
	pEngine->createObjects();

	//INIT TILE MANAGERS
	std::shared_ptr<InventoryTileManager> invTiles = pEngine->GetTilesInv();

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


	//GENERATE FLOOR
	srand(time(NULL));
	auto fman = pEngine->GetFloorManager();
	auto randomFloor = fman->genRandomFloor(pEngine->GetSaveManager());

	//Generate rooms in floor and pass to engine
	fman->genRooms(pEngine, randomFloor);
	pEngine->floor = fman->getFloor();

	//Pick a random room to start in
	Room* startRoom = nullptr;
	Room* endRoom = nullptr;

	auto floor = pEngine->floor;

	int cols = floor[0].size();
	int rows = floor.size();

	//Keep generating till we get a valid room to start in
	while (startRoom == nullptr) {
		startRoom = floor[rand() % rows][rand() % cols];
	}

	//do it again for the exit room (must be different from start)
	while (endRoom == nullptr || endRoom == startRoom) {
		endRoom = floor[rand() % rows][rand() % cols];
	}

	startRoom->entrance = true;
	endRoom->exit = true;
	endRoom->objects.push_back(new ExitObject(pEngine, endRoom, endRoom->xExit, endRoom->yExit)); //Add exit door

	startRoom->onEnter(-1);

	//Start playing music (interrupt current music)
	auto audio = pEngine->GetAudio();

	//Pick random floor music
	int m_i = (rand() % 4) + 1;

	std::string path = "music/Exploration_";
	path.append(std::to_string(m_i));
	path.append(".ogg");

	audio->playMusic(path.c_str(), -1);

	//Transition to Running
	pEngine->setState(pEngine->stateRunning);

}

void StateStart::onStateEnter()
{

}

void StateStart::onStateExit()
{

}