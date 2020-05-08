#pragma once
#include "EnemyObject.h"

class EnemyCultist :
	public EnemyObject
{
public:
	EnemyCultist(BaseEngine* pEngine, int xStart, int yStart);
	~EnemyCultist();

	virtual void turnStart() override;

	void virtDraw() override;
};

