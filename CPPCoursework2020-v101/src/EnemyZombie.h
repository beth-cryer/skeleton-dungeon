#pragma once
#include "EnemyObject.h"

class EnemyZombie :
	public EnemyObject
{
public:
	EnemyZombie(BaseEngine* pEngine, Room* room, int xStart, int yStart);
	~EnemyZombie();

	void virtDraw() override;

};

