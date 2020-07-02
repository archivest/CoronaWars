#pragma once
#include "header.h"
#include "EngineState.h"
#include "DrawingFilters.h"
#include "Sound.h"

class ZoomInState final : public EngineState
{
public:
    ZoomInState(BaseEngine* baseEngine);
    ~ZoomInState();

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
    Sound mZoomInSound;
};
