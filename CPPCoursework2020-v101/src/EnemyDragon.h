#pragma once
#include "EnemyObject.h"

class EnemyDragon :
	public EnemyObject
{
public:
	EnemyDragon(BaseEngine* pEngine, int xStart, int yStart);
	~EnemyDragon();

	virtual void turnStart() override;

	void virtDraw() override;

};
