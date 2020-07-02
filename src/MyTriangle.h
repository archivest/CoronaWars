#pragma once
#include "DisplayableObject.h"
#include "EzyjddTileManager.h"

class MyTriangle final :
	public DisplayableObject
{
public:
	MyTriangle(BaseEngine* pEngine, EzyjddTileManager& tm);
	void virtDraw() override;
	void virtDoUpdate(int iCurrentTime) override;

private:
	int m_iCurMouseX;
	int m_iCurMouseY;
	EzyjddTileManager& m_tm;
};

