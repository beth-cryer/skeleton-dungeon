#include "header.h"
#include "StateEditor.h"

#include "StateMenu.h"

StateEditor::StateEditor(GameEngine* pEngine)
	: BaseState(pEngine),
	tileLayer(tilesSolid)
{
}

void StateEditor::onStateEnter()
{
	//Set all tiles to 0

}

void StateEditor::onStateExit()
{
}

void StateEditor::virtSetupBackgroundBuffer()
{
	//Draw all tiles from the Tile Map to the background surface
	pEngine->GetTilesBack()->drawAllTiles(pEngine, pEngine->getBackgroundSurface());
	pEngine->GetTilesSolid()->drawAllTiles(pEngine, pEngine->getBackgroundSurface());
}

void StateEditor::virtDrawStringsOnTop()
{
	BaseState::virtDrawStringsOnTop();
}

void StateEditor::virtMouseDown(int iButton, int iX, int iY)
{
	BaseState::virtMouseDown(iButton, iX, iY);

	int mapX=0, mapY=0, value=0;
	//If Tile clicked on:
	if (tileLayer->isValidTilePosition(iX, iY))
	{
		//Get column, row, current value
		mapX = tileLayer->getMapXForScreenX(iX);
		mapY = tileLayer->getMapYForScreenY(iY);
		value = tileLayer->getMapValue(mapX, mapY);
	}

	switch (iButton) {

	//Click to place tile
	case(SDL_BUTTON_LEFT):
		if (tileLayer->isValidTilePosition(iX, iY)) tileLayer->setMapValue(mapX, mapY, tileId); //Delete tile (by changing to 0, the default empty tile)
		break;

	//Right click to delete tile
	case(SDL_BUTTON_RIGHT):
		if (tileLayer->isValidTilePosition(iX, iY)) tileLayer->setMapValue(mapX, mapY, 0); //Delete tile (by changing to 0, the default empty tile)
		break;

	//Middle click to fill empty tiles (just for background tiles really)
	case(SDL_BUTTON_MIDDLE):

		break;

	}

	//Force redraw screen
	pEngine->lockAndSetupBackground();
	pEngine->redrawDisplay();
}

void StateEditor::virtMouseWheel(int x, int y, int which, int timestamp)
{
	//Switch selected tile

	//Up
	if (y < 0) {
		if (tileId > 0) tileId++;
	}
	//Down
	else if (y > 0) {
		tileId--;
	}
}

void StateEditor::virtKeyDown(int iKeyCode)
{
	switch (iKeyCode) {

		//BACK TO MENU
	case SDLK_ESCAPE:
		pEngine->setState(pEngine->stateMenu);
		break;

		//MOVING CAMERA
	case SDLK_a:
		pEngine->moveCamera(50, 0);
		break;
	case SDLK_d:
		pEngine->moveCamera(-50, 0);
		break;
	case SDLK_w:
		pEngine->moveCamera(0, 50);
		break;
	case SDLK_s:
		pEngine->moveCamera(0, -50);
		break;
	}
}

void StateEditor::virtMainLoopPreUpdate()
{
	//Draw currently-selected tile

}

void StateEditor::virtMainLoopPostUpdate()
{
}