#pragma once
#include "header.h"
#include "SimpleTextObject.h"

class FlashingTextObject : public SimpleTextObject
{
public:
    FlashingTextObject(BaseEngine* pEngine, int posX, int posY, const char* pText, int colour1, int colour2, int flashRateMillisecs) :
        SimpleTextObject(pEngine, posX, posY, pText, colour1),
        mColour2(colour2),
        mFlashRateMillisecs(flashRateMillisecs),
        mUseColour2(false),
        miLastTime(0)
    {
        setVisible(true);
    }

    void virtDoUpdate(int iCurrentTime) override
    {
        if (miLastTime == 0)
        {
            miLastTime = iCurrentTime;
        }
        else
        {
            if (iCurrentTime >= miLastTime + mFlashRateMillisecs)
            {
                redrawDisplay();
                miLastTime = iCurrentTime;
                mUseColour2 = !mUseColour2;
            }
        }
    }

    void virtDraw() override
    {
        if (!mText.empty())
        {
            getEngine()->drawForegroundString(miPosX, miPosY, mText.c_str(), mUseColour2 ? mColour2 : mColour);
        }
    }

private:
    int mColour2;
    int mFlashRateMillisecs;
    bool mUseColour2;
    int miLastTime;
};
