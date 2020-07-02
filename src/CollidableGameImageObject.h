#pragma once

#include "header.h"
#include "GameImageObject.h"
#include "CollidableInterface.h"

class CollidableGameImageObject : public GameImageObject, public CollidableInterface
{
public:
    CollidableGameImageObject(BaseEngine* pEngine, const std::string& strURL, int xStart = 0, int yStart = 0, bool isMaskable = false, int maskColour = BLACK, bool isDraggable = false, bool useTopLeftFor00 = true, bool bVisible = true) :
        GameImageObject(pEngine, strURL, xStart, yStart, isMaskable, maskColour, isDraggable, useTopLeftFor00,  bVisible)
    {
    }

    virtual ~CollidableGameImageObject() {}

    int getX() override { return DisplayableObject::m_iCurrentScreenX; }
    int getY() override { return DisplayableObject::m_iCurrentScreenY; }
    int getWidth() override { return DisplayableObject::getDrawWidth(); }
    int getHeight() override { return DisplayableObject::getDrawHeight(); }
};
