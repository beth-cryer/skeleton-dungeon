#pragma once
#include "header.h"
#include "PlayerObject.h"

#include <vector>
#include <list>
#include <tuple>
#include <time.h>

class Room
{
public:
	Room(GameEngine* pEngine, bool room)
		: pEngine(pEngine),
		room(room)
	{
	}

	void onEnter() {
		//First, overwrite GameEngine tile managers with this room's managers



		//Create and fill object array in GameEngine
		pEngine->drawableObjectsChanged();

		//Destroy any existing objects
		pEngine->destroyOldObjects(true);

		pEngine->player = new PlayerObject(pEngine, pEngine->GetTilesSolid());
		pEngine->createObjectArray(100);
		pEngine->appendObjectToArray(pEngine->player);

		//Add all objects
		for (auto it = objects.begin(); it != objects.end(); it++) {
			pEngine->appendObjectToArray(*it);
		}

		pEngine->setAllObjectsVisible(true);
	}

	void onExit() {

	}

	//Signifies if the room is valid or null
	bool room;

	Room* roomRight = nullptr, * roomLeft = nullptr, * roomUp = nullptr, * roomDown = nullptr;

	//A list of objects that the room contains. Placed upon entering the room and destroyed on exit
	//(when an object in this list is destroyed, it needs to remove itself from the list first or it will be recreated when you exit and enter the room again)
	std::list<DisplayableObject*> objects;

	//Pointer used to tell the engine to create/destroy objects
	GameEngine* pEngine;

};

class FloorGenerator
{
public:

	//Defines a 2D grid of ints
	typedef std::vector<std::vector<int>> grid;

	grid genFloor(int size, int sector)
	{
		//Set generation seed (do this before every gen)
		srand(time(NULL));

		//Starting floor
		//0 represents areas outside the bounds of the generation shape
		//1 represents empty tiles
		//2 represents a room

		//An example we'll use is an L-shaped space in a 9x9 grid
		//Each sector of the grid is 3x3, will have a single dig node placed in each later

		grid floor = {
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

		//List of dig nodes (where we will remove chunks)
		std::vector<std::tuple<int, int>> digNodes;

		bool first = true;

		//foreach (sector*sector) area, pick a random element and check if it's (sector+1) tiles from every other dig node
		//keep generating till we get a valid tile
		for (int sx = 0; sx < size; sx += sector) {
			for (int sy = 0; sy < size; sy += sector) {

				if (floor[sy][sx] == 0) continue;

				//Pick the central tile for first one
				/*
				if (first) {
					std::cout << '(' << sx+1 << ',' << sy+1 << ')' << std::endl;
					digNodes.push_back(std::make_tuple(sx+1, sy+1));
					first = false;
					continue;
				}
				*/

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
						if (dist < sector+1) valid = false;

						//Also don't want nodes to be placed diagonal to another, can cut off passages when we expand the hole
						if (xdist == ydist) valid = false;
					}

				//Now it'll repeat until dist >= 3
				} while (!valid);

				std::cout << '(' << rx << ',' << ry << ')' << std::endl;

				//Add rx,ry to digNodes
				digNodes.push_back(std::make_tuple(rx,ry));

			}
		}

		//Once dig nodes created, make 2x2 holes in the grid (or sector-1 x sector-1)
		//growing down + right
		for (auto it = digNodes.begin(); it != digNodes.end(); it++) {
			int xstart = std::get<0>(*it);
			int ystart = std::get<1>(*it);

			for (int x = 0; x < sector-1; x++) {
				for (int y = 0; y < sector-1; y++) {
					//check if within vector
					if (size < ystart + y + 1 || size < xstart + x + 1) continue;

					if (floor[ystart + y][xstart + x] == 2)
						floor[ystart + y][xstart + x] = 1; //if within generation bounds, create a hole
				}
			}

		}

		//Let's just stop here and see how it do
		return floor;
	}

	std::vector<Room*> genRooms(GameEngine* pEngine, grid floorLayout)
	{
		std::vector<std::vector<Room*>> floor;
		int w, h;

		//We're gonna go through every tile in the floor and create a list of new Room()s, leaving them empty for now
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				floor[y].push_back(new Room(pEngine, x == 2)); //set new Room to true if it is an allocated room tile
			}
		}

		//Then, iterate through the list and connect each to its adjacent rooms, removing any Door tiles that don't lead anywhere
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {

				auto currentRoom = floor[y][x];
				//If this is a filled room:
				if (currentRoom->room) { 
					//Check each of the four adjacent Rooms to see if room==true
					//If so, assign the appropriate room pointer in currentRoom
					if (floor[y][x + 1]->room) currentRoom->roomRight = floor[y][x + 1];
					if (floor[y][x - 1]->room) currentRoom->roomLeft = floor[y][x - 1];
					if (floor[y + 1][x]->room) currentRoom->roomDown = floor[y + 1][x];
					if (floor[y - 1][x]->room) currentRoom->roomUp = floor[y - 1][x];
				}
			}
		}

		//Next, for each object that has a set amount in each Floor:
			//pick a valid room at random and, if it has a free corresponding Special tile available, place it there

		//Next place objects that require a set amount per Room: (ie. enemies)
			//loop through every room and place

	}

};