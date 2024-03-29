#include "header.h"
#include "CharObject.h"

#include <list>

CharObject::CharObject(int xStart, int yStart, BaseEngine* pEngine, int width, int height, bool topleft)
	: AnimatedObject(xStart, yStart, pEngine, width, height, topleft),
	currentState(CharState::stateIdle)
{
}

CharObject::~CharObject() {}

//Functions for returning the actual x/y positions, since the framework only gives you x/y center for some reason
int CharObject::getXPos()
{
	return m_iCurrentScreenX;
}

int CharObject::getYPos()
{
	return m_iCurrentScreenY;
}

//Most bare-bones implementation of move. Extended by PlayerObject, but it's enough for EnemyObject
void CharObject::move(int xmove, int ymove, int currentTime, int time)
{
	setMovement(currentTime, currentTime + time, currentTime,
		m_iCurrentScreenX, m_iCurrentScreenY, m_iCurrentScreenX + xmove, m_iCurrentScreenY + ymove);

	currentState = CharState::stateWalk;
}

//Takes the required parameters and calculates a new movement path using the objMovement object
void CharObject::setMovement(int iStartTime, int iEndTime, int iCurrentTime,
	int iStartX, int iStartY, int iEndX, int iEndY)
{
	objMovement.setup(iStartX, iStartY, iEndX, iEndY, iStartTime, iEndTime);
	objMovement.calculate(iCurrentTime);
	m_iCurrentScreenX = objMovement.getX();
	m_iCurrentScreenY = objMovement.getY();
}


//Using Bresenham's Line Algorithm to find the list of tiles between two points, then returning false if any of those tiles are solid
//Also returns false if there are too many tiles between the points (ie. out of the given range)
bool CharObject::lineOfSight(const int x1, const int y1, const int x2, const int y2, const int range) {

	std::list<std::tuple<int, int>> los;

	//Algorithm performed using only integer operations (fast and easy)
	int dy = y2 - y1;
	int dx = x2 - x1;
	int D = 2 * dy - dx;
	int y = y1;

	for (int x = x1; x <= x2; x += TILE_SIZE) {
		//std::cout << "(" << x << "," << y << ")\n";

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
		auto tiles = ((Psybc5Engine*)getEngine())->GetTilesSolid();

		if (tiles->isValidTilePosition(std::get<0>(*it), std::get<1>(*it))) {
			if (tiles->getMapValue(std::get<0>(*it), std::get<1>(*it)) != 0) {
				std::cout << "Line of sight blocked\n";
				return false;
			}
		}
	}

	return true;
}