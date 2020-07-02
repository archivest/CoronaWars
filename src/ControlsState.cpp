#include "ControlsState.h"
#include "EzyjddEngineCW4.h"
#include "StartupState.h"

using namespace std;

ControlsState::ControlsState(BaseEngine* baseEngine) :
    EngineState(baseEngine)
{
    mBackgroundImage = ImageManager::loadImage("images/CVBanner.png", true);
}

ControlsState::~ControlsState()
{
}

void ControlsState::setupBackgroundBuffer()
{
    getEngine()->fillBackground(BLACK);
    mBackgroundImage.renderImage(getEngine()->getBackgroundSurface(), 0, 0, 0, 0,
        mBackgroundImage.getWidth(), mBackgroundImage.getHeight());
    getEngine()->drawBackgroundString(175, 200, "ARROW KEYS - Control Ship", WHITE);
    getEngine()->drawBackgroundString(205, 250, "SPACE BAR - Launch missile", WHITE);
    getEngine()->drawBackgroundString(419, 300, "M - Lay Mine", WHITE);
    getEngine()->drawBackgroundString(425, 350, "P - Pause Game", WHITE);
    getEngine()->drawBackgroundString(425, 400, "S - Save Game", WHITE);
    getEngine()->drawBackgroundString(367, 450, "ESC - End Game", WHITE);
    getEngine()->drawBackgroundString(100, 600, "Press ENTER to return to Start", WHITE);
}

void ControlsState::keyDown(int iKeyCode)
{
    switch (iKeyCode)
    {
    case SDLK_RETURN:
        // return to the startup state
        getEngine()->destroyOldObjects(true);
        getEngine()->clearContents();
        static_cast<EzyjddEngineCW4*>(getEngine())->doStartup();
        break;

    default:
        // do nothing
        break;
    }
}
