#include "header.h"
#include "StateMenu.h"
#include <ctype.h>

StateMenu::StateMenu(GameEngine* pEngine) : BaseState(pEngine)
{
	fntTitle = pEngine->getFont("fonts/endgame.otf",45);
	fntButtons = pEngine->getFont("fonts/gameplay.ttf", 25);

	charInput = new std::string();
}

void StateMenu::virtSetupBackgroundBuffer()
{
	bg.renderImage(pEngine->getBackgroundSurface(), 0, 0, 0, 0, bg.getWidth(), bg.getHeight());
	skeleton.renderImageWithMask(pEngine->getBackgroundSurface(), 0, 0, -20, WIN_HEIGHT-skeleton.getHeight(), skeleton.getWidth(), skeleton.getHeight(), 0xFF00FF);
}

void StateMenu::virtDrawStringsOnTop()
{
	pEngine->drawForegroundString(375, 100, "SKELETON SAVES THE DUNGEON", 0xffffff, fntTitle);

	pEngine->drawForegroundString(500, 250, "Start Game", 0xffffff, fntButtons);

	//String input
	if (charInput->length() > 0) {
		pEngine->drawForegroundString(500, 350, charInput->c_str(), 0xffffff, fntButtons);
	}

}

void StateMenu::virtDrawStringsUnderneath()
{

}

void StateMenu::virtMouseDown(int iButton, int iX, int iY)
{

}

void StateMenu::virtMouseWheel(int x, int y, int which, int timestamp)
{

}

void StateMenu::virtKeyDown(int iKeyCode)
{
	//(We're using a font where lower and upper are the same)
	int k = std::tolower(iKeyCode);

	//Allow alphabet and spacebar for input
	if (k >= 97 && k <= 122 || k == SDLK_SPACE) {
		charInput->push_back((char)k);
		pEngine->redrawDisplay();
	}

	//Backspace - delete last letter if there are any letters
	if (iKeyCode == SDLK_BACKSPACE && charInput->length() > 0) {
		charInput->pop_back();
		pEngine->redrawDisplay();
	}

}