#pragma once
#include "AnimatedObject.h"

#include "CollisionDetection.h"
#include "MovementPosition.h"
#include "GameEngine.h"

#include "Weapon.h"

//This class includes any functionality that is shared between ALL Characters on the board (including the Player and Enemies)
//Inherits directly from AnimatedObject, as all characters are animated but not all AnimatedObjects are characters (hence the separation)

class CharObject :
	public AnimatedObject
{
public:

	CharObject(int xStart, int yStart, BaseEngine* pEngine, int width, int height, bool topleft, std::shared_ptr<Weapon> wep);
	~CharObject();

	void virtDraw() override;

	int getXPos();
	int getYPos();

	void setMovement(int iStartTime, int iEndTime, int iCurrentTime, int iStartX, int iStartY, int iEndX, int iEndY);
	virtual void move(int xmove, int ymove, int currentTime, int time);

	virtual void onProjectileHit(CharObject* target);

	//used for enemy aggro as well as checking if ranged attacks will hit
	bool lineOfSight(int x1, int y1, int x2, int y2, const int range);

	enum class CharState { stateIdle, stateWalk, stateAttack, stateDeath };

protected:
	GameEngine* pEngine;

	MovementPosition objMovement;
	CharState currentState;

	//Equipped weapon
	std::shared_ptr<Weapon> wep;

};

