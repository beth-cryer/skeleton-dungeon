#pragma once
#include "EnemyObject.h"

class EnemyDragonObject :
	public EnemyObject
{
public:
	EnemyDragonObject(BaseEngine* pEngine, int xStart, int yStart);
	~EnemyDragonObject();

	virtual void turnStart() override;

	void virtDraw() override;

};
