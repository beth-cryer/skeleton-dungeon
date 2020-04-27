#include "header.h"
#include "Psybc5TileManager.h"

void Psybc5TileManager::virtDrawTileAt(BaseEngine* pEngine, DrawingSurface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const
{
	//Draws the appropriate tile sprite from the sprite sheet
	int mapVal = getMapValue(iMapX, iMapY);
	if (mapVal != 0) tileSprites.renderImageWithMask(pSurface, m_iTileWidth * (mapVal % 11), m_iTileWidth * std::floor(mapVal / 10), iStartPositionScreenX, iStartPositionScreenY, m_iTileWidth, m_iTileHeight, 0xFF00FF);

}

void BackgroundTileManager::virtDrawTileAt(BaseEngine* pEngine, DrawingSurface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const
{
	int mapVal = getMapValue(iMapX, iMapY);

	switch (mapVal) {

		//SPECIAL TILES
			//Certain tile numbers have special functionality, or are animated with their own, separate sprite sheet
	case(25): break; //trapdoor (opens when switch is hit, changing to end-of-level tile)

	//Draw regular, non-functional tiles if required
	default: Psybc5TileManager::virtDrawTileAt(pEngine, pSurface, iMapX, iMapY, iStartPositionScreenX, iStartPositionScreenY);
	}

}

void SolidTileManager::virtDrawTileAt(BaseEngine* pEngine, DrawingSurface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const
{
	int mapVal = getMapValue(iMapX, iMapY);

	switch (mapVal) {

		//SPECIAL TILES
			//Certain tile numbers have special functionality, or are animated with their own, separate sprite sheet

	//Draw regular, non-functional tiles if required
	default: Psybc5TileManager::virtDrawTileAt(pEngine, pSurface, iMapX, iMapY, iStartPositionScreenX, iStartPositionScreenY);
	}


}

void InventoryTileManager::virtDrawTileAt(BaseEngine* pEngine, DrawingSurface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const
{
	//First draws a background rectangle for each tile
	invSlot.renderImage(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, 64, 64);

	//Draws the appropriate item sprite from the sprite sheet
	//by fetching the iconId from the corresponding Item at invArray[mapVal]
	int mapVal = getMapValue(iMapX, iMapY);
	int iconVal = invArray[mapVal]->iconId;

	invSprites.renderImageWithMask(pSurface, m_iTileWidth * (iconVal % 21), m_iTileWidth * std::floor(iconVal / 20), iStartPositionScreenX, iStartPositionScreenY, m_iTileWidth, m_iTileHeight, 0xFF00FF);

}