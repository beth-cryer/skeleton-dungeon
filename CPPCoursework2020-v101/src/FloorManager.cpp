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
	std::vector<std::string> tileList = { "back", "solid" };
	for (auto it = tileList.begin(); it != tileList.end(); it++) {
		auto tiles = getTileData(save, room, *it);

		//We now have a 2d vector containing the tile data inside the current tag
		//w, f, s represent each of the tilemanager types

		int cols = tiles[0].size();
		int rows = tiles.size();

		//Set referenced tile manager
		std::shared_ptr<Psybc5TileManager> tileManager; if (*it == "back") tileManager = backTiles; else tileManager = solidTiles;
		tileManager->setMapSize(cols, rows);		

		//Go through each tile and add it to the Room's corresponding tile manager
		for (int y = 0; y < rows; y++) {
			for (int x = 0; x < cols; x++) {
				//Get type of tile by the first character, and set the ID according to the second character
				switch (tiles[y][x][0]) {
				case('_'): tileManager->setMapValue(x, y, 0); break; //blank tile is same for solid and back, so set it on the right tileManager
				case('f'): backTiles->setMapValue(x, y, getTileID(&floorIDs, tiles[y][x][1] - '0')); break; // (- 0 converts char to int)
				case('w'): solidTiles->setMapValue(x, y, getTileID(&wallIDs, tiles[y][x][1] - '0')); break;
				case('s'): setSpecialTiles(x, y, tiles[y][x][1] - '0'); break; //Special tiles
				}
			}
		}

		solidTiles->setTopLeftPositionOnScreen(0, 0);
		backTiles->setTopLeftPositionOnScreen(0, 0);
	}

	//JOB DONE
}

void Room::setSpecialTiles(int x, int y, int id)
{
	switch (id)
	{
	case(1): //ENEMY
		objects.push_back(new EnemyZombie(pEngine, this, x * TILE_SIZE, y * TILE_SIZE));
		break;

	case(2):
		xEnter = x * TILE_SIZE;
		yEnter = y * TILE_SIZE;
		break; //ENTRANCE

	case(3):
		//objects.push_back(new ExitObject(pEngine, x, y));
		xExit = x * TILE_SIZE;
		yExit = y * TILE_SIZE;
		break; //EXIT

	//DOORS
	/*
	case(4): objects.push_back(new DoorObject(pEngine, this, 0, 30, x, y)); break; //RIGHT
	case(5): objects.push_back(new DoorObject(pEngine, this, 1, 30, x, y)); break; //DOWN
	case(6): objects.push_back(new DoorObject(pEngine, this, 2, 30, x, y)); break; //UP
	case(7): objects.push_back(new DoorObject(pEngine, this, 3, 30, x, y)); break; //LEFT
	*/

	case(8): break; //TREASURE
	}
}

int Room::getTileID(std::vector<int>* list, int n)
{
	//Make sure n within bounds of our list
	if (list->size() < n) return 1;
	if (n < 0) return 1;

	return list->at(n);
}

//Fetches a 2d vector containing all of the tile values inside the given tag
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
	std::cout << "Entering Room";

	//First, overwrite GameEngine tile managers with this room's managers
	pEngine->SetTilesBack(backTiles);
	pEngine->SetTilesSolid(solidTiles);

	//Update object array in GameEngine
	pEngine->drawableObjectsChanged();

	//Destroy any existing objects
	pEngine->destroyOldObjects(true);

	pEngine->createObjectArray(100);

	//Add player to the room in starting position
	pEngine->player = new PlayerObject(pEngine, std::make_shared<WoodSword>(pEngine));
	pEngine->player->setPosition(xEnter,yEnter);
	pEngine->appendObjectToArray(pEngine->player);

	//Add all objects to the room
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

			int max_loops = 100;
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

				
				if (max_loops-- <= 0) break; //Give up eventually if something goes wrong

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


//Generates a 2d vector of Room pointers from an input grid

void FloorManager::genRooms(GameEngine* pEngine, grid floorLayout)
{
	std::vector<std::vector<std::shared_ptr<Room>>> floor;

	int cols = floorLayout[0].size();
	int rows = floorLayout.size();

	//We're gonna go through every tile in the floor and create a list of new Room()s, leaving them empty for now
	for (int y = 0; y < rows; y++) {
		std::vector<std::shared_ptr<Room>> row;

		//Create list of Rooms for the current row
		for (int x = 0; x < cols; x++) {
			//set new Room to true if it is an allocated room tile
			if (floorLayout[y][x] == 2) {
				row.push_back(std::shared_ptr<Room> (new Room(pEngine))); //Room tiles are generated automatically on construction
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


//Using genFloor in combination with the SaveManager to load and modify a randomly-selected template floor layout

std::vector<std::vector<int>> FloorManager::genRandomFloor(SaveManager* save)
{
	//Generate array of Rooms
	std::cout << "GENERATING FLOOR" << std::endl;

	//Randomly pick from all Floor layout templates
	srand(time(NULL));

	save->loadFileContents("gen/floors.txt");

	std::string content = save->getText();
	std::string i_floors = save->getTagContents(content, "floors"); //number of floors stored

	std::cout << "Number of floor templates found: " << i_floors << std::endl;

	int i_rand = std::stoi(i_floors);
	i_rand = 1 + rand() % i_rand;

	std::cout << "Floor template ID picked: " << i_rand << std::endl;

	//Get tag of random floor generated
	std::string floor_tag = "floor";
	floor_tag.append(std::to_string(i_rand));

	std::string f_template = save->getTagContents(content, floor_tag);

	//Return list of the content in []
	std::vector<std::string> str = save->splitContentBetween(f_template, '[', ']');


	//Split content by comma
	std::vector<std::vector<int>> i_temp;
	int i = 0;
	for (auto it = str.begin(); it != str.end(); it++) {
		auto split = save->splitContentBy(*it, ',');

		std::cout << "[";
		//(And convert to int along the way)
		std::vector<int> row;
		for (auto sit = split.begin(); sit != split.end(); sit++) {
			row.push_back(std::stoi(*sit));
			std::cout << *sit << ",";
		}
		std::cout << "]";

		//Push the row
		i_temp.push_back(row);

		i++;
	}

	//Expand each tile to become a 3x3
	std::vector<std::vector<int>> i_grid;
	for (auto it = i_temp.begin(); it != i_temp.end(); it++)
	{
		//For each row, repeat every element 3 times
		std::vector<int> row;
		for (auto sit = (*it).begin(); sit != (*it).end(); sit++) {
			for (int i = 0; i < 3; i++) row.push_back(*sit);
		}

		//Then repeat the modified row for the next 3 rows of our result
		for (int i = 0; i < 3; i++) {
			i_grid.push_back(row);
		}
	}

	/* Print expanded version
	std::cout << std::endl;
	for (const std::vector<int>& v : i_grid) {
		for (int x : v) std::cout << x << ',';
		std::cout << std::endl;
	}
	*/

	//Get width and height of new vector
	int cols = i_grid[0].size();
	int rows = i_grid.size();
	std::cout << "\nRows = " << rows << ", Columns = " << cols << std::endl;

	//Generate floor grid from the processed template
	std::vector<std::vector<int>> floor = genFloor(i_grid, cols, rows, 3);

	//PRINT
	for (const std::vector<int>& v : floor) {
		for (int x : v) std::cout << x << ',';
		std::cout << std::endl;
	}

	return floor;
}