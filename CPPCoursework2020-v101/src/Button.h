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

//Changes the given variable by the given value, and then calls a passed function (which can be null)
template <class T>
class ButtonValueChange : public Button {
public:
	ButtonValueChange(GameEngine* pEngine, int x, int y, int width, int height, int colText, int colButton, const char* text, Font* font, T* var, T amount)
		: Button(pEngine, x, y, width, height, colText, colButton, text, font), var(var), amount(amount)
	{
	}
	
	virtual void onClick() override
	{
		*var += amount;
	}
protected:
	T* var;
	T amount;

};

template <class T>
class ButtonAllocateSkill : public ButtonValueChange<T> {
public:
	ButtonAllocateSkill(GameEngine* pEngine, int x, int y, int width, int height, int colText, int colButton, const char* text, Font* font, T* var, T amount, int* skillUps)
		: ButtonValueChange(pEngine, x, y, width, height, colText, colButton, text, font, var, amount), skillUps(skillUps)
	{
	}

	virtual void onClick() override
	{
		//Making sure var and skillUps don't go below zero
		if ((amount > 0 && *skillUps > 0) || (amount < 0 && *var > 1)) {

			//If we're OK, then apply opposite to skillUps and call parent function
			*skillUps -= amount;

			ButtonValueChange::onClick();

		}

	}

private:
	int* skillUps;

};

template <class T>
class ButtonValueSet : public Button {
public:
	ButtonValueSet(GameEngine* pEngine, int x, int y, int width, int height, int colText, int colButton, const char* text, Font* font, T* var, T amount)
		: Button(pEngine, x, y, width, height, colText, colButton, text), var(var), amount(amount) {}

	virtual void onClick() override
	{
		*var = amount;
	}
protected:
	T* var;
	T amount;
};


//STATE CHANGES

class ButtonChangeState : public Button {
public:
	ButtonChangeState(GameEngine* pEngine, BaseState* state, int x, int y, int width, int height, int colText, int colButton, const char* text, Font* font)
		: Button(pEngine, x, y, width, height, colText, colButton, text, font), state(state)
	{
		
	}
	virtual void onClick() override
	{
		pEngine->setState(state);
	}
private:
	BaseState* state;
};

class ButtonContinue : public Button {
public:
	ButtonContinue(GameEngine* pEngine, int x, int y, int width, int height, int colText, int colButton, const char* text, Font* font)
		: Button(pEngine, x, y, width, height, colText, colButton, text, font) {}
	virtual void onClick() override;
};

class ButtonExit : public Button {
public:
	ButtonExit(GameEngine* pEngine, int x, int y, int width, int height, int colText, int colButton, const char* text, Font* font)
		: Button(pEngine, x, y, width, height, colText, colButton, text, font) {}
	virtual void onClick() override;
};