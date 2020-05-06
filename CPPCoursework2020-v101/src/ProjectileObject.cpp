#include "header.h"
#include "ProjectileObject.h"

ProjectileObject::ProjectileObject(BaseEngine* pEngine)
	: AnimatedObject(0, 0, pEngine, 64, 64, true), rotator(0.0)
{
	//Get angle from origin to target
	int x1 = origin->getXCentre();
	int y1 = origin->getYCentre();
	int x2 = target->getXCentre();
	int y2 = target->getYCentre();

	int dot_product = x1 * x2 + y1 * y2;
	double L1 = std::sqrt(x1 ^ 2 * y1 ^ 2);
	double L2 = std::sqrt(x2 ^ 2 * y2 ^ 2);

	double angle = std::acos(dot_product / (L1 * L2));

	rotator.setRotation(angle);
}

ProjectileObject::~ProjectileObject()
{
	
}

void ProjectileObject::virtDraw()
{
	//Handles the animation for each player state
	//Set max frames in animation depending on which animation state we're in
	animate(5, 63, 73, 0, 146);

	AnimatedObject::virtDraw();

}


void ProjectileObject::animate(int numFrames, int width, int height, int xPos, int yPos, int xOffset, int yOffset)
{
	//Resets frame counter to 0 if over maximum: triggered when animation reaches loop point OR when a new animation has less frames than the previous
	if (anim_frame > numFrames) anim_frame = 0;

	//Render the current sprite with the parameter settings
	imgSprites.setTransparencyColour(0xFF00FF);
	imgSprites.renderImageApplyingMapping(getEngine(), getEngine()->getForegroundSurface(), m_iCurrentScreenX - xOffset, m_iCurrentScreenY - yOffset, width, height, &rotator);
}