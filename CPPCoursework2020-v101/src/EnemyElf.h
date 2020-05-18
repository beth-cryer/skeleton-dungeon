#pragma once
#include "EnemyObject.h"

class EnemyElf :
	public EnemyObject
{
public:
	EnemyElf(BaseEngine* pEngine, Room* room, int xStart, int yStart);
	~EnemyElf();

	void virtDraw() override;

};

