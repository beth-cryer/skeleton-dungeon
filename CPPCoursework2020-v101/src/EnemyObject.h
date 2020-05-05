#pragma once
#include "CharObject.h"

#include "MovementPosition.h"

#include "Weapon.h"

#include <queue>
#include <list>

//Storage class for use in the A* algorithm
class Node
{
public:
	Node(int x, int y, int g, int f, std::shared_ptr<Node> parent)
	{
		this->x = x;
		this->y = y;
		this->g = g;
		this->f = f;
		this->parent = parent;
	}

	~Node()
	{
	};

	int x, y;
	int g, f;
	std::shared_ptr<Node> parent;
};

//Generic Enemy class
class EnemyObject :
	public CharObject
{

public:
	EnemyObject(int xStart, int yStart, BaseEngine* pEngine, int width, int height, bool topleft,
		std::string name, std::string desc, int maxHealth, int maxStamina, int expDrop, int maxAttacks);
	~EnemyObject();

	void virtDoUpdate(int iCurrentTime) override;

	void move(int xmove, int ymove, int currentTime, int time) override;
	void damage(int amount);
	virtual void attack();

	//Decides what action to take next, continues until its stamina is depleted
	virtual void turnStart();
	virtual void AI();
	std::list<std::shared_ptr<Node>> calcPath(int goalX, int goalY);
	int calcHeuristic(Node n, int goalX, int goalY);

protected:
	std::string name, desc;
	int attacks, maxAttacks;
	int health, maxHealth;
	int stamina, maxStamina;
	int strength, ranged;
	int expDrop;

	std::list<std::shared_ptr<Node>> path;

	//Weapon used
	//Weapon wep;

	//Attack Type weakness
	//WepType weak;

};