#pragma once
#include "EnemyObject.h"

class EnemyCultist :
	public EnemyObject
{
public:
	EnemyCultist(BaseEngine* pEngine, Room* room, int xStart, int yStart);
	~EnemyCultist();

	void virtDraw() override;
};

