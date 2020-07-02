#pragma once

#include "header.h"
#include "DisplayableObject.h"
#include "CollidableInterface.h"
#include "CollisionCallbacks.h"

class Missile final : public DisplayableObject, public CollidableInterface
{
public:
    Missile(double r, CollisionCallbacks* pCollisionCallbacks, BaseEngine* pBaseEngine, int xStart, int yStart, int shipWidth, int shipHeight) :
        DisplayableObject(xStart, yStart, pBaseEngine, 5, 20),
        mpCollisionCallbacks(pCollisionCallbacks),
        mAngle(r),
        mX((double)xStart),
        mY((double)yStart),
        mXr(0.0),
        mYr(0.0),
        mSpeedX(sin(r) * 400.0),
        mSpeedY(cos(r) * 400.0),
        mShipWidth(shipWidth),
        mShipHeight(shipHeight),
        miLastDrawTime(0)
    {
    }

    ~Missile() {}

    bool hasCollided(GameImageObject* pVirus) override
    {
        // collision is based on whether two circles overlap
        // These are circles with the virus image's radius and one with the missile's coordinates with a radius of 0 pixels
        double x1c = pVirus->getXDbl() + ((double)pVirus->getImageWidth() / 2.0);
        double x2c = mXr;
        double y1c = pVirus->getYDbl() + ((double)pVirus->getImageHeight() / 2.0);
        double y2c = mYr;
        double r1 = (double)pVirus->getImageWidth() / 2.0; // actually the radius, but the centre is also the radius
        double r2 = 0;
        return (sqrt(pow(x2c - x1c, 2.0) + pow(y2c - y1c, 2.0)) <= (r1 + r2));
    }

    void onCollision() override
    {
        mpCollisionCallbacks->onMissileCollision(this);
    }

    void virtDoUpdate(int iCurrentTime) override
    {
        if (mX < 0.0 || mY < 0.0 || mX >(double)getEngine()->getWindowWidth() || mY >(double)getEngine()->getWindowHeight())
        {
            mpCollisionCallbacks->onMissileMiss(this);
            return;
        }

        if (miLastDrawTime == 0)
        {
            miLastDrawTime = iCurrentTime;
        }
        else
        {
            // move the missile by distance according to the speed 
            double diff = (double)iCurrentTime - (double)miLastDrawTime;
            double distanceX = mSpeedX * (diff / 1000.0);
            double distanceY = mSpeedY * (diff / 1000.0);
            mX -= distanceX;
            mY -= distanceY;
            setPosition((int)mX, (int)mY);
            miLastDrawTime = iCurrentTime;
        }
    }

    void virtDraw() override
    {
        // get the screen coordinates for the top left of the unrotated ship when the missile was created
        double xOffset = mX;
        double yOffset = mY;

        // The rotation is done by relative position rather than absolute position, so all calculations expect 0,0 to be the top left of the ship
        // I want the missile to emerge from the centre of the ship at the nose, so set x1 and y1 to the coordinate for that position
        double x1 = (mShipWidth / 2.0);
        double y1 = 0;
        // x2 and y2 are the other end of the missile line. x2 is the same as x1 and y2 is 20 pixels below
        double x2 = x1;
        double y2 = y1 + 20;

        // rotate around the centre of the ship, so adjust for the centre before rotating
        x1 -= (double)mShipWidth / 2.0;
        y1 -= (double)mShipHeight / 2.0;
        x2 -= (double)mShipWidth / 2.0;
        y2 -= (double)mShipHeight / 2.0;

        // do rotation of both start and end coordinates
        double x1r = (cos(-mAngle) * x1) - (sin(-mAngle) * y1);
        double y1r = (sin(-mAngle) * x1) + (cos(-mAngle) * y1);
        double x2r = (cos(-mAngle) * x2) - (sin(-mAngle) * y2);
        double y2r = (sin(-mAngle) * x2) + (cos(-mAngle) * y2);


        // undo the earlier centre adjustment
        x1 = x1r + ((double)mShipWidth / 2.0);
        y1 = y1r + ((double)mShipHeight / 2.0);
        x2 = x2r + ((double)mShipWidth / 2.0);
        y2 = y2r + ((double)mShipHeight / 2.0);

        // now adjust for the absolute offsets
        mXr = x1 + xOffset;
        mYr = y1 + yOffset;
        x2 += xOffset;
        y2 += yOffset;

        // draw the missile
        getEngine()->drawForegroundThickLine((int)mXr, (int)mYr, (int)x2, (int)y2, VIOLET, DisplayableObject::getDrawWidth());
    }

    double getAngle() const
    {
        return mAngle;
    }

    int getX() override { return DisplayableObject::m_iCurrentScreenX; }
    int getY() override { return DisplayableObject::m_iCurrentScreenY; }
    int getWidth() override { return DisplayableObject::getDrawWidth(); }
    int getHeight() override { return DisplayableObject::getDrawHeight(); }

private:
    CollisionCallbacks* mpCollisionCallbacks;
    double mAngle;
    double mX;
    double mY;
    double mXr;
    double mYr;
    double mSpeedX;
    double mSpeedY;
    int mShipWidth;
    int mShipHeight;
    int miLastDrawTime;
};
