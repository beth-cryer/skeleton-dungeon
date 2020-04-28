#include "header.h"

#include "StateCharCreate.h"

StateCharCreate::StateCharCreate(GameEngine* pEngine) : BaseState(pEngine)
{
	fntButtons = pEngine->getFont("fonts/gameplay.ttf", 25);

	charInput = new std::string();

	//buttons.push_back(new ButtonNewGame(pEngine, 550, 250, 160, 40, 0x2159ff, 0xd4e4ff, "New Game", fntButtons));
}

StateCharCreate::~StateCharCreate()
{
	delete charInput;
}

void StateCharCreate::virtSetupBackgroundBuffer()
{
	bg.renderImage(pEngine->getBackgroundSurface(), 0, 0, 0, 0, bg.getWidth(), bg.getHeight());
}

void StateCharCreate::virtDrawStringsOnTop()
{
	BaseState::virtDrawStringsOnTop();

	std::string printNameInput = "Enter Character Name: ";
	printNameInput.append(*charInput);

	//Draw string input
	if (charInput->length() > 0) {
		pEngine->drawForegroundString(100, 350, printNameInput.c_str(), 0xffffff, fntButtons);
	}

}

void StateCharCreate::virtDrawStringsUnderneath()
{	

}

void StateCharCreate::virtMouseDown(int iButton, int iX, int iY)
{
	BaseState::virtMouseDown(iButton, iX, iY);
}

void StateCharCreate::virtMouseWheel(int x, int y, int which, int timestamp)
{

}

void StateCharCreate::virtKeyDown(int iKeyCode)
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