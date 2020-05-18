#include "header.h"
#include "ExitObject.h"

#include "GameEngine.h"
#include "PlayerObject.h"

#include "StateStart.h"

void ExitObject::virtDraw()
{
	img.renderImageWithMask(getEngine()->getBackgroundSurface(),0,0,m_iCurrentScreenX,m_iCurrentScreenY,m_iDrawWidth,m_iDrawHeight,0xFF00FF);
}

void ExitObject::virtDoUpdate(int iCurrentTime)
{
	if (activate) {
		pEngine->GetAudio()->playAudio("sfx/objects/Open2.ogg", -1, 0);
		//Generate and load next floor (delete objects first)
		pEngine->nextLevel();
	}
}

//Click to move to next floor
void ExitObject::virtMouseDown(int iButton, int iX, int iY)
{
	if (!isVisible() || pEngine->isPaused()) return;

	if (iButton == SDL_BUTTON_RIGHT && virtIsPositionWithinObject(iX, iY)) {

		auto player = pEngine->GetPlayer();

		//If standing next to exit, can right-click to enter
		if (locks <= 0 && std::abs(m_iCurrentScreenX - player->getXPos()) + std::abs(m_iCurrentScreenY - player->getYPos()) <= TILE_SIZE) {
			activate = true; //delay to next frame to avoid crashes
		}

	}
}