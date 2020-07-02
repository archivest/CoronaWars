#pragma once

#include "DisplayableObject.h"
#include "MovementPosition.h"
#include "SimpleImage.h"
#include "DrawingSurface.h"

class MovingImageObject : public DisplayableObject
{
public:
    MovingImageObject(BaseEngine* pEngine, SimpleImage* pImage, int x, int y, int xSkip, int ySkip) :
        DisplayableObject(pEngine, pImage->getWidth(), pImage->getHeight(), true),
        mpImage(pImage),
        mx(x),
        my(y),
        mxSkip(xSkip),
        mySkip(ySkip),
        mLastTime(0),
        mpTempSurface(new DrawingSurface(pEngine))
    {
        mpTempSurface->createSurface(pImage->getWidth(), pImage->getHeight());
    }

    ~MovingImageObject()
    {
        delete mpTempSurface;
    }

    void virtDraw() override
    {
//        mpTempSurface->copyRectangleFrom(getEngine()->getBackgroundSurface(), mx, my, mpImage->getWidth(), mpImage->getHeight());
        mpImage->renderImageWithMask(getEngine()->getBackgroundSurface(), 0, 0, mx, my, mpImage->getWidth(), mpImage->getHeight());
        this->redrawRectangle();
    }

    void virtDoUpdate(int iCurrentTime) override
    {
        if (mLastTime == 0)
        {
            mLastTime = iCurrentTime;
        }
        else
        {
            int timeDiff = iCurrentTime - mLastTime;
            mx += timeDiff;
            my += timeDiff;
            mLastTime = iCurrentTime;
            if (mx > getEngine()->getWindowWidth() + mpImage->getWidth())
            {
                mx = 0;
            }
            if (my > getEngine()->getWindowHeight() + mpImage->getHeight())
            {
                my = 0;
            }
        }
    }

private:
    SimpleImage* mpImage;
    int mx;
    int my;
    int mxSkip;
    int mySkip;
    int mLastTime;
    DrawingSurface *mpTempSurface;
    MovementPosition mMovement;
};
