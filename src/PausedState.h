#pragma once
#include "header.h"
#include "EngineState.h"

class PausedState final : public EngineState
{
public:
    PausedState(BaseEngine* baseEngine);
    ~PausedState();

// EngineState overrides
public:
    void keyDown(int iKeyCode) override;
    void drawStringsOnTop() override;

private:
};
