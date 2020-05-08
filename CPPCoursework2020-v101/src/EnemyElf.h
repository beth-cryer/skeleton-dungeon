#pragma once
#include "EnemyObject.h"

class EnemyElf :
	public EnemyObject
{
public:
	EnemyElf(BaseEngine* pEngine, int xStart, int yStart);
	~EnemyElf();

	virtual void turnStart() override;

	void virtDraw() override;

};

