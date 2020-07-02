#include "WinGameState.h"
#include "EzyjddEngineCW4.h"
#include "ScrollingTextObject.h"
#include "GameImageObject.h"

using namespace std;

WinGameState::WinGameState(BaseEngine* baseEngine) :
    EngineState(baseEngine),
    mStartTime(0),
    mScore(0)
{
    mWinGameSound.load("resources/WinGame.wav");
}

WinGameState::~WinGameState()
{
}

int WinGameState::initialiseObjects()
{
    mWinGameSound.openDevice();
    mWinGameSound.play();
    mStartTime = 0;
    const vector<string>& winGameText = static_cast<EzyjddEngineCW4*>(getEngine())->getWinGameText(); 

    getEngine()->createObjectArray(winGameText.size() + 1);
    int posY = getEngine()->getWindowHeight();
    for (int i = 0; i < winGameText.size(); ++i)
    {
        getEngine()->storeObjectInArray(i, new ScrollingTextObject(getEngine(), 75, posY, winGameText[i].c_str(), i == 0 ? 0xfff200 : 0xffffff, 35, 1, 0));
        posY += 30;
    }
    //mpText = new FlashingTextObject(baseEngine, 650, 500);
    getEngine()->storeObjectInArray(winGameText.size(), new GameImageObject(getEngine(), "images/CVBanner.png"));
    getEngine()->notifyObjectsAboutKeys(false); // tell the main engine when keys are pressed rather than individual objects

    return 0;
}

void WinGameState::keyDown(int iKeyCode)
{
    if (iKeyCode == SDLK_RETURN)
    {
        mWinGameSound.stop();
        mWinGameSound.closeDevice();
        getEngine()->destroyOldObjects(true);
        getEngine()->clearContents();
        static_cast<EzyjddEngineCW4*>(getEngine())->doHighScores();
    }
}

void WinGameState::prepareState(int score)
{
    mScore = score;
}

void WinGameState::setupBackgroundBuffer()
{
    getEngine()->fillBackground(BLACK);
}

void WinGameState::updateAllObjects(int iCurrentTime)
{
    if (mStartTime == 0)
    {
        mStartTime = iCurrentTime;
    }
    if (iCurrentTime > mStartTime + 25000)
    {
        mWinGameSound.stop();
        mWinGameSound.closeDevice();
        getEngine()->destroyOldObjects(true);
        getEngine()->clearContents();
        static_cast<EzyjddEngineCW4*>(getEngine())->doHighScores();
    }
}
