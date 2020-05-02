#include "header.h"
#include "StateMenu.h"

#include "StateStart.h"

#include <ctype.h>

//MAIN MENU

StateMenu::StateMenu(GameEngine* pEngine) : BaseState(pEngine)
{
	fntTitle = pEngine->getFont("fonts/endgame.otf",45);
	fntButtons = pEngine->getFont("fonts/gameplay.ttf", 25);
}

void StateMenu::onStateEnter()
{
	//Create UI buttons
	buttons.push_back(new ButtonNewGame(pEngine, 550, 250, 230, 40, 0x2159ff, 0xd4e4ff, "New Game", fntButtons));
	buttons.push_back(new ButtonContinue(pEngine, 550, 300, 230, 40, 0x2159ff, 0xd4e4ff, "Continue", fntButtons));
	buttons.push_back(new ButtonEditor(pEngine, 550, 350, 230, 40, 0x2159ff, 0xd4e4ff, "Level Editor", fntButtons));
	buttons.push_back(new ButtonExit(pEngine, 550, 400, 230, 40, 0x2159ff, 0xd4e4ff, "Exit", fntButtons));
}

void StateMenu::virtSetupBackgroundBuffer()
{
	bg.renderImage(pEngine->getBackgroundSurface(), 0, 0, 0, 0, bg.getWidth(), bg.getHeight());
	skeleton.renderImageWithMask(pEngine->getBackgroundSurface(), 0, 0, -20, WIN_HEIGHT-skeleton.getHeight(), skeleton.getWidth(), skeleton.getHeight(), 0xFF00FF);
}

void StateMenu::virtDrawStringsOnTop()
{
	BaseState::virtDrawStringsOnTop();

	pEngine->drawForegroundString(375, 100, "SKELETON SAVES THE DUNGEON", 0xffffff, fntTitle);

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
	//Exit program
	if (iKeyCode == SDLK_ESCAPE) pEngine->setExitWithCode(0);
}

void StateMenu::virtMainLoopDoBeforeUpdate()
{

}


//CHARACTER CREATE

StateCharCreate::StateCharCreate(GameEngine* pEngine) : StateMenu(pEngine)
{
	charInput = new std::string();
}

StateCharCreate::~StateCharCreate()
{
	delete charInput;
}

void StateCharCreate::onStateEnter()
{
	
}

void StateCharCreate::virtSetupBackgroundBuffer()
{
	StateMenu::virtSetupBackgroundBuffer();
}

void StateCharCreate::virtDrawStringsOnTop()
{
	BaseState::virtDrawStringsOnTop();

	pEngine->drawForegroundString(375, 100, "Character Creation", 0xffffff, fntTitle);

	std::string printNameInput = "Name: ";

	//Append typed characters if applicable
	if (charInput->length() > 0) printNameInput.append(*charInput);

	//Append flashing line periodically if typing activated
	if (typing && pEngine->getModifiedTime() % 10 > 5) printNameInput.append("|");

	//Draw string input
	pEngine->drawForegroundString(420, 160, printNameInput.c_str(), 0xffffff, fntButtons);

	//Draw stats
	std::string printStrength = "STR: " + std::to_string(pEngine->strength);
	std::string printRanged = "RANGE: " + std::to_string(pEngine->ranged);
	std::string printMagic = "MAG: " + std::to_string(pEngine->magic);
	std::string printDefence = "DEF: " + std::to_string(pEngine->defence);
	pEngine->drawForegroundString(420, 190 + 50, printStrength.c_str(), 0xffffff, fntButtons);
	pEngine->drawForegroundString(420, 190 + 100, printRanged.c_str(), 0xffffff, fntButtons);
	pEngine->drawForegroundString(420, 190 + 150, printMagic.c_str(), 0xffffff, fntButtons);
	pEngine->drawForegroundString(420, 190 + 200, printDefence.c_str(), 0xffffff, fntButtons);
}

void StateCharCreate::virtKeyDown(int iKeyCode)
{
	//If typing is active:
	if (typing) {

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

	//Toggle typing
	if (iKeyCode == SDLK_RETURN) typing = !typing;

	//Back to menu
	if (iKeyCode == SDLK_ESCAPE) pEngine->setState(pEngine->stateMenu);
}


void StateCharCreate::virtMouseDown(int iButton, int iX, int iY)
{
	BaseState::virtMouseDown(iButton, iX, iY);
}

void StateCharCreate::virtMainLoopDoBeforeUpdate()
{
	pEngine->lockAndSetupBackground();
	pEngine->redrawDisplay();
}