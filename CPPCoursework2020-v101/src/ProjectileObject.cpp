#include "header.h"
#include "ProjectileObject.h"

ProjectileObject::ProjectileObject(BaseEngine* pEngine, const char* imagePath, CharObject* origin, CharObject* target)
	: DisplayableObject(0, 0, pEngine, 64, 64, true),
	rotator(0.0), origin(origin), target(target)
{
	imgSprite = ImageManager::loadImage("sprites/objects/arrow.png", true);

	//Get angle from origin to target
	int x1 = origin->getXCentre();
	int y1 = origin->getYCentre();
	int x2 = target->getXCentre();
	int y2 = target->getYCentre();

	int dot_product = x1 * x2 + y1 * y2;
	double L1 = std::sqrt(x1 ^ 2 * y1 ^ 2);
	double L2 = std::sqrt(x2 ^ 2 * y2 ^ 2);

	double angle = std::acos(dot_product / (L1 * L2));

	//Set rotattion mapping angle
	rotator.setRotation(angle);
}

ProjectileObject::~ProjectileObject()
{
	
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

	}
}