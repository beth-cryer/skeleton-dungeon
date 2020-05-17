#include "header.h"
#include "FloorManager.h"

#include "GameEngine.h"
#include "PlayerObject.h"
#include "EnemyZombie.h"
#include "ExitObject.h"
#include "DoorObject.h"

	//ROOM FUNCTIONS

void Room::genRoom()
{
	auto save = pEngine->GetSaveManager();

	//Randomly pick from all Room templates
	srand(time(NULL));

	save->loadFileContents("gen/rooms.txt");

	std::string content = save->getText();
	std::string i_rooms = save->getTagContents(content, "rooms"); //number of floors stored

	//std::cout << "Number of room templates found: " << i_rooms << std::endl;

	int i_rand = std::stoi(i_rooms);
	i_rand = 1 + rand() % i_rand;

	//std::cout << "Room template ID picked: " << i_rand << std::endl;

	//Get tag of random floor generated
	std::string floor_tag = "room";
	floor_tag.append(std::to_string(i_rand));

	std::string room = save->getTagContents(content, floor_tag);

	//Look in both tile tags
	std::vector<std::string> tileList = { "back","solid" };
	for (auto it = tileList.begin(); it != tileList.end(); it++) {
		auto tiles = getTileData(save, room, *it);

		//We now have a 2d vector containing the tile data inside the current tag
		//w, f, s represent each of the tilemanager types

		int cols = tiles[0].size();
		int rows = tiles.size();

		backTiles.setMapSize(cols, rows);
		solidTiles.setMapSize(cols, rows);
		specialTiles.setMapSize(cols, rows);

		//Set referenced tile manager
		Psybc5TileManager* tileManager; if (*it == "back") tileManager = &backTiles; else tileManager = &solidTiles;

		//Go through each tile and add it to the Room's corresponding tile manager
		for (int y = 0; y < rows; y++) {
			for (int x = 0; x < cols; x++) {
				//Get type of tile by the first character, and set the ID according to the second character
				switch (tiles[y][x][0]) {
				case('_'): tileManager->setMapValue(x, y, 0); //blank tile is same for solid and back, so set it on the right tileManager
				case('f'): backTiles.setMapValue(x, y, getTileID(&floorIDs, tiles[y][x][1] - '0')); // (- 0 converts char to int)
				case('w'): solidTiles.setMapValue(x, y, getTileID(&wallIDs, tiles[y][x][1] - '0'));
				case('s'): setSpecialTiles(x, y, tiles[y][x][1]); //Special tiles
				}
			}
		}

	}
	//JOB DONE
}

void Room::setSpecialTiles(int x, int y, int id)
{
	switch (id)
	{
	case(1): //ENEMY
		objects.push_back(new EnemyZombie(pEngine, x, y));
		break;

	case(2):

		break; //ENTRANCE

	case(3):
		objects.push_back(new ExitObject(pEngine, x, y));
		break; //EXIT

	//DOORS
	case(4): objects.push_back(new DoorObject(pEngine, this, 0, 30, x, y)); break; //RIGHT
	case(5): objects.push_back(new DoorObject(pEngine, this, 1, 30, x, y)); break; //DOWN
	case(6): objects.push_back(new DoorObject(pEngine, this, 2, 30, x, y)); break; //UP
	case(7): objects.push_back(new DoorObject(pEngine, this, 3, 30, x, y)); break; //LEFT

	case(8): break; //TREASURE
	}
}

int Room::getTileID(std::vector<int>* list, int n)
{
	//Make sure n within bounds of our list
	if (list->size() > n) return 1;
	if (n < 0) return 1;

	return list->at(n);
}

std::vector<std::vector<std::string>> Room::getTileData(SaveManager* save, std::string text, std::string tag)
{
	//Get list of tiles inside tag
	std::string r_template = save->getTagContents(text, tag);

	//Return list of the content inside the []
	std::vector<std::string> str = save->splitContentBetween(r_template, '[', ']');

	//Split content by comma
	std::vector<std::vector<std::string>> i_temp;
	int i = 0;
	for (auto it = str.begin(); it != str.end(); it++) {
		auto split = save->splitContentBy(*it, ',');

		//Create this row
		std::vector<std::string> row;
		for (auto sit = split.begin(); sit != split.end(); sit++) {
			row.push_back(*sit);
		}

		//Push the row
		i_temp.push_back(row);
		i++;
	}

	return i_temp;
}

void Room::onEnter() {
	//First, overwrite GameEngine tile managers with this room's managers
	//pEngine->GetTilesBack() = &backTiles;
	//pEngine->GetTilesSolid() = &solidTiles;


	//Create and fill object array in GameEngine
	pEngine->drawableObjectsChanged();

	//Destroy any existing objects
	pEngine->destroyOldObjects(true);

	pEngine->player = new PlayerObject(pEngine, std::shared_ptr<Weapon>(nullptr));
	pEngine->createObjectArray(100);
	pEngine->appendObjectToArray(pEngine->player);

	//Add all objects
	for (auto it = objects.begin(); it != objects.end(); it++) {
		pEngine->appendObjectToArray(*it);
	}

	pEngine->setAllObjectsVisible(true);
}

void Room::onExit()
{

}


	//FLOOR MANAGER FUNCTIONS
typedef std::vector<std::vector<int>> grid;

grid FloorManager::genFloor(grid floor, int sizex, int sizey, int sector)
{
	//Set generation seed (do this before every gen)
	srand(time(NULL));

	//Floor Template data:
	//0 represents areas outside the bounds of the generation shape
	//1 represents empty tiles
	//2 represents a room

	//List of dig nodes (where we will remove chunks)
	std::vector<std::tuple<int, int>> digNodes;

	bool first = true;

	//foreach (sector*sector) area, pick a random element and check if it's (sector+1) tiles from every other dig node
	//keep generating till we get a valid tile
	for (int sx = 0; sx < sizex; sx += sector) {
		for (int sy = 0; sy < sizey; sy += sector) {

			if (floor[sy][sx] == 0) continue;

			bool valid; int rx, ry;
			do {
				//Generate random position within the sector
				rx = sx + rand() % sector;
				ry = sy + rand() % sector;

				valid = true;
				for (auto it = digNodes.begin(); it != digNodes.end(); it++) { //loop through all existing dig nodes
					//Using Manhattan Distance
					int xdist = std::abs(std::get<0>(*it) - rx);
					int ydist = std::abs(std::get<1>(*it) - ry);
					int dist = xdist + ydist;

					//Making sure dig nodes are at least (sector+1) tiles apart (any smaller risks cutting paths off)
					if (dist < sector + 1) valid = false;

					//Also don't want nodes to be placed diagonal to another, can cut off passages when we expand the hole
					if (xdist == ydist) valid = false;
				}

				//Now it'll repeat until dist >= 3
			} while (!valid);

			std::cout << '(' << rx << ',' << ry << ')' << std::endl;

			//Add rx,ry to digNodes
			digNodes.push_back(std::make_tuple(rx, ry));

		}
	}

	//Once dig nodes created, make 2x2 holes in the grid (or sector-1 x sector-1)
	//growing down + right
	for (auto it = digNodes.begin(); it != digNodes.end(); it++) {
		int xstart = std::get<0>(*it);
		int ystart = std::get<1>(*it);

		for (int x = 0; x < sector - 1; x++) {
			for (int y = 0; y < sector - 1; y++) {
				//check if within vector
				if (sizey < ystart + y + 1 || sizex < xstart + x + 1) continue;

				if (floor[ystart + y][xstart + x] == 2)
					floor[ystart + y][xstart + x] = 1; //if within generation bounds, create a hole
			}
		}

	}

	//Let's just stop here and see how it do
	return floor;
}

void FloorManager::genRooms(GameEngine* pEngine, grid floorLayout)
{
	std::vector<std::vector<Room*>> floor;

	int cols = floorLayout[0].size();
	int rows = floorLayout.size();

	//We're gonna go through every tile in the floor and create a list of new Room()s, leaving them empty for now
	for (int y = 0; y < rows; y++) {
		std::vector<Room*> row;

		//Create list of Rooms for the current row
		for (int x = 0; x < cols; x++) {
			//set new Room to true if it is an allocated room tile
			if (floorLayout[y][x] == 2) {
				row.push_back(new Room(pEngine)); //Room tiles are generated automatically on construction
			}
			else {
				//else set to null
				row.push_back(nullptr);
			}
		}
		//Push row to the 2d vector
		floor.push_back(row);
	}

	//Then, iterate through the list and connect each to its adjacent rooms, removing any Door tiles that don't lead anywhere
	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {

			auto currentRoom = floor[y][x];
			//If this is a filled room:
			if (currentRoom != nullptr) {

				//Check each of the four adjacent elements to see if there is a Room pointer
				//If so, assign the appropriate room pointer in currentRoom
				if (x + 1 < cols && floor[y][x + 1] != nullptr) currentRoom->rooms[0] = floor[y][x + 1];
				if (x - 1 > 0 && floor[y][x - 1] != nullptr) currentRoom->rooms[1] = floor[y][x - 1];
				if (y + 1 < rows && floor[y + 1][x] != nullptr) currentRoom->rooms[2] = floor[y + 1][x];
				if (y - 1 > 0 && floor[y - 1][x] != nullptr) currentRoom->rooms[3] = floor[y - 1][x];
			}
		}
	}

	//Next, for each object that has a set amount in each Floor:
		//pick a valid room at random and, if it has a free corresponding Special tile available, place it there

	//Next place objects that require a set amount per Room:
		//loop through every room and place


	//Set this object's floor attribute to the newly-generated one
	this->floor = floor;
}