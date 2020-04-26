#include "header.h"
#include "AnimatedObject.h"

#include "Psybc5Engine.h"

#include <ctime>

AnimatedObject::AnimatedObject(int xStart, int yStart, BaseEngine* pEngine, int width, int height, bool topleft)
	: DisplayableObject(xStart, yStart, pEngine, width, height, topleft) //uses parameters given by constructor - AnimatedObject constructor will be called in subclasses
{
	last_frame = pEngine->getModifiedTime();
}

AnimatedObject::~AnimatedObject()
{
}

//Subclasses override this to add their animation code, then call the parent version
void AnimatedObject::virtDraw()
{
	//Check if enough gameticks have passed since last frame
	if (getEngine()->getModifiedTime() - last_frame >= TICK_LEN) {
		anim_frame++;
		last_frame = getEngine()->getModifiedTime(); //update "last frame change" variable
	}

	//Because this system uses GetModifiedTime, the animations will pause when the pause() function is called on Psybc5Engine
}

//Takes the number of frames in an animation (zero-indexed), the width and height of each sprite, the starting positions of the animation on the spritesheet, and the offset to draw the animation at (used to center large sprites to their occupied tile)
void AnimatedObject::animate(int numFrames, int width, int height, int xPos, int yPos, int xOffset, int yOffset)
{
	//Resets frame counter to 0 if over maximum: triggered when animation reaches loop point OR if a new animation has started that has lower numFrames than the previous
	if (anim_frame > numFrames) anim_frame = 0;

	//Set background transparency
	imgSprites.setTransparencyColour(0xFF00FF);
	
	if (flipX) {
		mapFlipX.setSheetValues(xPos + anim_frame * width, yPos, width, height, 0xFF00FF);
		imgSprites.renderImageApplyingMapping(getEngine(), getEngine()->getForegroundSurface(), m_iCurrentScreenX - xOffset, m_iCurrentScreenY - yOffset, width, height, &mapFlipX);
	
	}else{
		//Render the current sprite with the parameter settings
		imgSprites.renderImage(getEngine()->getForegroundSurface(), xPos + anim_frame * width, yPos, m_iCurrentScreenX - xOffset, m_iCurrentScreenY - yOffset, width, height);
	}
}

void AnimatedObject::resetAnim()
{
	anim_frame = 0;
}