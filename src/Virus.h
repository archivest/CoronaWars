#pragma once

#include "header.h"
#include "GameImageObject.h"
#include "MovementPosition.h"
#include "CollidableInterface.h"
#include "CollisionDetection.h"
#include "VirusCallbacks.h"

class Virus final : public GameImageObject
{
public:

public:
    Virus(VirusDirection direction, VirusCallbacks* pVirusCallbacks, BaseEngine* pEngine, std::string strURL, int xStart = 0, int yStart = 0, bool isMaskable = false, int maskColour = BLACK, bool isDraggable = false, bool useTopLeftFor00 = true, bool bVisible = true) :
        GameImageObject(pEngine, strURL, xStart, yStart, isMaskable, maskColour, isDraggable, useTopLeftFor00, bVisible),
        mpVirusCallbacks(pVirusCallbacks),
        mRotationSpeed(0.0),
        mToX(0),
        mToY(0),
        mMovementEndTime(0),
        mMovementTimeLeft(0),
        mDirection(direction)
    {
        startMovement();
    }

    Virus(VirusDirection direction, VirusCallbacks* pVirusCallbacks, BaseEngine* pEngine, std::string strURL, int xStart, int yStart, int xTo, int yTo, double rotationSpeed, int movementTimeLeft, bool isMaskable = false, int maskColour = BLACK, bool isDraggable = false, bool useTopLeftFor00 = true, bool bVisible = true) :
        GameImageObject(pEngine, strURL, xStart, yStart, isMaskable, maskColour, isDraggable, useTopLeftFor00, bVisible),
        mpVirusCallbacks(pVirusCallbacks),
        mRotationSpeed(rotationSpeed),
        mToX(xTo),
        mToY(yTo),
        mMovementEndTime(pEngine->getRawTime() + movementTimeLeft),
        mMovementTimeLeft(movementTimeLeft),
        mDirection(direction)
    {
        mMovPosCalc.setup(xStart, yStart, mToX, mToY, pEngine->getRawTime(), mMovementEndTime);
    }

    ~Virus()
    {
    }

    void addCollidableObject(CollidableInterface* pCollidable)
    {
        mCollidableObjects.push_back(pCollidable);
    }

    void clearCollidableObjects()
    {
        mCollidableObjects.clear();
    }

    void removeCollidableObject(CollidableInterface* pCollidable)
    {
        for (auto it = mCollidableObjects.begin(); it != mCollidableObjects.end(); ++it)
        {
            if ((*it) == pCollidable)
            {
                mCollidableObjects.erase(it);
                break;
            }
        }
    }

    void startMovement()
    {
        VirusDirection direction = mDirection == VirusDirection::Random ? static_cast<VirusDirection>((std::rand() % 4) + 1) : mDirection;

        // Make virus rotate
        double rotationPercent = ((double)(std::rand() % 1000) - 500.0) / 100000.0;
        mRotationSpeed = rotationPercent * (2 * M_PI); // express as a percentage of 2pi and store the final value (could be +/-)
        int fromX = 0, fromY = 0;
        switch (direction)
        {
        case VirusDirection::FromLeft:
            fromX = -getImageWidth();
            fromY = std::rand() % getEngine()->getWindowHeight();
            mToX = getEngine()->getWindowWidth() + getImageWidth();
            mToY = std::rand() % getEngine()->getWindowHeight();
            break;

        case VirusDirection::FromTop:
            fromX = std::rand() % getEngine()->getWindowWidth();
            fromY = -getImageHeight();
            mToX = std::rand() % getEngine()->getWindowWidth();
            mToY = getEngine()->getWindowHeight() + getImageHeight();
            break;

        case VirusDirection::FromRight:
            fromX = getEngine()->getWindowWidth() + getImageWidth();
            fromY = std::rand() % getEngine()->getWindowHeight();
            mToX = -getImageWidth();
            mToY = std::rand() % getEngine()->getWindowHeight();
            break;

        default: // from bottom of window
            fromX = std::rand() % getEngine()->getWindowWidth();
            fromY = getEngine()->getWindowHeight() + getImageHeight();
            mToX = std::rand() % getEngine()->getWindowWidth();
            mToY = -getImageHeight();
            break;
        }
        int curTime = getEngine()->getRawTime();
        int duration = 5000 + (std::rand() % 5000); // up to 10 secs to cross screen
        mMovementEndTime = curTime + duration;
        mMovementTimeLeft = duration;
        mMovPosCalc.setup(fromX, fromY, mToX, mToY, curTime, mMovementEndTime);
    }

    void virtDoUpdate(int iCurrentTime) override
    {
        mMovementTimeLeft = mMovementEndTime - iCurrentTime;
        double r = getRotator().getRotation() + mRotationSpeed;
        if (r >= (2 * M_PI))
        {
            r = 0.0;
        }
        else if (r <= 0.0)
        {
            r = 2.0 * M_PI;
        }
        getRotator().setRotation(r);
        mMovPosCalc.calculate(iCurrentTime);
        setPositionDbl(mMovPosCalc.getX(), mMovPosCalc.getY());
        for (auto it = mCollidableObjects.begin(), itEnd = mCollidableObjects.end(); it != itEnd; ++it)
        {
            CollidableInterface* pCollidable = *it;
            if (pCollidable->hasCollided(this))
            {
                pCollidable->onCollision();
                mpVirusCallbacks->onVirusCollision(this);
                break;
            }

            if (mMovPosCalc.hasMovementFinished(iCurrentTime))
            {
                startMovement();
            }
        }
    }

    const MovementPosition& getMovementPositionCalc() const
    {
        return mMovPosCalc;
    }

    int getToX() const
    {
        return mToX;
    }

    int getToY() const
    {
        return mToY;
    }

    int getMovementTimeLeft() const
    {
        return mMovementTimeLeft;
    }

    double getRotationSpeed() const
    {
        return mRotationSpeed;
    }

private:
    VirusCallbacks* mpVirusCallbacks;
    MovementPosition mMovPosCalc;
    std::vector<CollidableInterface*> mCollidableObjects; // vector of objects this image can collide with
    double mRotationSpeed;
    int mToX;
    int mToY;
    int mMovementEndTime;
    int mMovementTimeLeft;
    VirusDirection mDirection;
};
