#include "PausedState.h"
#include "EzyjddEngineCW4.h"

using namespace std;

PausedState::PausedState(BaseEngine* baseEngine) :
    EngineState(baseEngine)
{
}

PausedState::~PausedState()
{
}

void PausedState::keyDown(int iKeyCode)
{
    switch (iKeyCode)
    {
    case SDLK_p:
        // unpause the game
        static_cast<EzyjddEngineCW4*>(getEngine())->doUnpause();
        break;

    default:
        // do nothing
        break;
    }
}

void PausedState::drawStringsOnTop()
{
    getEngine()->drawForegroundString(350, 350, "*** PAUSED ***", YELLOW);
}
