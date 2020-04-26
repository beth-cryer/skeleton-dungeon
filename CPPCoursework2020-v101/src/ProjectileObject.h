#pragma once
#include "DisplayableObject.h"
#include "Psybc5Engine.h"
#include "MovementPosition.h"
#include "CoordinateMapping.h"

//Lets us store pointers of these objects
class DisplayableObject;
class CoordinateMapping;
class CoordinateMappingRotate;

class ProjectileObject :
	public DisplayableObject
{

public:
	ProjectileObject(BaseEngine* pEngine);
	~ProjectileObject();


	void virtDraw() override;
	//void virtDoUpdate(int iCurrentTime) override;

	void setMovement(int iStartTime, int iEndTime, int iCurrentTime, int iStartX, int iStartY, int iEndX, int iEndY);

	void animate(int numFrames, int width, int height, int xPos, int yPos);

	DisplayableObject *source, *target;

private:
	int anim_frame = 0;
	int last_frame = 0;
	SimpleImage imgSprites = ImageManager::loadImage("sprites/objects/arrow.png", true);

	MovementPosition objMovement;

protected:
	CoordinateMappingRotate* rotator;
	CoordinateMapping* m_pMapping;

};

