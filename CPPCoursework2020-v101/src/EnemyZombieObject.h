#pragma once
#include "EnemyObject.h"

class EnemyZombieObject :
	public EnemyObject
{
public:
	EnemyZombieObject(BaseEngine* pEngine, int xStart, int yStart);
	~EnemyZombieObject();

	virtual void turnStart() override;

	void virtDraw() override;

};

