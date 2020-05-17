#include "header.h"
#include "DoorObject.h"
#include "BaseEngine.h"

void DoorObject::virtDraw()
{
	//Draw tileID from sprite sheet
	tileSprites.renderImageWithMask(getEngine()->getForegroundSurface(), m_iDrawWidth * (tileID % 11), m_iDrawWidth * std::floor(tileID / 10), m_iCurrentScreenX, m_iCurrentScreenY, m_iDrawWidth, m_iDrawHeight, 0xFF00FF);
}

void DoorObject::virtDoUpdate(int iCurrentTime)
{

}

//Click to move to next room
void DoorObject::virtMouseDown(int iButton, int iX, int iY)
{
	//Get next room pointer
	auto newRoom = pRoom->rooms[doorType];

	//Call onExit on current room
	pRoom->onExit();

	//Call onEnter on new room
	newRoom->onEnter();
}