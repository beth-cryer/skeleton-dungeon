#pragma once
#include "EnemyObject.h"

class EnemyElfObject :
	public EnemyObject
{
public:
	EnemyElfObject(BaseEngine* pEngine);
	~EnemyElfObject();

	void virtDraw() override;

};

