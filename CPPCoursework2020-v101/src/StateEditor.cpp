#include "header.h"
#include "StateEditor.h"

#include "StateMenu.h"

StateEditor::StateEditor(GameEngine* pEngine)
	: BaseState(pEngine),
	tileLayer(&tilesSolid)
{
}

void StateEditor::onStateEnter()
{
	int w = 16; int h = 16;

	//Set all tiles to 0
	tilesSolid.setMapSize(w, h);
	tilesBack.setMapSize(w, h);
	tilesEditor.setMapSize(w, h);

	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			tilesSolid.setMapValue(x, y, 0);
			tilesBack.setMapValue(x, y, 0);
			tilesEditor.setMapValue(x, y, 0);
		}
	}

	tilesSolid.setTopLeftPositionOnScreen(0, 0);
	tilesBack.setTopLeftPositionOnScreen(0, 0);
	tilesEditor.setTopLeftPositionOnScreen(0, 0);

	pEngine->lockAndSetupBackground();

}

void StateEditor::onStateExit()
{
}

void StateEditor::virtSetupBackgroundBuffer()
{
	//Fill in background with black
	pEngine->fillBackground(0);

	//Draw all tiles from the Tile Map to the background surface
	tilesBack.drawAllTiles(pEngine, pEngine->getBackgroundSurface());
	tilesSolid.drawAllTiles(pEngine, pEngine->getBackgroundSurface());
	tilesEditor.drawAllTiles(pEngine, pEngine->getBackgroundSurface());

	//Draw grid system
	for (int y = 0; y < WIN_HEIGHT / TILE_SIZE; y += TILE_SIZE) {
		for (int x = 0; x < WIN_WIDTH / TILE_SIZE; x += TILE_SIZE) {
			grid.renderImageWithMask(pEngine->getBackgroundSurface(), 0, 0, x, y, TILE_SIZE, TILE_SIZE, 0xFF00FF);
		}
	}
	
}

void StateEditor::virtDrawStringsOnTop()
{
	BaseState::virtDrawStringsOnTop();

	//Draw currently-selected tile
	std::string tilestr = "Tile: ";
	tilestr.append(std::to_string(tileId));
	pEngine->drawForegroundString(0,0, tilestr.c_str(),0xFFFFFF);

	if (tileId != 0)
		tileLayer->GetTileSprites().renderImageWithMask(pEngine->getBackgroundSurface(), 0,0, 0, TILE_SIZE, TILE_SIZE, TILE_SIZE, 0xFF00FF);

	//tileLayer->GetTileSprites().renderImageWithMask(pEngine->getBackgroundSurface(), TILE_SIZE * (tileId % 11), TILE_SIZE * std::floor(tileId / 10), 0, TILE_SIZE, TILE_SIZE, TILE_SIZE, 0xFF00FF);
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
		tileLayer->setAndRedrawMapValueAt(mapX, mapY, tileId,pEngine,pEngine->getBackgroundSurface());
		pEngine->lockAndSetupBackground();
		pEngine->redrawDisplay();
		break;

	//Right click to delete tile
	case(SDL_BUTTON_RIGHT):
		tileLayer->setMapValue(mapX, mapY, 0); //Delete tile (by changing to 0, the default empty tile)
		pEngine->lockAndSetupBackground();
		pEngine->redrawDisplay();
		break;

	//Middle click to fill empty tiles (just for background tiles really)
	case(SDL_BUTTON_MIDDLE):

		break;

	}

}

void StateEditor::virtMouseWheel(int x, int y, int which, int timestamp)
{
	//Switch selected tile

	//Down
	if (y < 0) {
		if (tileId > 0) tileId--;
	}
	//Up
	else if (y > 0) {
		tileId++;
	}

	pEngine->lockAndSetupBackground();
	pEngine->redrawDisplay();
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
	
}

void StateEditor::virtMainLoopPostUpdate()
{
}