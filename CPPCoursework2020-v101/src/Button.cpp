#include "header.h"
#include "Button.h"

#include "StateMenu.h"
#include "StateStart.h"
#include "StateEditor.h"

Button::Button(GameEngine* pEngine, int x, int y, int width, int height, int colText, int colButton, const char* text, Font* font = NULL)
	: pEngine(pEngine), x(x), y(y), width(width), height(height), colText(colText), colButton(colButton), text(text), font(font)
{
}

Button::~Button() {}

void Button::onClick()
{
}

void Button::drawButton(bool drawBack = true)
{
	if (drawBack) pEngine->drawForegroundRectangle(x, y, x + width, y + height, colButton);
	pEngine->drawForegroundString(x+5, y+5, text, colText, font);
}

bool Button::isClicked(int xClick, int yClick)
{
	if (xClick > x&& yClick > y&& xClick < x + width && yClick < y + height) {
		return true;
	}
	return false;
}


//TO CHARACTER CREATOR
void ButtonCharCreator::onClick()
{
	pEngine->setState(pEngine->stateCharCreate);
}

//START NEW GAME
void ButtonNewGame::onClick()
{
	pEngine->setState(pEngine->stateStart);
}

//CONTINUE
void ButtonContinue::onClick()
{
	//Load save
	auto loader = pEngine->GetSaveManager();
	loader->loadFileContents("saves/test.txt");

	pEngine->playerName = loader->getSaveData("name");
	pEngine->level = std::stoi(loader->getSaveData("level"));
	pEngine->exp = std::stoi(loader->getSaveData("exp"));
	pEngine->expNext = std::stoi(loader->getSaveData("expnext"));
	pEngine->skillUps = std::stoi(loader->getSaveData("skillups"));

	pEngine->maxHealth = std::stoi(loader->getSaveData("maxhealth"));
	pEngine->health = std::stoi(loader->getSaveData("health"));
	pEngine->maxStamina = std::stoi(loader->getSaveData("maxstamina"));
	pEngine->stamina = std::stoi(loader->getSaveData("stamina"));
	pEngine->maxMagic = std::stoi(loader->getSaveData("maxmagic"));
	pEngine->magic = std::stoi(loader->getSaveData("magic"));

	pEngine->strength = std::stoi(loader->getSaveData("strength"));
	pEngine->ranged = std::stoi(loader->getSaveData("ranged"));
	pEngine->defence = std::stoi(loader->getSaveData("defence"));

	std::string solidTiles = loader->getSaveData("solid");
	std::string solidBack = loader->getSaveData("back");

	pEngine->setState(pEngine->stateStart);
}

//LEVEL EDITOR
void ButtonEditor::onClick()
{
	pEngine->setState(pEngine->stateEditor);
}

//EXIT
void ButtonExit::onClick()
{
	pEngine->setExitWithCode(0);
}