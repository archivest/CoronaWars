#include "EzyjddEngineCW4.h"
#include "StartupState.h"
#include "InstructionsState.h"
#include "ControlsState.h"
#include "LevelInstructionsState.h"
#include "RunningState.h"
#include "ZoomInFirstState.h"
#include "ZoomInState.h"
#include "ZoomOutState.h"
#include "PausedState.h"
#include "HighScoresState.h"
#include "WinGameState.h"
#include "LoseGameState.h"

EzyjddEngineCW4::EzyjddEngineCW4()
{
    SDL_Init(SDL_INIT_AUDIO);
    // seed random number generator with current time so we get different random numbers each time we run
    time_t curTime;
    std::time(&curTime);
    std::srand(curTime);
}

EzyjddEngineCW4::~EzyjddEngineCW4()
{
    SDL_CloseAudio();
    SDL_Quit();
}

void EzyjddEngineCW4::doControls()
{
    currentState = isControlsState;
    currentState->prepareState();
    lockAndSetupBackground();
}

void EzyjddEngineCW4::doHighScores()
{
    currentState = isHighScoresState;
    currentState->prepareState();
    lockAndSetupBackground();
}

void EzyjddEngineCW4::doInstructions()
{
    currentState = isInstructionsState;
    currentState->prepareState();
    lockAndSetupBackground();
    currentState->initialiseObjects();
}

void EzyjddEngineCW4::doLevelInstructions(int level)
{
    currentState = isLevelInstructionsState;
    std::static_pointer_cast<LevelInstructionsState>(currentState)->prepareState(level);
    lockAndSetupBackground();
    currentState->initialiseObjects();
}

void EzyjddEngineCW4::doLoseGame()
{
    currentState = isLoseGameState;
    currentState->prepareState();
    lockAndSetupBackground();
    currentState->initialiseObjects();
}

void EzyjddEngineCW4::doPause()
{
    BaseEngine::pause();
    currentState = isPausedState;
}

void EzyjddEngineCW4::doRunning(int level)
{
    currentState = isRunningState;
    std::static_pointer_cast<RunningState>(currentState)->prepareState(level);
    lockAndSetupBackground();
    currentState->initialiseObjects();
}

void EzyjddEngineCW4::doStartup()
{
    mScore = 0;
    currentState = isStartupState;
    currentState->prepareState();
    lockAndSetupBackground();
    currentState->initialiseObjects();
}

void EzyjddEngineCW4::doUnpause()
{
    currentState = isRunningState;
    BaseEngine::unpause();
}

void EzyjddEngineCW4::doWinGame()
{
    currentState = isWinGameState;
    currentState->prepareState();
    lockAndSetupBackground();
    currentState->initialiseObjects();
}

void EzyjddEngineCW4::doZoomIn(int level)
{
    currentState = isZoomInState;
    std::static_pointer_cast<ZoomInState>(currentState)->prepareState(level);
    lockAndSetupBackground();
    currentState->initialiseObjects();
}

void EzyjddEngineCW4::doZoomInFirst()
{
    currentState = isZoomInFirstState;
    currentState->prepareState();
    lockAndSetupBackground();
    currentState->initialiseObjects();
}

void EzyjddEngineCW4::doZoomOut(int level)
{
    currentState = isZoomOutState;
    std::static_pointer_cast<ZoomOutState>(currentState)->prepareState(level);
    lockAndSetupBackground();
    currentState->initialiseObjects();
}

bool EzyjddEngineCW4::isHighScore()
{
    for (int i = 0; i < NUM_HIGH_SCORES; i++)
    {
        if (mScore > mHighScores[i].getScore())
        {
            return true;
        }
    }
    return false;
}

bool EzyjddEngineCW4::loadGame()
{
    if (!std::static_pointer_cast<RunningState>(isRunningState)->loadGame(false))
    {
        return false;
    }
    currentState = isRunningState;
    return true;
}

void EzyjddEngineCW4::loadHighScores()
{
    std::fstream file;
    file.open("HighScores.txt", std::ios::in);
    mHighScores.clear();
    if (!file.is_open())
    {
        // no high scores file, so no high scores
        return;
    }

    // load high scores from file - file is already sorted from highest to lowest so no need to sort
    std::string initials;
    unsigned int score;
    for (int i = 0; i < NUM_HIGH_SCORES; ++i)
    {
        file >> initials >> score;
        if (file.eof())
        {
            break;
        }
        mHighScores.emplace_back(HighScore(initials, score));
    }

    file.close();
    return;
}

int EzyjddEngineCW4::loadInstructionsText()
{
    std::string line;
    std::fstream file;
    file.open("resources/Instructions.txt", std::ios::in);
    if (!file.is_open())
    {
        std::cerr << "Error Can't open instructions file" << std::endl;
        return 1;
    }

    while (getline(file, line))
    {
        mInstructionsText.push_back(line);
    }

    file.close();
    return 0;
}

int EzyjddEngineCW4::loadLoseGameText()
{
    std::string line;
    std::fstream file;
    file.open("resources/LoseGame.txt", std::ios::in);
    if (!file.is_open())
    {
        std::cerr << "Error Can't open lose game text file" << std::endl;
        return 1;
    }

    while (getline(file, line))
    {
        mLoseGameText.push_back(line);
    }

    file.close();
    return 0;
}

int EzyjddEngineCW4::loadWinGameText()
{
    std::string line;
    std::fstream file;
    file.open("resources/WinGame.txt", std::ios::in);
    if (!file.is_open())
    {
        std::cerr << "Error Can't open win game text file" << std::endl;
        return 1;
    }

    while (getline(file, line))
    {
        mWinGameText.push_back(line);
    }

    file.close();
    return 0;
}

void EzyjddEngineCW4::saveHighScores()
{
    std::fstream file;
    file.open("HighScores.txt", std::ios::out | std::ios::trunc);
    for (int i = 0; i < mHighScores.size(); i++)
    {
        file << mHighScores[i].getInitials() << " " << mHighScores[i].getScore() << std::endl;
    }
    file.close();
}

void EzyjddEngineCW4::setHighScore(const std::string& initials)
{
    // is the vector empty (no high scores)
    if (mHighScores.empty())
    {
        // yes - add the first one
        mHighScores.emplace_back(HighScore(initials, mScore));
    }
    else
    {
        // insert in the middle of the table or add it at the end
        bool hsDone = false;
        for (auto it = mHighScores.begin(); it != mHighScores.end(); ++it)
        {
            const HighScore& hs = *it;
            if (mScore > hs.getScore())
            {
                // insert the high score in the correct place
                mHighScores.insert(it, HighScore(initials, mScore));
                hsDone = true;
                if (mHighScores.size() > NUM_HIGH_SCORES)
                {
                    // remove the final high score so the table stays at the same size
                    mHighScores.pop_back();
                }
                break;
            }
        }
        // was the score inserted in the middle and is there space at the end?
        if (!hsDone && mHighScores.size() < NUM_HIGH_SCORES)
        {
            // yes - add it at the end
            mHighScores.emplace_back(HighScore(initials, mScore));
        }
    }

    // write the high scores
    saveHighScores();
}

void EzyjddEngineCW4::updateAllObjects(int iCurrentTime)
{
    // update all the objects at the state level (before they each get told to update themselves individually).
    currentState->updateAllObjects(iCurrentTime);
    BaseEngine::updateAllObjects(iCurrentTime);
}

void EzyjddEngineCW4::virtDrawStringsOnTop()
{
    if (currentState == isPausedState)
    {
        isRunningState->drawStringsOnTop();
    }
    currentState->drawStringsOnTop();
}

int EzyjddEngineCW4::virtInitialise()
{
    // Create my state objects - one for each state
    isStartupState = std::make_shared<StartupState>(this);
    isInstructionsState = std::make_shared<InstructionsState>(this);
    isControlsState = std::make_shared<ControlsState>(this);
    isLevelInstructionsState = std::make_shared<LevelInstructionsState>(this);
    isRunningState = std::make_shared<RunningState>(this);
    isZoomInFirstState = std::make_shared<ZoomInFirstState>(this);
    isZoomInState = std::make_shared<ZoomInState>(this);
    isZoomOutState = std::make_shared<ZoomOutState>(this);
    isPausedState = std::make_shared<PausedState>(this);
    isHighScoresState = std::make_shared<HighScoresState>(this);
    isWinGameState = std::make_shared<WinGameState>(this);
    isLoseGameState = std::make_shared<LoseGameState>(this);

    // set the initial state
    mScore = 0;
    currentState = isStartupState;

    // load instructions from file
    if (loadInstructionsText() != 0)
    {
        return 1;
    }

    // load the win game text from file
    if (loadWinGameText() != 0)
    {
        return 1;
    }

    // load the lose game text from file
    if (loadLoseGameText() != 0)
    {
        return 1;
    }

    // load the high scores from file
    loadHighScores();

    // call the base class so it can do its stuff
    return BaseEngine::virtInitialise();
}

int EzyjddEngineCW4::virtInitialiseObjects()
{
    // gets called back here during virtInitialise
    int res = BaseEngine::virtInitialiseObjects(); // call the base class
    if (res != 0)
    {
        return res;
    }

    return currentState->initialiseObjects(); // call the current state's implementation
}

void EzyjddEngineCW4::virtKeyDown(int iKeyCode)
{
    currentState->keyDown(iKeyCode);
    BaseEngine::virtKeyDown(iKeyCode);
}

void EzyjddEngineCW4::virtMainLoopPostUpdate()
{
    currentState->mainLoopPostUpdate();
    BaseEngine::virtMainLoopPostUpdate();
}

void EzyjddEngineCW4::virtSetupBackgroundBuffer()
{
    // This is called when we need to draw the background, so draw the background for the current state only
    currentState->setupBackgroundBuffer();
    // don't the base class otherwise it will draw the background with the default yellow
}
