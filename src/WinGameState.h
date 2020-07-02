#pragma once
#include "header.h"
#include "EngineState.h"
#include "Sound.h"

class WinGameState final : public EngineState
{
public:
    WinGameState(BaseEngine* baseEngine);
    ~WinGameState();

//EngineState overrides
public:
    int initialiseObjects() override;
    void setupBackgroundBuffer() override;
    void keyDown(int iKeyCode) override;
    void updateAllObjects(int iCurrentTime) override;

public:
    void prepareState(int score);

private:
    int mStartTime;
    int mScore;
    Sound mWinGameSound;
};
