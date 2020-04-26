#include "header.h"
#include "BaseEngine.h"
#include "FireballObject.h"

//REQ 5. Automated moving object that subclasses DisplayableObject
FireballObject::FireballObject(BaseEngine* pEngine, Psybc5TileManager* pTileManager)
	: DisplayableObject(100,100, pEngine, 36, 36, true),
	tileManager(pTileManager)
{
	setShouldDeleteOnRemoval(true); //Same as default
}

FireballObject::~FireballObject()
{
}

//Takes the required parameters and calculates a new movement path using the objMovement object
void FireballObject::setMovement(int iStartTime, int iEndTime, int iCurrentTime,
	int iStartX, int iStartY, int iEndX, int iEndY)
{
	objMovement.setup(iStartX, iStartY, iEndX, iEndY, iStartTime, iEndTime);
	objMovement.calculate(iCurrentTime);
	m_iCurrentScreenX = objMovement.getX();
	m_iCurrentScreenY = objMovement.getY();

	originX = m_iCurrentScreenX;
	originY = m_iCurrentScreenY;
	endX = iEndX;
	endY = iEndY;
	time = iEndTime - iStartTime;
}

//DRAW
void FireballObject::virtDraw()
{
	imgSprites.renderImageWithMask(getEngine()->getForegroundSurface(), anim_frame * 36, 0, m_iCurrentScreenX, m_iCurrentScreenY, 36, 36, 0xFF00FF);

	//Check if enough gameticks have passed since last frame
	if (getEngine()->getModifiedTime() - last_frame >= 120) {
		anim_frame = (anim_frame + 1) % 4;
		last_frame = getEngine()->getModifiedTime(); //update "last frame change" variable
	}

}

//UPDATE
void FireballObject::virtDoUpdate(int iCurrentTime)
{
	//Don't do update if invisible or paused
	if (!isVisible() || getEngine()->isPaused())
		return;

	//Calculate current position
	objMovement.calculate(iCurrentTime);
	m_iCurrentScreenX = objMovement.getX();
	m_iCurrentScreenY = objMovement.getY();

	//If movement has reached end point, reset movement
	if (objMovement.hasMovementFinished(iCurrentTime)) {
		m_iCurrentScreenX = originX;
		m_iCurrentScreenY = originY;
		objMovement.setup(originX, originY, endX, endY, getEngine()->getModifiedTime(), getEngine()->getModifiedTime() + time);
		objMovement.calculate(iCurrentTime);
	}

	//REQ 9. Tile collision effect	
	if (tileManager->isValidTilePosition(m_iCurrentScreenX, m_iCurrentScreenY)) {
		int tileX = tileManager->getMapXForScreenX(m_iCurrentScreenX);
		int tileY = tileManager->getMapYForScreenY(m_iCurrentScreenY);
		int tileCollide = tileManager->getMapValue(tileX, tileY);

		//Wood Collision
		if (tileCollide == 1) {
			tileManager->setAndRedrawMapValueAt(tileX, tileY, 2, getEngine(), getEngine()->getBackgroundSurface());
			getEngine()->lockAndSetupBackground();
			getEngine()->redrawDisplay();
		}
		
	}

	//Ensure that the object gets redrawn on the display, if something changed
	redrawDisplay();
}