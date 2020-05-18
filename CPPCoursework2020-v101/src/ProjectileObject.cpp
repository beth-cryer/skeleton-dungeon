#include "header.h"
#include "ProjectileObject.h"

ProjectileObject::ProjectileObject(BaseEngine* pEngine, CharObject* origin, CharObject* target)
	: DisplayableObject(0, 0, pEngine, 64, 64, true),
	pEngine((GameEngine*)pEngine), rotator(0.0, 0xFF00FF), origin(origin), target(target)
{
#define PI 3.14159265

	//For some reason this doesn't work :(
	//Don't have time to fix it
	CharObject* c1 = origin;
	CharObject* c2 = target;

	//Get angle from origin to target
	int x1 = c1->getXCentre();
	int y1 = c1->getYCentre();
	int x2 = c2->getXCentre();
	int y2 = c2->getYCentre();

	double dot = ((double)x1 * (double)x2) + ((double)y1 * (double)y2);
	double det = ((double)x1 * (double)y2) - ((double)y1 * (double)x2);
	double angle = std::atan2(-det, -dot) * 180 / PI;
	
	//std::cout << angle << std::endl;

	//Set rotation mapping angle
	rotator.setRotation(angle);

	//Set movement path
	setMovement(pEngine->getModifiedTime(), pEngine->getModifiedTime() + 400, pEngine->getModifiedTime(),
		origin->getXCentre(), origin->getYCentre(), target->getXCentre(), target->getYCentre());
}

//Allow an image to be set manually too
ProjectileObject::ProjectileObject(BaseEngine* pEngine, CharObject* origin, CharObject* target, const char* imagePath)
	: ProjectileObject(pEngine, origin, target)
{
	imgSprite = ImageManager::loadImage("sprites/objects/arrow.png", true);
}

ProjectileObject::~ProjectileObject()
{
	//Remove from object array on deletion
	pEngine->removeDisplayableObject(this);
}

//Takes the required parameters and calculates a new movement path using the objMovement object
void ProjectileObject::setMovement(int iStartTime, int iEndTime, int iCurrentTime, int iStartX, int iStartY, int iEndX, int iEndY)
{
	objMovement.setup(iStartX, iStartY, iEndX, iEndY, iStartTime, iEndTime);
	objMovement.calculate(iCurrentTime);
	m_iCurrentScreenX = objMovement.getX();
	m_iCurrentScreenY = objMovement.getY();
}

void ProjectileObject::virtDraw()
{
	imgSprite.setTransparencyColour(0xFF00FF);
	imgSprite.renderImageApplyingMapping(getEngine(), getEngine()->getForegroundSurface(), m_iCurrentScreenX, m_iCurrentScreenY, imgSprite.getWidth(), imgSprite.getHeight(), &rotator);
}

//We wanna let the projectile message its owner when it hits the target
void ProjectileObject::virtDoUpdate(int iCurrentTime)
{
	//Calculate current position based on movement calculator
	objMovement.calculate(iCurrentTime);
	m_iCurrentScreenX = objMovement.getX();
	m_iCurrentScreenY = objMovement.getY();

	//If movement has reached end point, exit walk state
	if (objMovement.hasMovementFinished(iCurrentTime)) {
		m_iCurrentScreenX = objMovement.getX();
		m_iCurrentScreenY = objMovement.getY();

		//Call attack() on owner, passing target as parameter
		origin->onProjectileHit(target);
		delete this;
	}
}