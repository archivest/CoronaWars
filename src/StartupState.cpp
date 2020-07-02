#include "StartupState.h"
#include "EzyjddEngineCW4.h"
#include "ImageManager.h"
#include "GlowingTextObject.h"

StartupState::StartupState(BaseEngine* baseEngine) :
    EngineState(baseEngine),
    mRotator(2.0 * M_PI),
    mLastTime(baseEngine->getRawTime())
{
    mVirusImage = ImageManager::loadImage("images/cv1.png", true);
    mStartupSound.load("resources/startup.wav");
}

StartupState::~StartupState()
{
}

int StartupState::initialiseObjects()
{
    mStartupSound.openDevice();
    mStartupSound.play();
    getEngine()->createObjectArray(7);
    getEngine()->storeObjectInArray(0, new GlowingTextObject(getEngine(), 330, 240, "JOSHUA DIAMOND", GREY, WHITE));
    getEngine()->storeObjectInArray(1, new GlowingTextObject(getEngine(), 430, 335, "I", GREY, WHITE));
    getEngine()->storeObjectInArray(2, new GlowingTextObject(getEngine(), 415, 395, "C", GREY, WHITE));
    getEngine()->storeObjectInArray(3, new GlowingTextObject(getEngine(), 310, 455, "ENTER", GREY, WHITE));
    getEngine()->storeObjectInArray(4, new GlowingTextObject(getEngine(), 415, 515, "L", GREY, WHITE));
    getEngine()->storeObjectInArray(5, new GlowingTextObject(getEngine(), 415, 575, "H", GREY, WHITE));
    getEngine()->storeObjectInArray(6, new GlowingTextObject(getEngine(), 365, 635, "ESC", GREY, WHITE));
    getEngine()->notifyObjectsAboutKeys(false); // tell the main engine when keys are pressed rather than individual objects
    return 0;
}

bool StartupState::mapCoordinates(double& x, double& y, const SimpleImage& image)
{
    if (x < 0) return false;
    if (y < 0) return false;
    if (x >= (image.getWidth() - 0.5)) return false;
    if (y >= (image.getHeight() - 0.5)) return false;

    x -= (double)image.getWidth() / 2.0;
    y -= (double)image.getHeight() / 2.0;

    double r = mRotator.getRotation();
    double x1 = (cos(r) * x) - (sin(r) * y);
    double y1 = (sin(r) * x) + (cos(r) * y);

    x = x1;
    y = y1;

    x += (double)image.getWidth() / 2.0;
    y += (double)image.getHeight() / 2.0;

    if (x < 0) return false;
    if (y < 0) return false;
    if (x >= (image.getWidth() - 0.5)) return false;
    if (y >= (image.getHeight() - 0.5)) return false;

    return true;
}

void StartupState::setupBackgroundBuffer()
{
    // Display the startup background
    getEngine()->fillBackground(BLACK);
    mBackgroundImage = ImageManager::loadImage("images/CVBanner.png", true);
    mBackgroundImage.renderImage(getEngine()->getBackgroundSurface(), 0, 0, 0, 0,
        mBackgroundImage.getWidth(), mBackgroundImage.getHeight());

    getEngine()->drawBackgroundString(500, 175, "By", WHITE);
    getEngine()->drawBackgroundString(520, 335, "Instructions", WHITE);
    getEngine()->drawBackgroundString(520, 395, "Controls", WHITE);
    getEngine()->drawBackgroundString(520, 455, "Start New Game", WHITE);
    getEngine()->drawBackgroundString(520, 515, "Load Saved Game", WHITE);
    getEngine()->drawBackgroundString(520, 575, "High Scores", WHITE);
    getEngine()->drawBackgroundString(520, 635, "Exit", WHITE);
}

void StartupState::keyDown(int iKeyCode)
{
    switch (iKeyCode)
    {
    case SDLK_c:
        mStartupSound.stop();
        mStartupSound.closeDevice();
        getEngine()->destroyOldObjects(true);
        getEngine()->clearContents();
        static_cast<EzyjddEngineCW4*>(getEngine())->doControls();
        break;

    case SDLK_h:
        mStartupSound.stop();
        mStartupSound.closeDevice();
        getEngine()->destroyOldObjects(true);
        getEngine()->clearContents();
        static_cast<EzyjddEngineCW4*>(getEngine())->doHighScores();
        break;

    case SDLK_i:
        mStartupSound.stop();
        mStartupSound.closeDevice();
        getEngine()->destroyOldObjects(true);
        getEngine()->clearContents();
        static_cast<EzyjddEngineCW4*>(getEngine())->doInstructions();
        break;

    case SDLK_l:
        mStartupSound.stop();
        mStartupSound.closeDevice();
        if (static_cast<EzyjddEngineCW4*>(getEngine())->loadGame() == false)
        {
            mStartupSound.openDevice();
            mStartupSound.play();
        }
        break;

    case SDLK_ESCAPE:
        // quit the game
        mStartupSound.stop();
        mStartupSound.closeDevice();
        getEngine()->destroyOldObjects(true);
        getEngine()->clearContents();
        getEngine()->setExitWithCode(0);
        break;

    case SDLK_RETURN:
        // run the game
        mStartupSound.stop();
        mStartupSound.closeDevice();
        getEngine()->destroyOldObjects(true);
        getEngine()->clearContents();
        static_cast<EzyjddEngineCW4*>(getEngine())->doZoomInFirst();
        break;

    default:
        // do nothing
        break;
    }
}

void StartupState::updateAllObjects(int iCurrentTime)
{
    // make the background viruses rotate
    double diff = (double)iCurrentTime - (double)mLastTime;
    double timeFraction = diff / 4000.0;
    double r = mRotator.getRotation() - (timeFraction * (2.0 * M_PI));
    // correct radians when virus rotates more than once
    if (r < 0)
    {
       r = 2.0 * M_PI;
    }
    mRotator.setRotation(r);
    getEngine()->redrawDisplay();
    getEngine()->lockBackgroundForDrawing();
    mVirusImage.renderImageApplyingMapping(getEngine(), getEngine()->getBackgroundSurface(), 143, 34, mVirusImage.getWidth(), mVirusImage.getHeight(), this);
    mVirusImage.renderImageApplyingMapping(getEngine(), getEngine()->getBackgroundSurface(), 324, 35, mVirusImage.getWidth(), mVirusImage.getHeight(), this);
    getEngine()->unlockBackgroundForDrawing();
    mLastTime = iCurrentTime;
}
