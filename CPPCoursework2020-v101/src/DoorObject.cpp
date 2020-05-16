#include "header.h"
#include "DoorObject.h"
#include "BaseEngine.h"

void DoorObject::virtDraw()
{
	img.renderImage(getEngine()->getBackgroundSurface(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, m_iDrawWidth, m_iDrawHeight);
}

void DoorObject::virtDoUpdate(int iCurrentTime)
{

}

//Click to move to next room
void DoorObject::virtMouseDown(int iButton, int iX, int iY)
{
	
}