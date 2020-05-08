#pragma once
#include "CharObject.h"

#include "MovementPosition.h"
#include "Psybc5TileManager.h"

class EnemyObject;

class PlayerObject :
	public CharObject
{

public:
	PlayerObject(BaseEngine* pEngine, std::shared_ptr<Weapon> wep);
	~PlayerObject();

	void virtDraw() override;
	void virtDoUpdate(int iCurrentTime) override;
	void virtMouseDown(int iButton, int iX, int iY) override;

	void move(int xmove, int ymove, int currentTime, int time) override;
	void attack(EnemyObject* pEnemy);
	void onProjectileHit(CharObject* target) override;

	bool adjacentTo(int x, int y, int squareSize);

};

