#pragma once
#include "DisplayableObject.h"

class MyChangingText final :
	public DisplayableObject
{
public:
	MyChangingText(BaseEngine* pEngine);
	void virtDraw() override;
	void virtDoUpdate(int iCurrentTime) override;
	void virtMouseDown(int iButton, int iX, int iY) override;

private:
	char m_Text[100];
};
