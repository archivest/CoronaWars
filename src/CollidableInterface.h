#pragma once

#include "header.h"
#include "GameImageObject.h"

class CollidableInterface
{
public:
    virtual bool hasCollided(GameImageObject* pVirus) = 0;
    virtual void onCollision() = 0;
    virtual int getX() = 0;
    virtual int getY() = 0;
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
};
