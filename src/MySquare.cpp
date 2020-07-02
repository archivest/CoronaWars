#include "header.h"
#include "MySquare.h"
#include "BaseEngine.h"
#include "DisplayableObjectContainer.h"
#include "CollisionDetection.h"

MySquare::MySquare(BaseEngine* pEngine, unsigned int iColour, Behaviour behaviour)
	: DisplayableObject(pEngine),
	m_iColour(iColour),
	m_behaviour(behaviour),
	m_Direction(Direction::down)
{
	m_iCurrentScreenX = 0; // Starting position on the screen
	m_iCurrentScreenY = 0;
	m_iDrawWidth = 50; // Width of drawing area
	m_iDrawHeight = 50; // Height of drawing area
    
	int curTime = getEngine()->getRawTime();
	switch (behaviour)
	{
	case Behaviour::no_collision:
		m_startX = m_startY = 0;
		m_endX = 1875 - m_iDrawWidth;
		m_endY = 1020 - m_iDrawHeight;
		m_posCalc.setup(m_startX, m_startY, m_endX, m_endY, curTime, curTime + 10000);
		break;

	case Behaviour::collision:
		m_startX = (std::rand() % 1875) - m_iDrawWidth;
		m_endX = (std::rand() % 1875) - m_iDrawWidth;
		m_startY = 0;
		m_endY = 1020 - m_iDrawHeight;
		m_posCalc.setup(m_startX, m_startY, m_endX, m_endY, curTime, curTime + 5000);
		break;
	}
}

void MySquare::virtDraw()
{
	getEngine()->drawForegroundRectangle( m_iCurrentScreenX, m_iCurrentScreenY,
		m_iCurrentScreenX + m_iDrawWidth, m_iCurrentScreenY + m_iDrawHeight,
		m_iColour);
}

void MySquare::virtDoUpdate(int iCurrentTime)
{
	m_posCalc.calculate(iCurrentTime);
	m_iCurrentScreenX = m_posCalc.getX();
	m_iCurrentScreenY = m_posCalc.getY();
	switch (m_behaviour)
	{
	case Behaviour::no_collision:
		if (m_posCalc.hasMovementFinished(iCurrentTime))
		{
			m_startX = m_iCurrentScreenX;
			m_startY = m_iCurrentScreenY;
			if (m_Direction == Direction::down)
			{
				m_Direction = Direction::up;
				m_endX = 0;
				m_endY = 0;
			}
			else
			{
				m_Direction = Direction::down;
				m_endX = 1875 - m_iDrawWidth;
				m_endY = 1020 - m_iDrawHeight;
			}
			m_posCalc.setup(m_startX, m_startY, m_endX, m_endY, iCurrentTime, iCurrentTime + 10000);
		}
		break;

	case Behaviour::collision:
		if (m_posCalc.hasMovementFinished(iCurrentTime))
		{
			m_startX = m_iCurrentScreenX;
			m_startY = m_iCurrentScreenY;
			m_endX = (std::rand() % 1875) - m_iDrawWidth;
			if (m_Direction == Direction::down)
			{
				m_Direction = Direction::up;
				m_endY = 0;
			}
			else
			{
				m_Direction = Direction::down;
				m_endY = 1020 - m_iDrawHeight;
			}
			m_posCalc.setup(m_startX, m_startY, m_endX, m_endY, iCurrentTime, iCurrentTime + 5000);
		}
		else if (CollisionDetection::checkRectangles(
			m_iCurrentScreenX, // our left x coord
			m_iCurrentScreenX + m_iDrawWidth, // our right x coord 
			m_iCurrentScreenY, //our top y coord
			m_iCurrentScreenY + m_iDrawHeight, // our bottom y coord
			m_pOtherSquare->m_iCurrentScreenX, // other square left x coord
			m_pOtherSquare->m_iCurrentScreenX + m_pOtherSquare->getDrawWidth(), // other square right x coord
			m_pOtherSquare->m_iCurrentScreenY, // other square top y coord
			m_pOtherSquare->m_iCurrentScreenY + m_pOtherSquare->getDrawHeight()) // other square bottom y coord
			)
		{
			// is this square to the left of the other square
			if (m_iCurrentScreenX < m_pOtherSquare->m_iCurrentScreenX)
			{
				// this square is to the left of the other one, so send it to the left
				m_endX = 0;
			}
			else
			{
				m_endX = 1050 - m_iDrawWidth;
			}
			m_startX = m_iCurrentScreenX;
			m_startY = m_iCurrentScreenY;
			if (m_Direction == Direction::down)
			{
				m_Direction = Direction::up;
				m_endY = 0;
			}
			else
			{
				m_Direction = Direction::down;
				m_endY = 1020 - m_iDrawHeight;
			}
			m_posCalc.setup(m_startX, m_startY, m_endX, m_endY, iCurrentTime, iCurrentTime + 5000);
		}
		break;
	}
	redrawDisplay();
}

void MySquare::setOtherSquare(MySquare* pOtherSquare)
{
	m_pOtherSquare = pOtherSquare;
}

void MySquare::virtMouseDown(int iButton, int iX, int iY)
{
	if (m_behaviour == Behaviour::collision)
	{
		m_startX = m_iCurrentScreenX;
		m_startY = m_iCurrentScreenY;
		m_endX = iX;
		m_endY = iY;
		m_posCalc.setup(m_startX, m_startY, m_endX, m_endY, getEngine()->getRawTime(), getEngine()->getRawTime() + 5000);
		m_iCurrentScreenX = iX;
		m_iCurrentScreenY = iY;
	}
}

