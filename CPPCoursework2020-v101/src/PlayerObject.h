#pragma once
#include "CharObject.h"

#include "MovementPosition.h"
#include "Psybc5TileManager.h"

class PlayerObject :
	public CharObject
{

public:
	PlayerObject(BaseEngine* pEngine, Psybc5TileManager* pTiles);
	~PlayerObject();

	void virtDraw() override;
	void virtDoUpdate(int iCurrentTime) override;

	void move(int xmove, int ymove, int currentTime, int time) override;

	bool adjacentTo(int x, int y, int squareSize);

protected:
	Psybc5TileManager* tileManager;

};

