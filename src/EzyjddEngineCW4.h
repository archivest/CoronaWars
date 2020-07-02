//Context class which controls access to the state interface
#pragma once

#include "header.h"
#include "BaseEngine.h"
#include "EngineState.h"
#include "HighScore.h"

class EzyjddEngineCW4 final : public BaseEngine
{
public:
    EzyjddEngineCW4();
    ~EzyjddEngineCW4();

public:
    void doStartup();
    void doInstructions();
    void doControls();
    void doHighScores();
    void doLevelInstructions(int level);
    void doRunning(int level);
    void doZoomIn(int level);
    void doZoomInFirst();
    void doZoomOut(int level);
    void doPause();
    void doUnpause();
    void doWinGame();
    void doLoseGame();
    bool loadGame();
    void saveHighScores();
    int getScore() const { return mScore; }
    void setScore(int score) { mScore = score; }
    void setHighScore(const std::string& initials);

    const std::vector<std::string>& getInstructionsText() const { return mInstructionsText; }
    const std::vector<std::string>& getWinGameText() const { return mWinGameText; }
    const std::vector<std::string>& getLoseGameText() const { return mLoseGameText; }
    const std::vector<HighScore>& getHighScores() const { return mHighScores; }

// BaseEngine virtual functions that I override
private:
    int virtInitialise() override;
    void virtSetupBackgroundBuffer() override;
    int virtInitialiseObjects() override;
    void virtKeyDown(int iKeyCode) override;
    void virtDrawStringsOnTop() override;
    void updateAllObjects(int iCurrentTime) override;
    void virtMainLoopPostUpdate() override;

private:
    void loadHighScores();
    int loadInstructionsText();
    int loadWinGameText();
    int loadLoseGameText();
    bool isHighScore();

private:
    std::vector<std::string> mInstructionsText;
    std::vector<std::string> mWinGameText;
    std::vector<std::string> mLoseGameText;
    std::vector<HighScore> mHighScores;

    std::shared_ptr<EngineState> isStartupState;
    std::shared_ptr<EngineState> isInstructionsState;
    std::shared_ptr<EngineState> isControlsState;
    std::shared_ptr<EngineState> isLevelInstructionsState;
    std::shared_ptr<EngineState> isRunningState;
    std::shared_ptr<EngineState> isZoomInFirstState;
    std::shared_ptr<EngineState> isZoomInState;
    std::shared_ptr<EngineState> isZoomOutState;
    std::shared_ptr<EngineState> isPausedState;
    std::shared_ptr<EngineState> isHighScoresState;
    std::shared_ptr<EngineState> isWinGameState;
    std::shared_ptr<EngineState> isLoseGameState;

    std::shared_ptr<EngineState> currentState;

    int mScore;
};
