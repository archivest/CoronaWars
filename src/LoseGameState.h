#pragma once
#include "header.h"
#include "EngineState.h"
#include "Sound.h"

class LoseGameState final : public EngineState
{
public:
    LoseGameState(BaseEngine* baseEngine);
    ~LoseGameState();

// EngineState overrides
public:
    int initialiseObjects() override;
    void setupBackgroundBuffer() override;
    void keyDown(int iKeyCode) override;
    void updateAllObjects(int iCurrentTime) override;

private:
    int mStartTime;
    Sound mLoseGameSound;
};
