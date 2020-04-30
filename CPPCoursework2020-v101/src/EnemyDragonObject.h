#pragma once
#include "EnemyObject.h"

class EnemyDragonObject :
	public EnemyObject
{
public:
	EnemyDragonObject(BaseEngine* pEngine);
	~EnemyDragonObject();

	void virtDraw() override;

};
