#pragma once
#include "EnemyObject.h"

class EnemyZombieObject :
	public EnemyObject
{
public:
	EnemyZombieObject(BaseEngine* pEngine);
	~EnemyZombieObject();

	void virtDraw() override;

};

