#pragma once
#include "CharObject.h"
#include "CoordinateMappingFlip.h"

//Class can be used generically, with the image passed in constructor for different projectiles, or overridden to give it special properties eg. different flight patterns
class ProjectileObject :
	public DisplayableObject
{

public:
	ProjectileObject(BaseEngine* pEngine, CharObject* origin, CharObject* target);
	ProjectileObject(BaseEngine* pEngine, CharObject* origin, CharObject* target, const char* imagePath);
	~ProjectileObject();

	void virtDraw() override;
	void virtDoUpdate(int iCurrentTime) override;

	void setMovement(int iStartTime, int iEndTime, int iCurrentTime, int iStartX, int iStartY, int iEndX, int iEndY);

	CharObject *origin, *target;

protected:
	GameEngine* pEngine;

	SimpleImage imgSprite;
	CoordinateMappingRotateTransparent rotator;
	MovementPosition objMovement;

};

class ArrowProjectile : public ProjectileObject
{
public:
	ArrowProjectile(BaseEngine* pEngine, CharObject* origin, CharObject* target) : ProjectileObject(pEngine, origin, target)
	{
		imgSprite = ImageManager::loadImage("sprites/objects/arrow.png", true);
	}
};