#include "header.h"
#include "EnemyObject.h"

#include "Psybc5Engine.h"


EnemyObject::EnemyObject(int xStart, int yStart, BaseEngine* pEngine, int width, int height, bool topleft,
	std::string name, std::string desc, int maxHealth, int maxStamina, int expDrop)
	: AnimatedObject(xStart, yStart, pEngine, width, height, topleft),
	name(name), desc(desc), maxHealth(maxHealth), health(maxHealth), maxStamina(maxStamina), stamina(maxStamina), expDrop(expDrop),
	currentState(stateIdle)
{
	
}

EnemyObject::~EnemyObject()
{
}

void EnemyObject::virtDoUpdate(int iCurrentTime) {

	//Query AI if movement is finished
	if (currentState != stateIdle && objMovement.hasMovementFinished(iCurrentTime)) {
		currentState = stateIdle;
		AI();
	}
}

//ALL DEFAULT AI BEHAVIOUR HERE
//will attempt to move into attack range of player and then attack
void EnemyObject::AI() {

	//code on turn start
	if (stamina == maxStamina) {
		((Psybc5Engine*)getEngine())->audio.playAudio("sfx/monsters/Growl.ogg",-1,0); //(play growl by default)

		//Generate path to desired location
		PlayerObject* player = ((Psybc5Engine*)getEngine())->GetPlayer();
		//std::list<Node*> path = calcPath(player.get);
	}

	//turn is over once stamina is used
	if (stamina <= 0) {
		//signal to Engine that it's the next enemy's turn
	
		return;
	}

	//Otherwise,

	//move closer
	/*

	if (not close to player) {
	move(...)
	
	}else{

	*/

	//attack if close enough
	//attack();

}


std::list<Node*> EnemyObject::calcPath (int goalX, int goalY)
{
#define MAX_ITERATIONS 1000

	int its = 0;

	//Create and initialise lists
	std::list<Node*> open_list;
	std::list<Node*> closed_list;

	open_list.clear();
	closed_list.clear();

	//Push starting node to open list
	open_list.push_back(new Node(m_iCurrentScreenX, m_iCurrentScreenY, 0, 0, nullptr));

	//While open list not empty
	while (open_list.size() > 0) {

		//find lowest f value element in open list
		Node* lowest = open_list.front();
		for (std::list<Node*>::iterator it = open_list.begin(); it != open_list.end(); it++) {
			Node* current = *it;

			if (current->f < lowest->f) lowest = current;
		}

		//Remove lowest node from the list
		open_list.remove(lowest);

		//Add lowest node to closed list
		closed_list.push_back(lowest);

		//generate successors of q (one for each of the four possible directions)
		int x = lowest->x;
		int y = lowest->y;
		int g = lowest->g;

		std::queue<Node*> children;
		children.push(new Node(x + 64, y, g + 1, 0, lowest));
		children.push(new Node(x - 64, y, g + 1, 0, lowest));
		children.push(new Node(x, y + 64, g + 1, 0, lowest));
		children.push(new Node(x, y - 64, g + 1, 0, lowest));

		//for each successor
		while (children.size() > 0) {
			Node* child = children.front();
			children.pop();

			int x = child->x;
			int y = child->y;

			//calculate sucessor f
			int f = child->g + calcHeuristic(*child, goalX, goalY);
			child->f = f;

			//if successor is goal, stop here
			if (x == goalX && y == goalY) {
				//Return a list containing the full path
				std::list<Node*> path;

				Node* current = child;
				while (current != nullptr) {
					path.push_back(current);
					current = current->parent;
				}
				path.reverse();
				return path;

			}

			//If a solid tile exist here, skip
			if ( ((Psybc5Engine*)getEngine())->GetTilesSolid().isValidTilePosition(x, y) ) continue;

			bool skip = false;
			//if node with same <x,y> as successor exists in open_list with a lower f than successor, then skip
			for (std::list<Node*>::iterator it = open_list.begin(); it != open_list.end(); it++) {
				if ((*it) == child && (*it)->f < f) skip = true;
			}
			
			//if node with same <x,y> as successor exists in closed_list with a lower f than successor, then skip
			for (std::list<Node*>::iterator it = closed_list.begin(); it != closed_list.end(); it++) {
				if ((*it) == child && (*it)->f < f) skip = true;
			}
			
			//else add to open_list
			if (skip) continue;
			else open_list.push_back(child);			

		}

		//Break out of an infinite loop at some point
		if (++its > MAX_ITERATIONS) break;
	
	}

}

//Using Manhattan Distance - aka. total distance travelled using right-angles only, since no diagonal moves are permitted
int EnemyObject::calcHeuristic(Node n, int goalX, int goalY)
{
	return std::abs(n.x - goalX) + std::abs(n.y - goalY);
}

void EnemyObject::damage(int amount)
{
	health -= amount;
	std::cout << "Enemy " << name << " took " << amount << " damage, has " << health << " health left.\n";

	//DEBUG: do line of sight check
	lineOfSight(64, 64, 256, 192, 3);

	//DEAD
	if (health <= 0) {
		std::cout << "Enemy " << name << " was killed.\n";
		getEngine()->removeDisplayableObject(this);
		delete this;
	}
}

//Override this where necessary
void EnemyObject::attack() {
	((Psybc5Engine*)getEngine())->health--;
}

//Most bare-bones implementation of move. Extended by PlayerObject, but it's enough for EnemyObject
void EnemyObject::move(int xmove, int ymove, int currentTime, int time)
{
	setMovement(currentTime, currentTime + time, currentTime,
		m_iCurrentScreenX, m_iCurrentScreenY, m_iCurrentScreenX + xmove, m_iCurrentScreenY + ymove);

	currentState = stateWalk;
}

//Takes the required parameters and calculates a new movement path using the objMovement object
void EnemyObject::setMovement(int iStartTime, int iEndTime, int iCurrentTime,
	int iStartX, int iStartY, int iEndX, int iEndY)
{
	objMovement.setup(iStartX, iStartY, iEndX, iEndY, iStartTime, iEndTime);
	objMovement.calculate(iCurrentTime);
	m_iCurrentScreenX = objMovement.getX();
	m_iCurrentScreenY = objMovement.getY();
}


//Using Bresenham's Line Algorithm to find the list of tiles between two points, then returning false if any of those tiles are solid
//Also returns false if there are too many tiles between the points (ie. out of the given range)
bool EnemyObject::lineOfSight(const int x1, const int y1, const int x2, const int y2, const int range) {

	std::list<std::tuple<int, int>> los;

	//Algorithm performed using only integer operations (fast and easy)
	int dy = y2 - y1;
	int dx = x2 - x1;
	int D = 2 * dy - dx;
	int y = y1;

	for (int x = x1; x <= x2; x += TILE_SIZE) {
		std::cout << "(" << x << "," << y << ")\n";

		los.push_back(std::make_tuple(x, y));

		//Increment y when it's time to slope
		if (D >= 0) {
			y += TILE_SIZE;
			D -= 2 * dx;
		}
		D += 2 * dy;
	}

	//Check length of line, return false if greater than the range
	//(range ignored if set to zero or less)
	if (range < 0 && los.size() > range) {
		std::cout << "Target out of range\n";
		return false;
	}

	//Check if any tiles are solid, return false if any found
	for (auto it = los.begin(); it != los.end(); it++) {
		if (((Psybc5Engine*)getEngine())->GetTilesSolid().isValidTilePosition(std::get<0>(*it), std::get<1>(*it))) {
			std::cout << "Line of sight blocked\n";
			return false;
		}
	}

	return true;
}