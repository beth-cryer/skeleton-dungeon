#pragma once
#include "GameEngine.h"

class Button
{
public:
	Button(GameEngine* pEngine, int x, int y, int width, int height, int colText, int colButton, const char* text, Font* font);
	~Button();

	virtual void onClick();
	virtual void drawButton(bool drawBack); //can override but shouldn't need to
	bool isClicked(int xClick, int yClick);

protected:
	GameEngine* pEngine;
	int x, y, width, height, colText, colButton;
	const char* text;
	Font* font;

};

class ButtonNewGame : public Button {
public:
	ButtonNewGame(GameEngine* pEngine, int x, int y, int width, int height, int colText, int colButton, const char* text, Font* font)
		: Button(pEngine, x, y, width, height, colText, colButton, text, font) {}
	virtual void onClick() override;
};

class ButtonContinue : public Button {
public:
	ButtonContinue(GameEngine* pEngine, int x, int y, int width, int height, int colText, int colButton, const char* text, Font* font)
		: Button(pEngine, x, y, width, height, colText, colButton, text, font) {}
	virtual void onClick() override;
};

class ButtonEditor : public Button {
public:
	ButtonEditor(GameEngine* pEngine, int x, int y, int width, int height, int colText, int colButton, const char* text, Font* font)
		: Button(pEngine, x, y, width, height, colText, colButton, text, font) {}
	virtual void onClick() override;
};


class ButtonExit : public Button {
public:
	ButtonExit(GameEngine* pEngine, int x, int y, int width, int height, int colText, int colButton, const char* text, Font* font)
		: Button(pEngine, x, y, width, height, colText, colButton, text, font) {}
	virtual void onClick() override;
};