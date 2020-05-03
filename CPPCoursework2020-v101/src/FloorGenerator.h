#pragma once
#include "header.h"
#include <vector>
#include <list>
#include <tuple>
#include <time.h>

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
						//Making sure dig nodes are at least (sector+1) tiles apart (any smaller risks cutting paths off)
						//Using Manhattan Distance
						int dist = std::abs(std::get<0>(*it) - rx) + std::abs(std::get<1>(*it) - ry);
						if (dist < sector+2) valid = false;
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
					if (size < ystart + y + 2 || size < xstart + x + 1) continue;

					if (floor[ystart + y][xstart + x] == 2)
						floor[ystart + y][xstart + x] = 1; //if within generation bounds, create a hole
				}
			}

		}

		//Let's just stop here and see how it do
		return floor;
	}

};

