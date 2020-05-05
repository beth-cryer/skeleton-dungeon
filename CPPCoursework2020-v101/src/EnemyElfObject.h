#pragma once
#include "EnemyObject.h"

class EnemyElfObject :
	public EnemyObject
{
public:
	EnemyElfObject(BaseEngine* pEngine);
	~EnemyElfObject();

	virtual void turnStart() override;

	void virtDraw() override;

};

