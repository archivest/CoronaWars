#pragma once
#include "header.h"
#include "EngineState.h"
#include "SimpleImage.h"
#include "Sound.h"

class HighScoresState final : public EngineState
{
public:
    HighScoresState(BaseEngine* baseEngine);
    ~HighScoresState();

// EngineState overrides
public:
    void setupBackgroundBuffer() override;
    void keyDown(int iKeyCode) override;
    void prepareState() override;

private:
    void displayInitials(bool lockBackground);

private:
    SimpleImage mBackgroundImage;
    std::string mInitials;
    unsigned int mScore;
    int mEnterYPos;
    Sound mHighScoreSound;
};
