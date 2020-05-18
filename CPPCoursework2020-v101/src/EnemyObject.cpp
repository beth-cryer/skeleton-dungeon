#include "header.h"
#include "EnemyObject.h"

#include "PlayerObject.h"
#include "StateRunning.h"
#include "FloorManager.h"


EnemyObject::EnemyObject(int xStart, int yStart, BaseEngine* pEngine, Room* room, int width, int height, bool topleft,
	std::shared_ptr<Weapon> wep, std::string name, std::string desc, int maxHealth, int maxStamina, int strength, int ranged, int expDrop, int maxAttacks,
	const char* attackSound, const char* moveSound)
	: CharObject(xStart, yStart, pEngine, width, height, topleft, wep),
	 room(room), name(name), desc(desc), maxHealth(maxHealth), health(maxHealth), maxStamina(maxStamina), stamina(maxStamina), strength(strength), ranged(ranged), expDrop(expDrop), maxAttacks(maxAttacks), attacks(maxAttacks), attackSound(attackSound), moveSound(moveSound)
{
	
}

EnemyObject::~EnemyObject()
{
}

void EnemyObject::virtDoUpdate(int iCurrentTime)
{
	//Don't do update if invisible or paused
	if (!isVisible() || getEngine()->isPaused())
		return;

	if (currentState == CharState::stateWalk) {

		objMovement.calculate(iCurrentTime);
		m_iCurrentScreenX = objMovement.getX();
		m_iCurrentScreenY = objMovement.getY();

		//Query AI if movement is finished
		if (objMovement.hasMovementFinished(iCurrentTime)) {
			currentState = CharState::stateIdle;

			//Snap to grid in case we strayed a couple pixels somehow
			m_iCurrentScreenX = std::round((double)m_iCurrentScreenX / (double)TILE_SIZE) * TILE_SIZE;
			m_iCurrentScreenY = std::round((double)m_iCurrentScreenY / (double)TILE_SIZE) * TILE_SIZE;

			//Redo order of depth for charobjects by height
			pEngine->orderCharsByHeight();

			AI();
		}
	}

	//Check if attack animation is finished
	if (currentState == CharState::stateAttack) {

		if (anim_end) {
			currentState = CharState::stateIdle;
			attacks--;

			wep->attack(this,pEngine->GetPlayer());
			//attack();
			AI();
		}
	}

	//Ensure that the objects get redrawn on the display
	redrawDisplay();

	//Check if death animation is finished
	if (currentState == CharState::stateDeath) {
		if (anim_end) {
			std::cout << "Enemy " << name << " was killed.\n";
			room->objects.remove(this); //Remove from Room container so it doesn't respawn on re-entering the room
			pEngine->drawableObjectsChanged();
			pEngine->removeDisplayableObject(this); //Then remove from the engine object array
			delete this;
		}
	}
	
}

void EnemyObject::turnStart()
{
	if (aggroed == false) pEngine->GetAudio()->playAudio(attackSound, -1, 0);

	aggroed = true;

	//Generate path to desired location
	PlayerObject* player = pEngine->GetPlayer();
	path = calcPath(player->getXPos(), player->getYPos());

	//Print path
	/*
	for (auto it = path.begin(); it != path.end(); it++) {
		std::cout << '(' << (*it)->x << ',' << (*it)->y << ')' << std::endl;
	}*/

	AI();
}

//ALL DEFAULT AI BEHAVIOUR HERE
//will attempt to move into attack range of player and then attack
void EnemyObject::AI()
{
	//If within range of weapon with attack(s) left, attack
	PlayerObject* player = pEngine->GetPlayer();

	//Using line of sight algorithm
	bool in_range = false;
	if (lineOfSight(player->getXPos(), player->getYPos(), m_iCurrentScreenX, m_iCurrentScreenY, wep->range)) in_range = true;

	if (attacks > 0 && in_range) {
		anim_frame = 0;

		//face player
		if (player->getXPos() > m_iCurrentScreenX) flipX = false; else if(player->getXPos() < m_iCurrentScreenX) flipX = true;

		if (attackSound) pEngine->GetAudio()->playAudio(attackSound, -1, 0);
		attack();

		std::cout << "Enemy " << name << " attacks!\n";
		
		currentState = CharState::stateAttack;
		return;

	}
	//Otherwise, if stamina left then move towards player (unless already next to them)
	else if (stamina > 0 && path.size() > 1 ) {

		//If in range, don't bother moving any closer. just end its turn
		if (!in_range) {

			//Initiate movement
			std::shared_ptr<Node> nextMove = path.front();
			path.pop_front();

			//Print current move
			std::cout << "Enemy " << name << " moves towards you!\n";
			//std::cout << '(' << nextMove->x << ',' << nextMove->y << ')' << std::endl;

			move((nextMove->x) - m_iCurrentScreenX, (nextMove->y) - m_iCurrentScreenY, getEngine()->getModifiedTime(), 400);

			return;
		}
	}

	//Enemies will try and stay at their max attack range
	//Enemy will attack instead of moving if possible
	//If enemy runs out of attacks while inside their range, they should just end their turn instead of moving closer

	//Turn is over if not moving or attacking

	//Reset stamina n attacks
	stamina = maxStamina;
	attacks = maxAttacks;

	//Signal to Engine that it's the next enemy's turn
	auto e = dynamic_cast<StateEnemyTurn*>(pEngine->getState());
	if (e) e->triggerNextEnemy();
	else
		std::cout << "Should be in Enemy Turn state, but we ain't. Something is terribly wrong <0__0>"; //ERROR TIME (wrong state)

	return;

}

void EnemyObject::damage(int amount)
{
	health -= amount; if (health < 0) health = 0;

	std::cout << "Enemy " << name << " took " << amount << " damage, has " << health << " health left.\n";

	//DEAD
	if (health == 0) {
		anim_frame = 0;
		currentState = CharState::stateDeath;

		//Object will be deleted at the end of the animation
	}
}

//Override this where necessary
void EnemyObject::attack()
{
	//Calculate damage
	int mod; if (wep->range > 2) mod = pEngine->ranged; else mod = pEngine->strength; //using strength or ranged?
	int dmg = strength + wep->damage - (pEngine->defence);
	if (dmg < 1) dmg = 1; //attacks always do at least 1 damage, so you can't just pump defence and tank literally everything

	std::cout << "Enemy " << name << " hits you for " << dmg << " damage!\n";

	//Deal damage
	pEngine->health -= dmg;
}

void EnemyObject::move(int xmove, int ymove, int currentTime, int time)
{
	stamina--;

	//set flipX accordingly (only change if we switch directions)
	if (xmove > 0) flipX = false; else if (xmove < 0) flipX = true;

	if (moveSound) pEngine->GetAudio()->playAudio(moveSound, -1, 0);

	CharObject::move(xmove, ymove, currentTime, time);
}

void EnemyObject::onProjectileHit(CharObject* target)
{
	attack();
}

//A* pathfinding algorithm
//Can cause little lag-spikes, but i tried to limit it by just telling them to work with what they've got after a certain number of loops
std::list<std::shared_ptr<Node>> EnemyObject::calcPath(int goalX, int goalY)
{
#define MAX_ITERATIONS 500

	int its = 0;

	//Create and initialise lists
	std::list<std::shared_ptr<Node>> open_list;
	std::list<std::shared_ptr<Node>> closed_list;

	open_list.clear();
	closed_list.clear();

	//Push starting node to open list
	open_list.push_back(std::make_shared<Node>(m_iCurrentScreenX, m_iCurrentScreenY, 0, 0, nullptr));

	//While open list not empty
	while (open_list.size() > 0) {

		//find lowest f value element in open list
		auto lowest = open_list.front();
		for (auto it = open_list.begin(); it != open_list.end(); it++) {
			auto current = *it;

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

		std::queue<std::shared_ptr<Node>> children;
		children.push(std::make_shared<Node>(x + TILE_SIZE, y, g + TILE_SIZE, 0, lowest));
		children.push(std::make_shared<Node>(x - TILE_SIZE, y, g + TILE_SIZE, 0, lowest));
		children.push(std::make_shared<Node>(x, y + TILE_SIZE, g + TILE_SIZE, 0, lowest));
		children.push(std::make_shared<Node>(x, y - TILE_SIZE, g + TILE_SIZE, 0, lowest));

		//for each successor
		while (children.size() > 0) {
			auto child = children.front();
			children.pop();

			int x = child->x;
			int y = child->y;

			//calculate sucessor f
			int f = child->g + calcHeuristic(*child, goalX, goalY);
			child->f = f;

			//if successor is goal, stop here
			//OR if we've looped a bunch and can't reach the target, in which case just return the best path we have so far
			if ((x == goalX && y == goalY) || its >= MAX_ITERATIONS) {
				//Return a list containing the full path
				std::list<std::shared_ptr<Node>> path;

				std::shared_ptr<Node> current = child;
				while (current != nullptr) {
					path.push_back(current);
					current = current->parent;
				}
				path.reverse();
				path.pop_front(); //Remove head, as it is the starting node

				return path;

			}

			//If a solid tile exist here, skip
			auto solidTiles = pEngine->GetTilesSolid();
			if (solidTiles->isValidTilePosition(x, y)) {
				if (solidTiles->getMapValue(solidTiles->getMapXForScreenX(x), solidTiles->getMapYForScreenY(y)) != 0) {
					continue;
				}
			}

			//If an empty background tile exists here, skip
			auto backTiles = pEngine->GetTilesBack();
			if (backTiles->isValidTilePosition(x, y)) {
				if (backTiles->getMapValue(backTiles->getMapXForScreenX(x), backTiles->getMapYForScreenY(y)) == 0) {
					continue;
				}
			}

			bool skip = false;

			//If a CharObject exists here, skip
			DisplayableObject* pObj;
			for (int i = 0; i < pEngine->getContentCount(); i++) {
				if ((pObj = pEngine->getDisplayableObject(i)) == NULL) continue; //skip null objects
				CharObject* pChar = dynamic_cast<CharObject*> (pObj);
				if (pChar && pChar->getXPos() == x && pChar->getYPos() == y) skip = true;
			}
			if (skip) continue;

			//if node with same <x,y> as successor exists in open_list with a lower f than successor, then skip
			for (auto it = open_list.begin(); it != open_list.end(); it++) {
				if ((*it) == child && (*it)->f < f) skip = true;
			}

			//if node with same <x,y> as successor exists in closed_list with a lower f than successor, then skip
			for (auto it = closed_list.begin(); it != closed_list.end(); it++) {
				if ((*it) == child && (*it)->f < f) skip = true;
			}

			//else add to open_list
			if (skip) continue;
			else open_list.push_back(child);

		}

		its++;

		//Will break out of an infinite loop at some point, then return the best path it has

	}

}

//Using Manhattan Distance - aka. total distance travelled using right-angles only, since no diagonal moves are permitted
int EnemyObject::calcHeuristic(Node n, int goalX, int goalY)
{
	return std::abs(n.x - goalX) + std::abs(n.y - goalY);
}