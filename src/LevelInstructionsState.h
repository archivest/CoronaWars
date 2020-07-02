#pragma once
#include "header.h"
#include "EngineState.h"
#include "SimpleImage.h"
#include "Sound.h"

class LevelInstructionsState final : public EngineState
{
public:
    LevelInstructionsState(BaseEngine* baseEngine);
    ~LevelInstructionsState();

// EngineState overrides
public:
    int initialiseObjects() override;
    void setupBackgroundBuffer() override;
    void keyDown(int iKeyCode) override;

public:
    void prepareState(int level);

private:
    int mLevel;
    static const char* mLevelNames[4];
    SimpleImage mBackgroundImage;
    Sound mLevel1Sound;
    Sound mLevel2Sound;
    Sound mLevel3Sound;
    Sound mLevel4Sound;
};
