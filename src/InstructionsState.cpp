#include "InstructionsState.h"
#include "EzyjddEngineCW4.h"
#include "ScrollingTextObject.h"
#include "GameImageObject.h"

using namespace std;

InstructionsState::InstructionsState(BaseEngine* baseEngine) :
    EngineState(baseEngine)
{
    mInstructionsSound.load("resources/Instructions.wav");
}

InstructionsState::~InstructionsState()
{
}

int InstructionsState::initialiseObjects()
{
    mInstructionsSound.openDevice();
    mInstructionsSound.play();
    const vector<string>& instructions = static_cast<EzyjddEngineCW4*>(getEngine())->getInstructionsText(); // get the instructions vector reference

    getEngine()->createObjectArray(instructions.size() + 1);
    int posY = getEngine()->getWindowHeight();
    for (int i = 0; i < instructions.size(); ++i)
    {
        getEngine()->storeObjectInArray(i, new ScrollingTextObject(getEngine(), 100, posY, instructions[i].c_str(), i == 0 ? YELLOW : WHITE, 35, 1, i == (instructions.size() - 1) ? 600 : 0));
        posY += 30;
    }
    getEngine()->storeObjectInArray(instructions.size(), new GameImageObject(getEngine(), "images/CVBanner.png"));
    getEngine()->notifyObjectsAboutKeys(false); // tell the main engine when keys are pressed rather than individual objects
    return 0;
}

void InstructionsState::setupBackgroundBuffer()
{
    getEngine()->fillBackground(BLACK);
}

void InstructionsState::keyDown(int iKeyCode)
{
    switch (iKeyCode)
    {
    case SDLK_RETURN:
        // return to the startup state
        mInstructionsSound.stop();
        mInstructionsSound.closeDevice();
        getEngine()->destroyOldObjects(true);
        getEngine()->clearContents();
        static_cast<EzyjddEngineCW4*>(getEngine())->doStartup();
        break;

    default:
        // do nothing
        break;
    }
}
