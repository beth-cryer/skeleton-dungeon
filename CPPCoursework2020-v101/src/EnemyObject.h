#pragma once
#include "AnimatedObject.h"

#include "MovementPosition.h"
#include "Psybc5TileManager.h"

#include "Weapon.h"

#include <queue>
#include <list>

//Storage class for use in the A* algorithm
class Node
{
public:
	Node(int x, int y, int g, int f, Node* parent) {
		this->x = x;
		this->y = y;
		this->g = g;
		this->f = f;
		this->parent = parent;
	}

	~Node() {};

	int x, y;
	int g, f;
	Node* parent;
};

//Generic Enemy class
class EnemyObject :
	public AnimatedObject
{

public:
	EnemyObject(int xStart, int yStart, BaseEngine* pEngine, int width, int height, bool topleft,
		std::string name, std::string desc, int maxHealth, int maxStamina, int expDrop);
	~EnemyObject();

	void virtDoUpdate(int iCurrentTime) override;

	void damage(int amount);
	void attack();

	//Decides what action to take next, continues until its stamina is depleted
	void AI();
	std::list<Node*> calcPath(int goalX, int goalY);
	int calcHeuristic(Node n, int goalX, int goalY);

	//TODO: Add Bresenham's Line Algorithm for line-of-sight, which controls whether enemy is aggroed
	//TODO: Add A* Algorithm to find the most efficient path to the player

	void setMovement(int iStartTime, int iEndTime, int iCurrentTime, int iStartX, int iStartY, int iEndX, int iEndY);
	void move(int xmove, int ymove, int currentTime, int time);

	bool lineOfSight(const int x1, const int y1, const int x2, const int y2, const int range);

	enum State { stateIdle, stateWalk };

protected:
	std::string name, desc;
	int health, maxHealth;
	int stamina, maxStamina;
	int expDrop;

	//Weapon used
	Weapon wep;

	MovementPosition objMovement;
	State currentState;

};