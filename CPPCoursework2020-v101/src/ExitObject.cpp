#include "header.h"
#include "ExitObject.h"
#include "BaseEngine.h"

void ExitObject::virtDraw()
{
	img.renderImageWithMask(getEngine()->getBackgroundSurface(),0,0,m_iCurrentScreenX,m_iCurrentScreenY,m_iDrawWidth,m_iDrawHeight,0xFF00FF);
}

void ExitObject::virtDoUpdate(int iCurrentTime)
{

}

//Click to move to next floor
void ExitObject::virtMouseDown(int iButton, int iX, int iY)
{
	if (locks <= 0) {

	}
}