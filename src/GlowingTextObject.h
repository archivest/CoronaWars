#pragma once
#include "header.h"
#include "SimpleTextObject.h"

class GlowingTextObject : public SimpleTextObject
{
public:
    GlowingTextObject(BaseEngine* pEngine, int posX, int posY, const char* pText, int fromColour, int toColour) :
        SimpleTextObject(pEngine, posX, posY, pText, fromColour),
        mFromColour(fromColour),
        mToColour(toColour),
        miLastTime(0),
        mForward(true),
        mNumMS(500 / ((toColour - fromColour) / 0x010101))
    {
    }

    virtual ~GlowingTextObject() {}

    void virtDoUpdate(int iCurrentTime) override
    {
        if (miLastTime == 0)
        {
            miLastTime = iCurrentTime;
        }
        else
        {
            int diff = iCurrentTime - miLastTime;
            int steps = diff / mNumMS;
            for (int i = 0; i < steps; ++i)
            {
                if (mForward)
                {
                    mColour += 0x010101;
                    if (mColour > mToColour)
                    {
                        mColour = mToColour;
                        mForward = false;
                    }
                }
                else
                {
                    mColour -= 0x010101;
                    if (mColour < mFromColour)
                    {
                        mColour = mFromColour;
                        mForward = true;
                    }
                }
            }
            miLastTime = iCurrentTime;
            getEngine()->redrawDisplay();
        }
    }

    void virtDraw() override
    {
        if (!mText.empty())
        {
            getEngine()->drawForegroundString(miPosX, miPosY, mText.c_str(), mColour);
        }
    }
    
private:
    int mFromColour;
    int mToColour;
    int miLastTime;
    bool mForward;
    int mNumMS;
};
