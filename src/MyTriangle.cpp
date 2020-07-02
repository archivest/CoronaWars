#include "header.h"
#include "MyTriangle.h"
#include "BaseEngine.h"
#include "DisplayableObjectContainer.h"

MyTriangle::MyTriangle(BaseEngine* pEngine, EzyjddTileManager& tm)
	: DisplayableObject(pEngine),
	m_tm(tm)
{
	m_iCurrentScreenX = 938; // Starting position on the screen
	m_iCurrentScreenY = 512;
	m_iDrawWidth = 50; // Width of drawing area
	m_iDrawHeight = 60; // Height of drawing area
	m_iCurMouseX = getEngine()->getCurrentMouseX();
	m_iCurMouseY = getEngine()->getCurrentMouseY();
}

void MyTriangle::virtDraw()
{
	getEngine()->drawForegroundTriangle(
		m_iCurrentScreenX, m_iCurrentScreenY,
		m_iCurrentScreenX + m_iDrawWidth - 1, m_iCurrentScreenY, 
		m_iCurrentScreenX + m_iDrawWidth/2, m_iCurrentScreenY - m_iDrawHeight,
		0xB00000);
}

void MyTriangle::virtDoUpdate(int iCurrentTime)
{
	BaseEngine* be = getEngine();
	// Change position if player presses a key
	if (be->isKeyPressed(SDLK_UP))
	{
		m_iCurrentScreenY -= 2;
	}
	else if (be->isKeyPressed(SDLK_DOWN))
	{
		m_iCurrentScreenY += 2;
	}
	else if (be->isKeyPressed(SDLK_LEFT))
	{
		m_iCurrentScreenX -= 2;
	}
	else if (be->isKeyPressed(SDLK_RIGHT))
	{
		m_iCurrentScreenX += 2;
	}
	else
	{
		int iCurMouseX = be->getCurrentMouseX();
		int iCurMouseY = be->getCurrentMouseY();
		if (m_iCurMouseX != iCurMouseX)
		{
			m_iCurMouseX = iCurMouseX;
			m_iCurrentScreenX = iCurMouseX;
		}
		if (m_iCurMouseY != iCurMouseY)
		{
			m_iCurMouseY = iCurMouseY;
			m_iCurrentScreenY = iCurMouseY;
		}
	}
	// Ensure that the objects get redrawn on the display
	if (m_tm.isValidTilePosition(m_iCurrentScreenX, m_iCurrentScreenY))
	{
		m_tm.doSomething(m_iCurrentScreenX, m_iCurrentScreenY);
	}
	redrawDisplay();
}
