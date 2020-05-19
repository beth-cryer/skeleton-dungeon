#include "header.h"
#include "StateMenu.h"

#include "StateStart.h"
#include "StateEditor.h"

#include <ctype.h>

//MAIN MENU

StateMenu::StateMenu(GameEngine* pEngine)
	: BaseState(pEngine),
	scrollMap(1,0)
{
	fntTitle = pEngine->getFont("fonts/endgame.otf",45);
	fntButtons = pEngine->getFont("fonts/gameplay.ttf", 25);
}

StateMenu::~StateMenu()
{
	delete stateInfo;
	delete stateCharCreate;
	delete stateEditor;
}

void StateMenu::onStateEnter()
{
	//Create state objects if not already created
	if (stateInfo == nullptr)
	{
		stateInfo = new StateInfo(pEngine);
		stateCharCreate = new StateCharCreate(pEngine);
		stateEditor = new StateEditor(pEngine);
	}

	//Reset inventory to default
	pEngine->GetTilesInv()->setInvDefault();

	//Create UI buttons
	buttons.clear();
	buttons.push_back(std::unique_ptr<Button>(new ButtonChangeState(pEngine, stateCharCreate, 550, 250, 230, 40, 0x2159ff, 0xd4e4ff, "New Game", fntButtons)));
	buttons.push_back(std::unique_ptr<Button>(new ButtonContinue(pEngine, 550, 300, 230, 40, 0x2159ff, 0xd4e4ff, "Continue", fntButtons)));
	buttons.push_back(std::unique_ptr<Button>(new ButtonChangeState(pEngine, stateInfo, 550, 350, 230, 40, 0x2159ff, 0xd4e4ff, "Info", fntButtons)));
	buttons.push_back(std::unique_ptr<Button>(new ButtonChangeState(pEngine, stateEditor, 550, 400, 230, 40, 0x2159ff, 0xd4e4ff, "Level Editor", fntButtons)));
	buttons.push_back(std::unique_ptr<Button>(new ButtonExit(pEngine, 550, 450, 230, 40, 0x2159ff, 0xd4e4ff, "Exit", fntButtons)));

	//Start menu music (if not already playing)
	auto audio = pEngine->GetAudio();
	auto path = "music/CharacterCreation.ogg";
	if (!audio->isSongPlaying(path)) audio->playMusic(path,-1);
}

void StateMenu::virtSetupBackgroundBuffer()
{
	bg.renderImageApplyingMapping(pEngine,pEngine->getBackgroundSurface(), 0, 0, bg.getWidth(), bg.getHeight(), &scrollMap);
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
	switch (iKeyCode) {

		//Exit program
	case (SDLK_ESCAPE): pEngine->setExitWithCode(0); break;

		//Save current stats
	case (SDLK_s): pEngine->saveGame(); break;

	}
}

void StateMenu::virtMainLoopDoBeforeUpdate()
{
	//scrollMap.setXShift(pEngine->getModifiedTime() / 10);

	pEngine->lockAndSetupBackground();
	pEngine->redrawDisplay();
}

	//INFO\\

StateInfo::StateInfo(GameEngine* pEngine) : StateMenu(pEngine)
{
	fntInfo = pEngine->getFont("fonts/gameplay.ttf", 15);
}

void StateInfo::onStateEnter()
{

}

void StateInfo::virtSetupBackgroundBuffer()
{
	bg.renderImageApplyingMapping(pEngine, pEngine->getBackgroundSurface(), 0, 0, bg.getWidth(), bg.getHeight(), &scrollMap);
}

void StateInfo::virtDrawStringsOnTop()
{
	pEngine->drawForegroundString(300, 100, "Info", 0xffffff, fntTitle);
	pEngine->drawForegroundString(40, 170, "You are an skeletal undead, born in the upper levels of the Great Dungeon.", 0xffffff, fntInfo);
	pEngine->drawForegroundString(40, 200, "Recently, other monsters have been suffering from a curse that pilots their flesh like a puppeteer.", 0xffffff, fntInfo);
	pEngine->drawForegroundString(40, 230, "Your bones are immune to the spell, so it falls onto you to discover the source of the affliction...", 0xffffff, fntInfo);

	pEngine->drawForegroundString(200, 300, "ARROW KEYS = Move", 0xffffff, fntButtons);
	pEngine->drawForegroundString(200, 340, "SPACE = Next Turn", 0xffffff, fntButtons);
	pEngine->drawForegroundString(200, 380, "LEFT CLICK = Move to Tile", 0xffffff, fntButtons);
	pEngine->drawForegroundString(200, 420, "RIGHT CLICK = Interact/Attack object", 0xffffff, fntButtons);
	pEngine->drawForegroundString(200, 460, "WASD = Move Camera", 0xffffff, fntButtons);
	pEngine->drawForegroundString(200, 500, "SCROLL WHEEL = Zoom In/Out", 0xffffff, fntButtons);
	pEngine->drawForegroundString(200, 540, "ESCAPE = Back/Quit Game", 0xffffff, fntButtons);
}

void StateInfo::virtKeyDown(int iKeyCode)
{
	//Back to menu
	if (iKeyCode == SDLK_ESCAPE) pEngine->setState(pEngine->stateMenu);
}


	//CHARACTER CREATE\\

StateCharCreate::StateCharCreate(GameEngine* pEngine) : StateMenu(pEngine)
{
	buttons.clear();
	buttons.push_back(std::unique_ptr<Button>(new ButtonChangeState(pEngine, pEngine->stateStart, 500, 600, 230, 40, 0x2159ff, 0xd4e4ff, "Start", fntButtons)));

	//Attribute change buttons
	auto skillUps = &(pEngine->skillUps);
	buttons.push_back(std::unique_ptr<Button>(new ButtonAllocateSkill<int>(pEngine, 420, 235, 40, 40, 0x2159ff, 0xd4e4ff, "+", fntButtons, &(pEngine->strength), 1, skillUps)));
	buttons.push_back(std::unique_ptr<Button>(new ButtonAllocateSkill<int>(pEngine, 470, 235, 40, 40, 0x2159ff, 0xd4e4ff, "-", fntButtons, &(pEngine->strength), -1, skillUps)));

	buttons.push_back(std::unique_ptr<Button>(new ButtonAllocateSkill<int>(pEngine, 420, 285, 40, 40, 0x2159ff, 0xd4e4ff, "+", fntButtons, &(pEngine->ranged), 1, skillUps)));
	buttons.push_back(std::unique_ptr<Button>(new ButtonAllocateSkill<int>(pEngine, 470, 285, 40, 40, 0x2159ff, 0xd4e4ff, "-", fntButtons, &(pEngine->ranged), -1, skillUps)));

	buttons.push_back(std::unique_ptr<Button>(new ButtonAllocateSkill<int>(pEngine, 420, 335, 40, 40, 0x2159ff, 0xd4e4ff, "+", fntButtons, &(pEngine->maxMagic), 1, skillUps)));
	buttons.push_back(std::unique_ptr<Button>(new ButtonAllocateSkill<int>(pEngine, 470, 335, 40, 40, 0x2159ff, 0xd4e4ff, "-", fntButtons, &(pEngine->maxMagic), -1, skillUps)));

	buttons.push_back(std::unique_ptr<Button>(new ButtonAllocateSkill<int>(pEngine, 420, 385, 40, 40, 0x2159ff, 0xd4e4ff, "+", fntButtons, &(pEngine->defence), 1, skillUps)));
	buttons.push_back(std::unique_ptr<Button>(new ButtonAllocateSkill<int>(pEngine, 470, 385, 40, 40, 0x2159ff, 0xd4e4ff, "-", fntButtons, &(pEngine->defence), -1, skillUps)));
}

StateCharCreate::~StateCharCreate()
{

}

void StateCharCreate::onStateEnter()
{
	//Set default name
	charInput.assign("Skeleton");

	pEngine->resetStats();
}

void StateCharCreate::onStateExit()
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
	if (charInput.length() > 0) printNameInput.append(charInput);

	//Append flashing line periodically if typing activated
	if (typing && pEngine->getModifiedTime() % 300 > 150) printNameInput.append("|");

	//Draw string input
	pEngine->drawForegroundString(420, 160, printNameInput.c_str(), 0xffffff, fntButtons);

	//Draw stats
	std::string printStrength = "STR: " + std::to_string(pEngine->strength);
	std::string printRanged = "RANGE: " + std::to_string(pEngine->ranged);
	std::string printMagic = "MAG: " + std::to_string(pEngine->maxMagic);
	std::string printDefence = "DEF: " + std::to_string(pEngine->defence);
	std::string printSkillUps = "Points: " + std::to_string(pEngine->skillUps);
	pEngine->drawForegroundString(520, 190 + 50, printStrength.c_str(), 0xffffff, fntButtons);
	pEngine->drawForegroundString(520, 190 + 100, printRanged.c_str(), 0xffffff, fntButtons);
	pEngine->drawForegroundString(520, 190 + 150, printMagic.c_str(), 0xffffff, fntButtons);
	pEngine->drawForegroundString(520, 190 + 200, printDefence.c_str(), 0xffffff, fntButtons);

	pEngine->drawForegroundString(420, 190 + 300, printSkillUps.c_str(), 0xffffff, fntButtons);
}

void StateCharCreate::virtKeyDown(int iKeyCode)
{
	//If typing is active:
	if (typing) {

		//(We're using a font where lower and upper are the same)
		int k = std::tolower(iKeyCode);

		//Allow alphabet and spacebar for input
		if (k >= 97 && k <= 122 || k == SDLK_SPACE) {
			charInput.push_back((char)k);
			pEngine->redrawDisplay();
		}

		//Backspace - delete last letter if there are any letters
		if (iKeyCode == SDLK_BACKSPACE && charInput.length() > 0 && charInput.length() < 16) {
			charInput.pop_back();
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