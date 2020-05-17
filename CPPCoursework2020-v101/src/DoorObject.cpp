#include "header.h"
#include "DoorObject.h"
#include "BaseEngine.h"
#include "GameEngine.h"
#include "PlayerObject.h"

#include <cstring>

void DoorObject::virtDraw()
{
	//Draw tileID from sprite sheet
	tileSprites.renderImage(pEngine->getForegroundSurface(), TILE_SIZE * (33 % 11), TILE_SIZE * std::floor(33 / 10), m_iCurrentScreenX, m_iCurrentScreenY, TILE_SIZE, TILE_SIZE);

	pEngine->drawForegroundString(m_iCurrentScreenX, m_iCurrentScreenY, std::to_string(doorType).c_str(), 0xFFFFFF, NULL);
	//pEngine->drawForegroundRectangle(m_iCurrentScreenX, m_iCurrentScreenY, m_iCurrentScreenX + 64, m_iCurrentScreenY + 64, 0xFFFFFF);
}

void DoorObject::virtDoUpdate(int iCurrentTime)
{

}

//Click to move to next room
void DoorObject::virtMouseDown(int iButton, int iX, int iY)
{
	//If right-clicked on
	if (iButton == SDL_BUTTON_RIGHT && virtIsPositionWithinObject(iX, iY)) {

		auto player = pEngine->GetPlayer();

		//If standing next to door, can right-click to enter
		if (std::abs(m_iCurrentScreenX - player->getXPos()) + std::abs(m_iCurrentScreenY - player->getYPos()) <= TILE_SIZE) {

			//Get next room pointer
			auto newRoom = pRoom->rooms[doorType];
			if (newRoom == nullptr) {
				std::cout << "Door is null";
				return;
			}

			//Call onExit on current room
			pRoom->onExit();

			//Call onEnter on new room

			//Find correct door id (opposite of this doorType)
			int newDoorType;
			switch (doorType)
			{
			case 0: newDoorType = 3; break;
			case 1: newDoorType = 2; break;
			case 2: newDoorType = 1; break;
			case 3: newDoorType = 0; break;
			default: newDoorType = 0; break;
			}

			std::cout << "Moved to new room\n";

			newRoom->onEnter(newDoorType);

		}
	}
	
}