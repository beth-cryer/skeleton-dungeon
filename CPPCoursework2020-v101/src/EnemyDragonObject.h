#pragma once
#include "EnemyObject.h"

class EnemyDragonObject :
	public EnemyObject
{
public:
	EnemyDragonObject(BaseEngine* pEngine);
	~EnemyDragonObject();

	virtual void turnStart() override;

	void virtDraw() override;

};
