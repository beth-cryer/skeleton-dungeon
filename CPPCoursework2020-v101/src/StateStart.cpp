#include "header.h"
#include "StateStart.h"

#include "StateRunning.h"

#include "PlayerObject.h"
#include "EnemyZombie.h"
#include "EnemyDragon.h"
#include "EnemyCultist.h"

#include "SaveManager.h"
#include "FloorGenerator.h"

StateStart::StateStart(GameEngine* pEngine) : BaseState(pEngine)
{

}

void StateStart::virtSetupBackgroundBuffer()
{
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


	//INIT TILE MANAGERS

	SolidTileManager* solidTiles = pEngine->GetTilesSolid();
	BackgroundTileManager* bgTiles = pEngine->GetTilesBack();
	InventoryTileManager* invTiles = pEngine->GetTilesInv();

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

	//Transition to Running
	pEngine->setState(pEngine->stateRunning);
}


//All this state does is initialise everything for the Running state
//(aka. set up object array, generate level)
void StateStart::onStateEnter()
{

	//ROOM GENERATION CODE. WILL PROBABLY MOVE SOMEWHERE ELSE LATER
	
	/*
	//Generate array of Rooms
	auto gen = pEngine->GetFloorGenerator();
	auto save = pEngine->GetSaveManager();

	//Randomly pick from all Floor layout templates
	srand(time(NULL));

	std::string content = save->loadFileContents("gen/floors.txt");
	std::string i_floors = save->getTagContents(content,"floors"); //number of floors stored

	int i_rand = std::stoi(i_floors);
	int f = 1 + rand() % i_rand;
	
	//Get tag of random floor generated
	std::string f_template = save->getTagContents(content,"floor"+f);

	//Return list of the content in []
	std::vector<std::string> str = save->splitContentBetween(f_template,'[',']');

	//Split content by comma
	std::vector<std::vector<int>> i_template;
	int i = 0;
	for (auto it = str.begin(); it != str.end(); it++) {
		std::vector<int> row;
		auto split = save->splitContentBy(*it, ',');

		//Convert to int first
		for (auto sit = split.begin(); sit != split.end(); sit++) {
			row.push_back(std::stoi(*sit));
		}

		i_template[i++] = row;
	}

	//Get width and height of new vector
	int rows = i_template.size() / i_template[0].size();
	int cols = i_template[0].size();

	std::vector<std::vector<int>> floor = gen->genFloor(rows,cols,3);
	*/

	
}

void StateStart::onStateExit()
{

}