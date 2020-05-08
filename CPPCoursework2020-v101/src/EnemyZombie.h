#pragma once
#include "EnemyObject.h"

class EnemyZombie :
	public EnemyObject
{
public:
	EnemyZombie(BaseEngine* pEngine, int xStart, int yStart);
	~EnemyZombie();

	virtual void turnStart() override;

	void virtDraw() override;

};

