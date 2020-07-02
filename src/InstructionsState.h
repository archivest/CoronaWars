#pragma once
#include "header.h"
#include "EngineState.h"
#include "Sound.h"

class InstructionsState final : public EngineState
{
public:
    InstructionsState(BaseEngine* baseEngine);
    ~InstructionsState();

// EngineState overrides
public:
    int initialiseObjects() override;
    void setupBackgroundBuffer() override;
    void keyDown(int iKeyCode) override;

private:
    Sound mInstructionsSound;
};
