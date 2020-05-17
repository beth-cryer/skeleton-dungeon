#pragma once
#include "TileManager.h"
#include "Item.h"

#include <ctime>

class GameEngine;

class Psybc5TileManager :
	public TileManager
{

public:
	Psybc5TileManager() : TileManager(TILE_SIZE, TILE_SIZE)
	{
	}
	~Psybc5TileManager()
	{
	}

	virtual void virtDrawTileAt(BaseEngine* pEngine, DrawingSurface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const override;

	SimpleImage& GetTileSprites() { return tileSprites; }

protected:
	//All image resources stored as smart pointers here, used repeatedly by Tile draw calls
	SimpleImage tileSprites;
};


//BACKGROUND TILES
class BackgroundTileManager :
	public Psybc5TileManager
{
public:
	BackgroundTileManager() : Psybc5TileManager()
	{
		tileSprites = ImageManager::loadImage("sprites/tiles_floor.png", true);
	}

	virtual void virtDrawTileAt(BaseEngine* pEngine, DrawingSurface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const override;

protected:
	SimpleImage chestSprites = ImageManager::loadImage("sprites/chests.png", true);

};

//NON-PASSABLE TILES
class SolidTileManager :
	public Psybc5TileManager
{
public:
	SolidTileManager() : Psybc5TileManager()
	{
		tileSprites = ImageManager::loadImage("sprites/tiles.png", true);
	}

	virtual void virtDrawTileAt(BaseEngine* pEngine, DrawingSurface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const override;

protected:

};

//EDITOR TILES (special tiles placed only in the editor and used for map generation, eg. enemy spawns, entrances and exits, etc.)
class EditorTileManager :
	public Psybc5TileManager
{
public:
	EditorTileManager() : Psybc5TileManager()
	{
		tileSprites = ImageManager::loadImage("sprites/tiles_special.png", true);
	}
	//Only used to mark special tile positions in the editor

	/* Notes on how tiles will be handled:

	0 = ENEMY: 
	1 = START: the place on this floor where the player spawns
	2 = END: the tile on the floor that leads to the next floor (may require key(s) to unlock first, depending on the floor)
	3-6 = ARROWS: transitions between rooms. once battle starts, they are locked until all enemies killed.
		if the arrow doesn't lead to an existing room, it turns into its closest Solid tile

	*/
};



//INVENTORY
//Like the other tile managers, but with some extra data for Item objects - it is the indexes in this vector that the tile IDs are actually referencing here
class InventoryTileManager :
	public TileManager
{

public:
	InventoryTileManager(GameEngine *pEngine)
		: TileManager(64, 64),
		pEngine(pEngine)
	{
		srand(time(NULL));

		for (int i = 0; i < invSize; i++) {
			std::shared_ptr<Item> item(new Item(pEngine, rand() % 5,"Test","A test item"));
			invArray.push_back(item);
		}
	}

	~InventoryTileManager(void)
	{

	}

	virtual void virtDrawTileAt(BaseEngine* pEngine, DrawingSurface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const override;

	//Get item from array
	std::shared_ptr<Item> getItemAt(int i) {
		return invArray.at(i);
	}

	//Set item in array
	void setItemAt(int i, std::shared_ptr<Item> item) {
		invArray[i] = item;
	}

private:
	GameEngine* pEngine;

	SimpleImage invSlot = ImageManager::loadImage("sprites/invSlot.png", true);
	SimpleImage invSprites = ImageManager::loadImage("sprites/items.png", true);

	int invSize = 8;

	//Vector of Smart Points to Items (Items destroyed when no longer in use)
	//Using shared_ptr since item pointers get copied all the time (they are transferred to and from from ItemObjects when picking up and dropping)
	std::vector<std::shared_ptr<Item>> invArray;

};

