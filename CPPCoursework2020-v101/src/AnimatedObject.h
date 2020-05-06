#pragma once
#include "DisplayableObject.h"
#include "BaseEngine.h"

#include "CoordinateMappingFlip.h"


//This class implements the animation system used in all character DisplayableObjects.
//Automates the process of cycling through a set of images at a constant rate, using defined sections of a single spritesheet image.

class AnimatedObject :
	public DisplayableObject
{
public:

	AnimatedObject(int xStart, int yStart, BaseEngine* pEngine, int width, int height, bool topleft);
	~AnimatedObject();

	void virtDraw() override;

	void animate(int numFrames, int width, int height, int xPos, int yPos, int xOffset = 0, int yOffset = 0);
	void resetAnim();

protected:
	int anim_frame = 0;
	int last_frame = 0;
	bool anim_end = false;

	SimpleImage imgSprites;
	CoordinateMappingFlipSheet mapFlipX;
	bool flipX = false;

};

