#pragma once
#include "CharObject.h"
#include "CoordinateMapping.h"

class ProjectileObject :
	public AnimatedObject
{

public:
	ProjectileObject(BaseEngine* pEngine);
	~ProjectileObject();


	void virtDraw() override;
	//void virtDoUpdate(int iCurrentTime) override;

	void animate(int numFrames, int width, int height, int xPos, int yPos, int xOffset = 0, int yOffset = 0) override;

	CharObject *origin, *target;

protected:
	SimpleImage imgSprites = ImageManager::loadImage("sprites/objects/arrow.png", true);

	CoordinateMappingRotate rotator;

};

