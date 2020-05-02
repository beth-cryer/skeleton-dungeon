#include "header.h"
#include "Button.h"

#include "StateMenu.h"
#include "StateStart.h"

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


//NEW GAME
void ButtonNewGame::onClick()
{
	pEngine->setState(pEngine->stateCharCreate);
}

//CONTINUE
void ButtonContinue::onClick()
{
	//Load save
	auto loader = pEngine->GetSaveReader();
	loader->getFile("saves/test.txt");

	pEngine->playerName = loader->getData("name");
	pEngine->level = std::stoi(loader->getData("level"));
	pEngine->exp = std::stoi(loader->getData("exp"));
	pEngine->expNext = std::stoi(loader->getData("expnext"));
	pEngine->skillUps = std::stoi(loader->getData("skillups"));

	pEngine->maxHealth = std::stoi(loader->getData("maxhealth"));
	pEngine->health = std::stoi(loader->getData("health"));
	pEngine->maxStamina = std::stoi(loader->getData("maxstamina"));
	pEngine->stamina = std::stoi(loader->getData("stamina"));
	pEngine->maxMagic = std::stoi(loader->getData("maxmagic"));
	pEngine->magic = std::stoi(loader->getData("magic"));

	pEngine->strength = std::stoi(loader->getData("strength"));
	pEngine->ranged = std::stoi(loader->getData("ranged"));
	pEngine->defence = std::stoi(loader->getData("defence"));

	std::string solidTiles = loader->getData("solid");
	std::string solidBack = loader->getData("back");

	pEngine->setState(pEngine->stateStart);
}

//LEVEL EDITOR
void ButtonEditor::onClick()
{
	pEngine->setState(pEngine->stateStart);
}

//EXIT
void ButtonExit::onClick()
{
	pEngine->setExitWithCode(0);
}