#pragma once
#include "TileManager.h"
#include "Item.h"
#include "Weapon.h"

class Psybc5TileManager :
	public TileManager
{

public:
	Psybc5TileManager() : TileManager(64, 64)
	{
	}
	~Psybc5TileManager(void)
	{
	}

	virtual void virtDrawTileAt(BaseEngine* pEngine, DrawingSurface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const override;

protected:
	//All image resources stored as smart pointers here, used repeatedly by Tile draw calls
	SimpleImage tileSprites;
};


//BACKGROUND TILES
class BackgroundTileManager :
	public Psybc5TileManager
{
public:
	BackgroundTileManager()
	{
		tileSprites = ImageManager::loadImage("sprites/tiles_floor.png", true);
		Psybc5TileManager();
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
	SolidTileManager()
	{
		tileSprites = ImageManager::loadImage("sprites/tiles.png", true);
	}

	virtual void virtDrawTileAt(BaseEngine* pEngine, DrawingSurface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const override;

protected:

};

#pragma once
//INVENTORY
class InventoryTileManager :
	public TileManager
{

public:
	InventoryTileManager(Psybc5Engine* pEngine) : TileManager(64, 64)
	{
		for (int i = 0; i < invSize; i++) {
			invArray.push_back(new WoodSword(pEngine));
		}
	}

	~InventoryTileManager(void)
	{
	}

	virtual void virtDrawTileAt(BaseEngine* pEngine, DrawingSurface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const override;

	Item* getItemAt(int i) {
		return invArray.at(i);
	}

	void setItemAt(int i, Item* item) {
		invArray[i] = item;
	}

private:
	SimpleImage invSlot = ImageManager::loadImage("sprites/invSlot.png", true);
	SimpleImage invSprites = ImageManager::loadImage("sprites/items.png", true);

	int invSize = 8;

	//Vector of Smart Points to Items (Items destroyed when no longer in use)
	std::vector<Item*> invArray;

};

