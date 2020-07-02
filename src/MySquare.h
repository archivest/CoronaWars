#pragma once
#include "DisplayableObject.h"
#include "MovementPosition.h"

class MySquare final :
	public DisplayableObject
{
public:
   enum class Behaviour {no_collision, collision};
   enum class Direction { up, down };

public:
	MySquare(BaseEngine* pEngine, unsigned int iColour, Behaviour behaviour);
	void virtDraw() override;
	void virtDoUpdate(int iCurrentTime) override;
	void virtMouseDown(int iButton, int iX, int iY) override;
	void setOtherSquare(MySquare* pOtherSquare);

private:
	MovementPosition m_posCalc;
	unsigned int m_iColour;
	Behaviour m_behaviour;
	Direction m_Direction;
	int m_startX;
	int m_startY;
	int m_endX;
	int m_endY;
	MySquare* m_pOtherSquare;
};

