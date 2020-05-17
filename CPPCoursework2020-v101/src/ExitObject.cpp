#include "header.h"
#include "ExitObject.h"

#include "GameEngine.h"
#include "PlayerObject.h"

void ExitObject::virtDraw()
{
	img.renderImageWithMask(getEngine()->getBackgroundSurface(),0,0,m_iCurrentScreenX,m_iCurrentScreenY,m_iDrawWidth,m_iDrawHeight,0xFF00FF);
}

void ExitObject::virtDoUpdate(int iCurrentTime)
{

}

//Click to move to next floor
void ExitObject::virtMouseDown(int iButton, int iX, int iY)
{
	if (iButton == SDL_BUTTON_RIGHT && virtIsPositionWithinObject(iX, iY)) {

		auto player = pEngine->GetPlayer();

		//If standing next to exit, can right-click to enter
		if (locks <= 0 && std::abs(m_iCurrentScreenX - player->getXPos()) + std::abs(m_iCurrentScreenY - player->getYPos()) <= TILE_SIZE) {

			//Next floor...in theory
			//(Just ending the game for now, don't have time to fix memory problems lol)
			pEngine->setExitWithCode(0);

		}

	}
}