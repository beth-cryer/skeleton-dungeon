#include "header.h"
#include "CharObject.h"

#include <list>

CharObject::CharObject(int xStart, int yStart, BaseEngine* pEngine, int width, int height, bool topleft, std::shared_ptr<Weapon> wep)
	: AnimatedObject(xStart, yStart, pEngine, width, height, topleft),
	currentState(CharState::stateIdle), pEngine((GameEngine*)pEngine), wep(wep)
{
	//Setting this to false, because when the program ends we want to first remove all objects in the container and then delete any remaining objects bound to Rooms
	//If I don't control the order the deletes happen, it messes things up :(
	setShouldDeleteOnRemoval(false);
}

CharObject::~CharObject() {}

void CharObject::virtDraw()
{
	//Show that tile is occupied by a character
	//getEngine()->drawForegroundRectangle(m_iCurrentScreenX,m_iCurrentScreenY, m_iCurrentScreenX+getDrawWidth(), m_iCurrentScreenY+getDrawHeight(), 0);
}

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


void CharObject::onProjectileHit(CharObject* target)
{
}

//Using Bresenham's Line Algorithm to find the list of tiles between two points, then returning false if any of those tiles are solid
//Also returns false if there are too many tiles between the points (ie. out of the given range)
bool CharObject::lineOfSight(int x0, int y0, int x1, int y1, const int range) {

	//Translating between actual coords and tile numbers. just doing it with the actual coords didn't work before :/
	x0 /= TILE_SIZE; y0 /= TILE_SIZE;
	x1 /= TILE_SIZE; y1 /= TILE_SIZE;

	std::list<std::tuple<int, int>> los;

	//Using the integer-only operations version. Should work regardless of whether x0 > x1 or anything like that, unlike my last attempt
	int dx = abs(x1 - x0);
	int sx; if (x0 < x1) sx = 1; else sx = -1;

	int dy = -abs(y1 - y0);
	int sy; if (y0 < y1) sy = 1; else sy = -1;

	int err = dx + dy, e2;

	while(true) {
		//Add this coord to the list (and multiply by tile size to get actual coords)
		los.push_back(std::make_tuple(x0 * TILE_SIZE, y0 * TILE_SIZE));

		//std::cout << "(" << x0 * TILE_SIZE << "," << y0 * TILE_SIZE << ")\n";

		//Reached target
		if (x0 == x1 && y0 == y1) break;

		//Check if it's time to move on the slope yet
		e2 = 2 * err;
		if (e2 >= dy) {
			err += dy;
			x0 += sx;
		}

		if (e2 <= dx) {
			err += dx;
			y0 += sy;
		}
	}

	//LIST COMPLETE: now we check its elements:

	//Check length of line, return false if greater than the range
	//(range ignored if set to zero or less)
	if (range == 0 || los.size() > range) {
		//std::cout << "Target out of range\n";
		return false;
	}

	//Check if any tiles are solid, return false if any found
	for (auto it = los.begin(); it != los.end(); it++) {
		auto tiles = pEngine->GetTilesSolid();
		int x = std::get<0>(*it);
		int y = std::get<1>(*it);

		if (tiles->isValidTilePosition(x, y)) {
			if (tiles->getMapValue(tiles->getMapXForScreenX(x), tiles->getMapYForScreenY(y)) != 0) {
				//std::cout << "Line of sight blocked\n";
				return false;
			}
		}
	}

	return true;
}