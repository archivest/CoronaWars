#pragma once
#include "header.h"
#include "EngineState.h"
#include "DrawingFilters.h"
#include "Sound.h"

class ZoomOutState final : public EngineState
{
public:
    ZoomOutState(BaseEngine* baseEngine);
    ~ZoomOutState();

// EngineState overrides
public:
    int initialiseObjects() override;
    void updateAllObjects(int iCurrentTime) override;

public:
    void prepareState(int level);

private:
    int mLevel;
    FilterPointsScaling m_filterScaling;
    FilterPointsTranslation m_filterTranslation;
    int mStartTime;
    int mZoomCount;
    Sound mZoomOutSound;
};
