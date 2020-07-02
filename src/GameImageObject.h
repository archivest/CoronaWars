#pragma once

#include "header.h"
#include "MappableImageObject.h"
#include "CoordinateMapping.h"
#include "BaseEngine.h"

class GameImageObject : public MappableImageObject, public CoordinateMapping
{
public:
    GameImageObject(BaseEngine* pEngine, const std::string& strURL, int xStart = 0, int yStart = 0, bool isMaskable = false, int maskColour = BLACK, bool isDraggable = false, bool useTopLeftFor00 = true, bool bVisible = true)
        : MappableImageObject(this, xStart, yStart, pEngine, strURL, useTopLeftFor00, bVisible),
        mIsMaskable(isMaskable),
        mMaskColour(maskColour),
        mIsDraggable(isDraggable),
        mRotator(0.0),
        mXDbl((double)xStart),
        mYDbl((double)yStart)
    {
    }

    virtual ~GameImageObject() {}

    // CoordinateMapping
    bool mapCoordinates(double& x, double& y, const SimpleImage& image) override
    {
        if (x < 0) return false;
        if (y < 0) return false;
        if (x >= (image.getWidth() - 0.5)) return false;
        if (y >= (image.getHeight() - 0.5)) return false;

        // Shift offset to the centre of the image, so we can rotate around centre
        x -= image.getWidth() / 2;
        y -= image.getHeight() / 2;

        // Rotate it
        double r = mRotator.getRotation();
        double x1 = (::cos(r) * x) - (sin(r) * y);
        double y1 = (::sin(r) * x) + (cos(r) * y);

        x = x1;
        y = y1;

        // Shift offset back to the corner
        x += image.getWidth() / 2;
        y += image.getHeight() / 2;

        if (x < 0) return false;
        if (y < 0) return false;
        if (x >= (image.getWidth() - 0.5)) return false;
        if (y >= (image.getHeight() - 0.5)) return false;

        return true;
    }

    bool changePixelColour(int x, int y, int& colour, DrawingSurface* pTarget) override
    {
        if (mIsMaskable)
        {
            return colour != mMaskColour;
        }
        return true;
    }

    void virtMouseDown(int iButton, int iX, int iY) override
    {
        if (mIsDraggable)
        {
            MappableImageObject::virtMouseDown(iButton, iX, iY);
        }
    }

    double getXDbl() const
    {
        return mXDbl;
    }

    double getYDbl() const
    {
        return mYDbl;
    }

    CoordinateMappingRotate& getRotator()
    {
        return mRotator;
    }

    void setPositionDbl(double xDbl, double yDbl)
    {
        if (yDbl < 0.0)
        {
            int i = 0;
        }
        mXDbl = xDbl;
        mYDbl = yDbl;
        MappableImageObject::setPosition((int)xDbl, (int)yDbl);
    }

private:
    // have made this private and not virtual so it can't be called by derived classes.
    // Instead they use my double version above
    void setPosition(int x, int y) override
    {
        MappableImageObject::setPosition((int)x, (int)y);
    }

private:
    bool mIsMaskable;
    int mMaskColour;
    bool mIsDraggable;
    CoordinateMappingRotate mRotator;
    double mXDbl; // need to store the double versions of the base class's integer x and y (m_icurrentX and Y)
    double mYDbl;
};
