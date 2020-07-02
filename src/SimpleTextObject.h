#pragma once
#include "header.h"
#include "DisplayableObject.h"

class SimpleTextObject : public DisplayableObject
{
public:
    SimpleTextObject(BaseEngine* pEngine, int posX, int posY, const char* pText, int colour) :
        DisplayableObject(pEngine),
        miPosX(posX),
        miPosY(posY),
        mText(pText),
        mColour(colour)
    {
        setVisible(true);
    }

    virtual ~SimpleTextObject() {};

    virtual void virtDoUpdate(int iCurrentTime) override
    {
        redrawDisplay();
    }

    virtual void virtDraw() override
    {
        if (!mText.empty())
        {
            getEngine()->drawForegroundString(miPosX, miPosY, mText.c_str(), mColour);
        }
    }

protected:
    int miPosX;
    int miPosY;
    std::string mText;
    int mColour;
};
