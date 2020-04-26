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
	public CharObject
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

protected:
	std::string name, desc;
	int health, maxHealth;
	int stamina, maxStamina;
	int expDrop;

	//Weapon used
	Weapon wep;

};