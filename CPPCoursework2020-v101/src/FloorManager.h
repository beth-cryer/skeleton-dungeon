#pragma once
#include "header.h"

#include <vector>
#include <list>
#include <tuple>
#include <time.h>

#include "GameEngine.h"

class Room
{
public:
	Room(GameEngine* pEngine, std::vector<int> wallIDs = { 1 }, std::vector<int> floorIDs = {1} )
		: pEngine(pEngine), wallIDs(wallIDs), floorIDs(floorIDs)
	{
		genRoom();
	}

	//Randomly picks a room template and sets this Room's Tile Manager objects
	//Also places Special tiles/objects like enemies and chests
	void genRoom();

	//Returns corresponding tileID from its number in the template file
	//Returns 1 by default if invalid
	int getTileID(std::vector<int>* list, int n);

	//Adds a special tile to the Room's object vector
	void setSpecialTiles(int x, int y, int id);
	
	//Fetches a 2D vector of the data inside brackets, separated by commas
	std::vector<std::vector<std::string>> getTileData(SaveManager* save, std::string text, std::string tag);

	void onEnter();

	//Remove all room objects from the engine's object array
	void onExit();

	//Used to check whether to spawn the player here on floor start, or place an exit trapdoor
	bool entrance = false;
	bool exit = false;

	//Coordinates of entrance or exit (if either are in this Room)
	int xEnter, yEnter;
	int xExit, yExit;

	//Connected rooms (right,up,down,left)
	Room* rooms[4] = { nullptr, nullptr, nullptr, nullptr };

	//List of tile IDs for this room's walls and floors
	std::vector<int> wallIDs;
	std::vector<int> floorIDs;

	//A list of objects that the room contains. Placed upon entering the room and destroyed on exit
	//(when an object in this list is destroyed, it needs to remove itself from this list first or it will be recreated when you exit and enter the room again)
	std::list<DisplayableObject*> objects;

private:
	//Pointer used to tell the engine to create/destroy objects
	GameEngine* pEngine;

	//Tile Managers
	SolidTileManager solidTiles;
	BackgroundTileManager backTiles;
	Psybc5TileManager specialTiles; //temporary, is used by FloorManager to determine where to put number-limited objects on the floor

};


//FLOOR GENERATOR/MANAGER
class FloorManager
{
public:

	FloorManager() {}
	~FloorManager() {}

	//Getter for this object's Floor structure
	std::vector<std::vector<Room*>> *getFloor() { return &floor;  }

	//Defines a 2D grid of ints
	typedef std::vector<std::vector<int>> grid;

	grid genFloor(grid floor, int sizex, int sizey, int sector);
	void genRooms(GameEngine* pEngine, grid floorLayout);

private:
	std::vector<std::vector<Room*>> floor;

};