#pragma once
#include "EnemyObject.h"

class EnemyDragon :
	public EnemyObject
{
public:
	EnemyDragon(BaseEngine* pEngine, Room* room, int xStart, int yStart);
	~EnemyDragon();

	void virtDraw() override;

};
