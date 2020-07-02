#pragma once
#include "header.h"
#include "EngineState.h"
#include "SimpleImage.h"
#include "CoordinateMapping.h"
#include "Sound.h"

class StartupState final : public EngineState, public CoordinateMapping
{
public:
    StartupState(BaseEngine* baseEngine);
    ~StartupState();

// EngineState overrides
public:
    int initialiseObjects() override;
    void setupBackgroundBuffer() override;
    void keyDown(int iKeyCode) override;
    void updateAllObjects(int iCurrentTime) override;

// CoordinateMapping overrides
public:
    bool mapCoordinates(double& x, double& y, const SimpleImage& image) override;

private:
    SimpleImage mBackgroundImage;
    SimpleImage mVirusImage;
    CoordinateMappingRotate mRotator;
    int mLastTime;
    Sound mStartupSound;
};
