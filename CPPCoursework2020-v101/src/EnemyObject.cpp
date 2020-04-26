#include "header.h"
#include "EnemyObject.h"

#include "PlayerObject.h"


EnemyObject::EnemyObject(int xStart, int yStart, BaseEngine* pEngine, int width, int height, bool topleft,
	std::string name, std::string desc, int maxHealth, int maxStamina, int expDrop)
	: CharObject(xStart, yStart, pEngine, width, height, topleft),
	name(name), desc(desc), maxHealth(maxHealth), health(maxHealth), maxStamina(maxStamina), stamina(maxStamina), expDrop(expDrop)
{
	
}

EnemyObject::~EnemyObject()
{
}

void EnemyObject::virtDoUpdate(int iCurrentTime) {

	//Query AI if movement is finished
	if (currentState != CharState::stateIdle && objMovement.hasMovementFinished(iCurrentTime)) {
		currentState = CharState::stateIdle;
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