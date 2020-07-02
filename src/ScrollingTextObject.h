#pragma once
#include "header.h"
#include "SimpleTextObject.h"

class ScrollingTextObject : public SimpleTextObject
{
public:
    ScrollingTextObject(BaseEngine* pEngine, int posX, int posY, const char* pText, int colour, int delayMillisecs, int deltaPosition, int minPosY = 0) :
        SimpleTextObject(pEngine, posX, posY, pText, colour),
        mDelayMillisecs(delayMillisecs),
        mDeltaPosition(deltaPosition),
        miMinPosY(minPosY),
        miLastTime(0)
    {
        setVisible(true);
    }

    virtual ~ScrollingTextObject() = default;

    void virtDoUpdate(int iCurrentTime) override
    {
        if (miLastTime == 0)
        {
            redrawDisplay();
            miLastTime = iCurrentTime;
        }
        else if (miPosY > miMinPosY && (iCurrentTime >= (miLastTime + mDelayMillisecs)))
        {
            miPosY -= mDeltaPosition;
            redrawDisplay();
            miLastTime = iCurrentTime;
        }
    }

    void virtDraw() override
    {
        if (!mText.empty())
        {
            getEngine()->drawForegroundString(miPosX, miPosY, mText.c_str(), mColour);
        }
    }

protected:
    int mDelayMillisecs;
    int mDeltaPosition;
    int miMinPosY;
    int miLastTime;
};
