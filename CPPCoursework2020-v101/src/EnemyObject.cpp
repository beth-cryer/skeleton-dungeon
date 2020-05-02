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

void EnemyObject::virtDoUpdate(int iCurrentTime)
{
	//Query AI if movement is finished
	if (currentState != CharState::stateIdle && objMovement.hasMovementFinished(iCurrentTime)) {

		//Redo order of depth for charobjects by height
		((GameEngine*)getEngine())->orderCharsByHeight();

		currentState = CharState::stateIdle;
		AI();
	}
}

void EnemyObject::turnStart()
{
	//Enemy Turn Sound
	//((Psybc5Engine*)getEngine())->audio.playAudio("sfx/monsters/Growl.ogg", -1, 0);

	//Generate path to desired location
	PlayerObject* player = ((GameEngine*)getEngine())->GetPlayer();
	path = calcPath(player->getXPos(), player->getYPos());

	AI();
}

//ALL DEFAULT AI BEHAVIOUR HERE
//will attempt to move into attack range of player and then attack
void EnemyObject::AI()
{

	//turn is over once stamina is used
	if (stamina <= 0) {
		//signal to Engine that it's the next enemy's turn
	
		return;
	}

	//Otherwise,

	//If within range of weapon, attack
	PlayerObject* player = ((GameEngine*)getEngine())->GetPlayer();
	if (lineOfSight(m_iCurrentScreenX, m_iCurrentScreenY, player->getXPos(), player->getYPos(), 0)) {
		attack();
	
	//Otherwise, move towards player
	} else {
		Node* nextMove = path.front();
		path.pop_front();

		//Initiate movement
		move(m_iCurrentScreenX - nextMove->x, m_iCurrentScreenY - nextMove->y, getEngine()->getModifiedTime(), 400);
		currentState = CharState::stateWalk;
	}


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
		children.push(new Node(x + TILE_SIZE, y, g + TILE_SIZE, 0, lowest));
		children.push(new Node(x - TILE_SIZE, y, g + TILE_SIZE, 0, lowest));
		children.push(new Node(x, y + TILE_SIZE, g + TILE_SIZE, 0, lowest));
		children.push(new Node(x, y - TILE_SIZE, g + TILE_SIZE, 0, lowest));

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
			SolidTileManager* tiles = ((GameEngine*)getEngine())->GetTilesSolid();
			if (tiles->isValidTilePosition(x, y)) {
				if (tiles->getMapValue(x, y) != 0) continue;
			}

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
	//lineOfSight(64, 64, 256, 192, 3);

	//DEAD
	if (health <= 0) {
		std::cout << "Enemy " << name << " was killed.\n";
		getEngine()->removeDisplayableObject(this);
		delete this;
	}
}

//Override this where necessary
void EnemyObject::attack()
{
	((GameEngine*)getEngine())->health--;
}