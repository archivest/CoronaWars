#pragma once

#include "header.h"
#include "GameImageObject.h"

class VirusCallbacks
{
public:
    virtual void onVirusCollision(GameImageObject* pVirus) = 0;
};
