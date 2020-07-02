#pragma once
#include "header.h"
#include "EngineState.h"
#include "DrawingFilters.h"
#include "Sound.h"

class ZoomInFirstState final : public EngineState
{
public:
    ZoomInFirstState(BaseEngine* baseEngine);
    ~ZoomInFirstState();

//EngineState overrides
public:
    void prepareState() override;
    int initialiseObjects() override;
    void updateAllObjects(int iCurrentTime) override;
    void setupBackgroundBuffer() override;

private:
    FilterPointsScaling m_filterScaling;
    FilterPointsTranslation m_filterTranslation;
    int mStartTime;
    int mZoomCount;
    Sound mZoomInSound;
};
