#include "header.h"
#include "ProjectileObject.h"


ProjectileObject::ProjectileObject(BaseEngine* pEngine)
	: DisplayableObject(0, 0, pEngine, 63, 73, true)
{
	setShouldDeleteOnRemoval(true); //Same as default
	last_frame = getEngine()->getModifiedTime();
}

ProjectileObject::~ProjectileObject()
{
}

//Takes the required parameters and calculates a new movement path using the objMovement object
void ProjectileObject::setMovement(int iStartTime, int iEndTime, int iCurrentTime,
	int iStartX, int iStartY, int iEndX, int iEndY)
{
	objMovement.setup(iStartX, iStartY, iEndX, iEndY, iStartTime, iEndTime);
	objMovement.calculate(iCurrentTime);
	m_iCurrentScreenX = objMovement.getX();
	m_iCurrentScreenY = objMovement.getY();
}

void ProjectileObject::virtDraw()
{
	//Handles the animation for each player state
	//Set max frames in animation depending on which animation state we're in
	animate(5, 63, 73, 0, 146);

	//Check if enough gameticks have passed since last frame
	if (getEngine()->getModifiedTime() - last_frame >= 120) {
		anim_frame++;
		last_frame = getEngine()->getModifiedTime(); //update "last frame change" variable
	}

}


void ProjectileObject::animate(int numFrames, int width, int height, int xPos, int yPos)
{
	//Resets frame counter to 0 if over maximum: triggered when animation reaches loop point OR when a new animation has less frames than the previous
	if (anim_frame > numFrames) anim_frame = 0;

	//Render the current sprite with the parameter settings
	imgSprites.renderImageWithMask(getEngine()->getForegroundSurface(), xPos + anim_frame * width, yPos, m_iCurrentScreenX, m_iCurrentScreenY, width, height, 0xFF00FF);
}