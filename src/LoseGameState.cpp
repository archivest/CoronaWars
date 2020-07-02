#include "LoseGameState.h"
#include "EzyjddEngineCW4.h"
#include "ScrollingTextObject.h"
#include "GameImageObject.h"

using namespace std;

LoseGameState::LoseGameState(BaseEngine* baseEngine) :
    EngineState(baseEngine),
    mStartTime(0)
{
    mLoseGameSound.load("resources/LoseGame.wav");
}

LoseGameState::~LoseGameState()
{
}

int LoseGameState::initialiseObjects()
{
    mLoseGameSound.openDevice();
    mLoseGameSound.play();
    mStartTime = 0;
    const vector<string>& loseGameText = static_cast<EzyjddEngineCW4*>(getEngine())->getLoseGameText(); 

    getEngine()->createObjectArray(loseGameText.size() + 1);
    int posY = getEngine()->getWindowHeight();
    for (int i = 0; i < loseGameText.size(); ++i)
    {
        getEngine()->storeObjectInArray(i, new ScrollingTextObject(getEngine(), 75, posY, loseGameText[i].c_str(), i == 0 ? YELLOW : WHITE, 35, 1, 0));
        posY += 30;
    }
    getEngine()->storeObjectInArray(loseGameText.size(), new GameImageObject(getEngine(), "images/CVBanner.png"));
    getEngine()->notifyObjectsAboutKeys(false); // tell the main engine when keys are pressed rather than individual objects
    return 0;
}

void LoseGameState::setupBackgroundBuffer()
{
    getEngine()->fillBackground(BLACK);
}

void LoseGameState::keyDown(int iKeyCode)
{
    if (iKeyCode == SDLK_RETURN)
    {
        getEngine()->destroyOldObjects(true);
        getEngine()->clearContents();
        mLoseGameSound.stop();
        mLoseGameSound.closeDevice();
        static_cast<EzyjddEngineCW4*>(getEngine())->doStartup();
    }
}

void LoseGameState::updateAllObjects(int iCurrentTime)
{
    if (mStartTime == 0)
    {
        mStartTime = iCurrentTime;
    }
    if (iCurrentTime > mStartTime + 20000)
    {
        getEngine()->destroyOldObjects(true);
        getEngine()->clearContents();
        mLoseGameSound.stop();
        mLoseGameSound.closeDevice();
        static_cast<EzyjddEngineCW4*>(getEngine())->doStartup();
    }
}
