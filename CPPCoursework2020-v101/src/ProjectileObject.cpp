#include "header.h"
#include "ProjectileObject.h"

ProjectileObject::ProjectileObject(BaseEngine* pEngine, CharObject* origin, CharObject* target)
	: DisplayableObject(0, 0, pEngine, 64, 64, true),
	pEngine((GameEngine*)pEngine), rotator(0.0, 0xFF00FF), origin(origin), target(target)
{
#define PI 3.14159265

	//Get angle from origin to target
	int x1 = origin->getXCentre();
	int y1 = origin->getYCentre();
	int x2 = target->getXCentre();
	int y2 = target->getYCentre();

	double x = std::pow(x1 - x2, 2);
	double y = std::pow(y1 - y2, 2);
	double d = std::abs(std::sqrt(x + y));

	//Get degrees in radians and convert to degrees
	double angle = std::atan2(y,x) * 180.0 / PI;

	//Set rotation mapping angle
	rotator.setRotation(angle);

	//Set movement path
	setMovement(pEngine->getModifiedTime(), pEngine->getModifiedTime() + 400, pEngine->getModifiedTime(), x1, y1, x2, y2);
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