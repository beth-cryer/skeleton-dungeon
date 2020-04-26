#pragma once
#include "AnimatedObject.h"

#include "MovementPosition.h"
#include "Psybc5TileManager.h"
#include "CoordinateMapping.h"
#include "Psybc5Engine.h"

class PlayerObject :
	public AnimatedObject
{

public:
	PlayerObject(BaseEngine* pEngine, Psybc5TileManager* pTiles);
	~PlayerObject();

	void virtDraw() override;
	void virtDoUpdate(int iCurrentTime) override;

	void setMovement(int iStartTime, int iEndTime, int iCurrentTime, int iStartX, int iStartY, int iEndX, int iEndY);
	void move(int xmove, int ymove, int currentTime, int time);

	bool lineOfSight(const int x1, const int y1, const int x2, const int y2, const int range);

	bool adjacentTo(int x, int y, int squareSize);
	enum State { stateIdle, stateWalk };

protected:
	MovementPosition objMovement;
	State currentState;
	Psybc5TileManager* tileManager;
	CoordinateMapping* flip;

};

