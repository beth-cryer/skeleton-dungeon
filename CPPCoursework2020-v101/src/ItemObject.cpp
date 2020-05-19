#include "header.h"
#include "ItemObject.h"

#include "Item.h"
#include "GameEngine.h"
#include "FloorManager.h"


void ItemObject::virtDraw()
{
	//Draw item in this position
	int iconVal = item->iconId;
	invSprites.renderImageWithMask(pEngine->getForegroundSurface(), TILE_SIZE * (iconVal % 21), TILE_SIZE * (int)std::floor(iconVal / 20), m_iCurrentScreenX, m_iCurrentScreenX, TILE_SIZE, TILE_SIZE, 0xFF00FF);

	redrawDisplay();
}

void ItemObject::virtDoUpdate(int iCurrentTime)
{
	//Delete outside of click event
	if (picked_up) {
		pEngine->currentRoom->objects.remove(this); //remove from room's object list
		pEngine->drawableObjectsChanged();
		pEngine->removeDisplayableObject(this); //remove from main object list
		delete this; //now it's safe to delete
	}
}

void ItemObject::virtMouseDown(int iButton, int iX, int iY)
{
	//Pick up on right-click
	if (iButton == SDL_BUTTON_RIGHT)
	{
		//mouse must be over this item
		if (virtIsPositionWithinObject(iX, iY)) {
			auto inv = pEngine->GetTilesInv();

			//If there's room in the inventory, pick this up
			if (inv->addItem(item)) {
				picked_up = true; //set to delete
			}
		}

	}
}