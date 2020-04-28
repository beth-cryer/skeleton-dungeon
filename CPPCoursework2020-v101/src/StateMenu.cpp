#include "header.h"
#include "StateMenu.h"
#include <ctype.h>

StateMenu::StateMenu(GameEngine* pEngine) : BaseState(pEngine)
{
	fntTitle = pEngine->getFont("fonts/endgame.otf",45);
	fntButtons = pEngine->getFont("fonts/gameplay.ttf", 25);

	//Create UI buttons
	buttons.push_back(new ButtonNewGame(pEngine,550,250,160,40,0x2159ff,0xd4e4ff,"New Game",fntButtons));
}

void StateMenu::virtSetupBackgroundBuffer()
{
	bg.renderImageApplyingMapping(pEngine, pEngine->getBackgroundSurface(), 0, 0, bg.getWidth(), bg.getHeight(),this);
	skeleton.renderImageWithMask(pEngine->getBackgroundSurface(), 0, 0, -20, WIN_HEIGHT-skeleton.getHeight(), skeleton.getWidth(), skeleton.getHeight(), 0xFF00FF);
}

void StateMenu::virtDrawStringsOnTop()
{
	BaseState::virtDrawStringsOnTop();

	pEngine->drawForegroundString(375, 100, "SKELETON SAVES THE DUNGEON", 0xffffff, fntTitle);

	pEngine->drawForegroundString(550, 300, "Continue", 0xffffff, fntButtons);
	pEngine->drawForegroundString(550, 350, "Level Editor", 0xffffff, fntButtons);
	pEngine->drawForegroundString(550, 400, "Exit", 0xffffff, fntButtons);

}

void StateMenu::virtDrawStringsUnderneath()
{
	
}

void StateMenu::virtMouseDown(int iButton, int iX, int iY)
{
	BaseState::virtMouseDown(iButton, iX, iY);
}

void StateMenu::virtMouseWheel(int x, int y, int which, int timestamp)
{

}

void StateMenu::virtKeyDown(int iKeyCode)
{
	
}

void StateMenu::virtMainLoopPreUpdate()
{

}

void StateMenu::virtMainLoopPostUpdate()
{
	
}

//Scrolling background mapper
bool StateMenu::mapCoordinates(double& x, double& y, const SimpleImage& image)
{
	int iOffset = pEngine->getModifiedTime() / 10;

	if (x < 0) return false;
	if (y < 0) return false;
	if (x >= (image.getWidth() - 0.5)) return false;
	if (y >= (image.getHeight() - 0.5)) return false;

	x = ((int)(x + 0.5) + iOffset) % image.getWidth();

	return true;
}