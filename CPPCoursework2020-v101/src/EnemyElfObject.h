#pragma once
#include "EnemyObject.h"

class EnemyElfObject :
	public EnemyObject
{
public:
	EnemyElfObject(BaseEngine* pEngine, int xStart, int yStart);
	~EnemyElfObject();

	virtual void turnStart() override;

	void virtDraw() override;

};

