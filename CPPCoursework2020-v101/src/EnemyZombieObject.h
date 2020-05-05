#pragma once
#include "EnemyObject.h"

class EnemyZombieObject :
	public EnemyObject
{
public:
	EnemyZombieObject(BaseEngine* pEngine);
	~EnemyZombieObject();

	virtual void turnStart() override;

	void virtDraw() override;

};

