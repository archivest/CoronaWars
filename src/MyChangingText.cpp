#include "header.h"
#include "MyChangingText.h"
#include "BaseEngine.h"
#include "DisplayableObjectContainer.h"

MyChangingText::MyChangingText(BaseEngine* pEngine)
	: DisplayableObject(pEngine)
{
	m_iCurrentScreenX = m_iCurrentScreenY = 0; // make sure these are zero just in case
    strcpy(m_Text, "Press the mouse button");
}

void MyChangingText::virtDraw()
{
	getEngine()->drawForegroundString(m_iCurrentScreenX, m_iCurrentScreenY,
		m_Text,
		0xAFA4B3);
}

void MyChangingText::virtMouseDown(int iButton, int iX, int iY)
{
	m_iCurrentScreenX = iX;
	m_iCurrentScreenY = iY;
	sprintf(m_Text, "Xpos=%d, Ypos=%d", iX, iY);
}

void MyChangingText::virtDoUpdate(int iCurrentTime)
{
	redrawDisplay();
}
