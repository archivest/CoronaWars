#pragma once

#include "header.h"
#include "CollidableInterface.h"

class CollisionCallbacks
{
public:
    virtual void onMineCollision(CollidableInterface *pMine) = 0;
    virtual void onMissileCollision(CollidableInterface* pMissile) = 0;
    virtual void onShipCollision(CollidableInterface* pShip) = 0;
    virtual void onMissileMiss(CollidableInterface* pMissile) = 0;
    virtual void onMineDrop(CollidableInterface* pMine) = 0;
};
