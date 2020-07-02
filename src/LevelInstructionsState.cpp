#include "LevelInstructionsState.h"
#include "EzyjddEngineCW4.h"
#include "FlashingTextObject.h"

const char* LevelInstructionsState::mLevelNames[4] = { "Level 1 - Colon", "Level 2 - Veins", "Level 3 - Heart", "Level 4 - Lungs" };

LevelInstructionsState::LevelInstructionsState(BaseEngine* baseEngine) :
    EngineState(baseEngine),
    mLevel(0)
{
    mLevel1Sound.load("resources/Level1.wav");
    mLevel2Sound.load("resources/Level2.wav");
    mLevel3Sound.load("resources/Level3.wav");
    mLevel4Sound.load("resources/Level4.wav");
}

LevelInstructionsState::~LevelInstructionsState()
{
}

void LevelInstructionsState::prepareState(int level)
{
    mLevel = level;
    switch (mLevel)
    {
    case 0:
        mLevel1Sound.openDevice();
        break;
    case 1:
        mLevel2Sound.openDevice();
        break;
    case 2:
        mLevel3Sound.openDevice();
        break;
    case 3:
        mLevel4Sound.openDevice();
        break;
    }
}

int LevelInstructionsState::initialiseObjects()
{
    switch (mLevel)
    {
    case 0:
        mLevel1Sound.play();
        getEngine()->createObjectArray(9);
        getEngine()->storeObjectInArray(0, new FlashingTextObject(getEngine(), 350, 50, mLevelNames[mLevel], YELLOW, BLACK, FLASH_RATE));
        getEngine()->storeObjectInArray(1, new SimpleTextObject(getEngine(), 270, 200, "INJECTION COMPLETE", WHITE));
        getEngine()->storeObjectInArray(2, new SimpleTextObject(getEngine(), 280, 300, "You've arrive safely.", WHITE));
        getEngine()->storeObjectInArray(3, new SimpleTextObject(getEngine(), 230, 350, "Not too many viruses here.", WHITE));
        getEngine()->storeObjectInArray(4, new SimpleTextObject(getEngine(), 180, 400, "Better get used to controlling", WHITE));
        getEngine()->storeObjectInArray(5, new SimpleTextObject(getEngine(), 240, 450, "your ship before the real", WHITE));
        getEngine()->storeObjectInArray(6, new SimpleTextObject(getEngine(), 380, 500, "battle begins!", WHITE));
        getEngine()->storeObjectInArray(7, new SimpleTextObject(getEngine(), 420, 600, "Get ready!", WHITE));
        getEngine()->storeObjectInArray(8, new SimpleTextObject(getEngine(), 280, 700, "Press ENTER to Start", YELLOW));
        break;

    case 1:
        mLevel2Sound.play();
        getEngine()->createObjectArray(7);
        getEngine()->storeObjectInArray(0, new FlashingTextObject(getEngine(), 350, 50, mLevelNames[mLevel], YELLOW, BLACK, FLASH_RATE));
        getEngine()->storeObjectInArray(1, new SimpleTextObject(getEngine(), 260, 200, "Looks like the viruses are", WHITE));
        getEngine()->storeObjectInArray(2, new SimpleTextObject(getEngine(), 210, 250, "using the veins to get around!", WHITE));
        getEngine()->storeObjectInArray(3, new SimpleTextObject(getEngine(), 230, 300, "There's bound to be more of", WHITE));
        getEngine()->storeObjectInArray(4, new SimpleTextObject(getEngine(), 430, 350, "them here.", WHITE));
        getEngine()->storeObjectInArray(5, new SimpleTextObject(getEngine(), 430, 450, "Watch out!", WHITE));
        getEngine()->storeObjectInArray(6, new SimpleTextObject(getEngine(), 280, 600, "Press ENTER to Start", YELLOW));
        break;

    case 2:
        mLevel3Sound.play();
        getEngine()->createObjectArray(7);
        getEngine()->storeObjectInArray(0, new FlashingTextObject(getEngine(), 350, 50, mLevelNames[mLevel], YELLOW, BLACK, FLASH_RATE));
        getEngine()->storeObjectInArray(1, new SimpleTextObject(getEngine(), 230, 200, "You've reached the heart!", WHITE));
        getEngine()->storeObjectInArray(2, new SimpleTextObject(getEngine(), 220, 250, "Currents are strong here", WHITE));
        getEngine()->storeObjectInArray(3, new SimpleTextObject(getEngine(), 260, 300, "so it'll be difficult to", WHITE));
        getEngine()->storeObjectInArray(4, new SimpleTextObject(getEngine(), 320, 350, "control your ship.", WHITE));
        getEngine()->storeObjectInArray(5, new SimpleTextObject(getEngine(), 400, 450, "Be careful!", WHITE));
        getEngine()->storeObjectInArray(6, new SimpleTextObject(getEngine(), 280, 700, "Press ENTER to Start", YELLOW));
        break;

    case 3:
        mLevel4Sound.play();
        getEngine()->createObjectArray(11);
        getEngine()->storeObjectInArray(0, new FlashingTextObject(getEngine(), 350, 50, mLevelNames[mLevel], YELLOW, BLACK, FLASH_RATE));
        getEngine()->storeObjectInArray(1, new SimpleTextObject(getEngine(), 230, 150, "You've reached the lungs!", WHITE));
        getEngine()->storeObjectInArray(2, new SimpleTextObject(getEngine(), 50, 230, "It's a hotbed of viruses, but your ship", WHITE));
        getEngine()->storeObjectInArray(3, new SimpleTextObject(getEngine(), 60, 270, "has been sent an over-the-air update.", WHITE));
        getEngine()->storeObjectInArray(4, new SimpleTextObject(getEngine(), 180, 350, "You now have Smart Missiles!", WHITE));

        getEngine()->storeObjectInArray(5, new SimpleTextObject(getEngine(), 50, 430, "Click the left mouse button to launch a", WHITE));
        getEngine()->storeObjectInArray(6, new SimpleTextObject(getEngine(), 50, 470, "Smart Missile, but watch out - it's the", WHITE));
        getEngine()->storeObjectInArray(7, new SimpleTextObject(getEngine(), 60, 510, "beta version and there's been no time", WHITE));
        getEngine()->storeObjectInArray(8, new SimpleTextObject(getEngine(), 400, 550, "to test it!", WHITE));

        getEngine()->storeObjectInArray(9, new SimpleTextObject(getEngine(), 320, 630, "Take a deep breath!", WHITE));
        getEngine()->storeObjectInArray(10, new SimpleTextObject(getEngine(), 280, 700, "Press ENTER to Start", YELLOW));
        break;
    }

    getEngine()->notifyObjectsAboutKeys(false); // tell the main engine when keys are pressed rather than individual objects
    return 0;
}

void LevelInstructionsState::setupBackgroundBuffer()
{
    // load correct background for this level
    std::string url = "images/level" + std::to_string(mLevel + 1) + "background.png";
    mBackgroundImage = ImageManager::loadImage(url.c_str(), true);
    mBackgroundImage.renderImage(getEngine()->getBackgroundSurface(), 0, 0, 0, 0,
        mBackgroundImage.getWidth(), mBackgroundImage.getHeight());
}

void LevelInstructionsState::keyDown(int iKeyCode)
{
    switch (iKeyCode)
    {
        case SDLK_RETURN:
            switch (mLevel)
            {
                case 0:
                    mLevel1Sound.stop();
                    mLevel1Sound.closeDevice();
                    break;

                case 1:
                    mLevel2Sound.stop();
                    mLevel2Sound.closeDevice();
                    break;

                case 2:
                    mLevel3Sound.stop();
                    mLevel3Sound.closeDevice();
                    break;

                case 3:
                    mLevel4Sound.stop();
                    mLevel4Sound.closeDevice();
                    break;
            }
            getEngine()->destroyOldObjects(true);
            getEngine()->clearContents();
            static_cast<EzyjddEngineCW4*>(getEngine())->doRunning(mLevel);
            break;

        default:
            // do nothing
        break;
    }
}
