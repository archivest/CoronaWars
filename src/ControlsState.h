#pragma once
#include "header.h"
#include "EngineState.h"

class ControlsState final : public EngineState
{
public:
    ControlsState(BaseEngine* baseEngine);
    ~ControlsState();

// EngineState overrides
public:
    void setupBackgroundBuffer() override;
    void keyDown(int iKeyCode) override;

private:
    SimpleImage mBackgroundImage;
};