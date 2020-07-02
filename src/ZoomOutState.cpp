#include "ZoomOutState.h"
#include "EzyjddEngineCW4.h"
#include "GameImageObject.h"

using namespace std;

ZoomOutState::ZoomOutState(BaseEngine* baseEngine) :
    EngineState(baseEngine),
    mLevel(0),
    m_filterScaling(0, 0, baseEngine),
    m_filterTranslation(0, 0, &m_filterScaling),
    mStartTime(0),
    mZoomCount(0)
{
    mZoomOutSound.load("resources/ZoomOut.wav");
}

ZoomOutState::~ZoomOutState()
{
}

void ZoomOutState::prepareState(int level)
{
    mLevel = level;
    mStartTime = 0;
    mZoomCount = 0;
}

int ZoomOutState::initialiseObjects()
{
    mZoomOutSound.openDevice();
    mZoomOutSound.play();

    // initialise filters used for zooming
    getEngine()->getBackgroundSurface()->setDrawPointsFilter(&m_filterTranslation);
    getEngine()->getForegroundSurface()->setDrawPointsFilter(&m_filterTranslation);

    // display the background image in the foreground for zooming
    getEngine()->createObjectArray(1);
    std::string url = "images/level" + std::to_string(mLevel + 1) + "background.png";
    getEngine()->storeObjectInArray(0, new GameImageObject(getEngine(), url));
    for (int i = 0; i < 20; ++i)
    {
        int iOldCentreX = getEngine()->convertClickedToVirtualPixelXPosition(getEngine()->getWindowWidth() / 2);
        int iOldCentreY = getEngine()->convertClickedToVirtualPixelYPosition(getEngine()->getWindowHeight() / 2);
        m_filterScaling.stretch();
        int iNewCentreX = getEngine()->convertClickedToVirtualPixelXPosition(getEngine()->getWindowWidth() / 2);
        int iNewCentreY = getEngine()->convertClickedToVirtualPixelYPosition(getEngine()->getWindowHeight() / 2);
        m_filterTranslation.changeOffset(iNewCentreX - iOldCentreX, iNewCentreY - iOldCentreY);
        getEngine()->redrawDisplay(); // Force total redraw
    }
    return 0;
}

void ZoomOutState::updateAllObjects(int iCurrentTime)
{
    if (mStartTime == 0)
    {
        // zoom in but don't display any changes
        mStartTime = iCurrentTime;
        mZoomCount = 0;
    }
    else
    {
        double diff = (double)iCurrentTime - (double)mStartTime;
        if (diff > ZOOM_DURATION_MS)
        {
            diff = ZOOM_DURATION_MS;
        }
        int targetZoom = (int)((diff / ZOOM_DURATION_MS) * ZOOM_DEPTH);
        while (mZoomCount < targetZoom)
        {
            int iOldCentreX = getEngine()->convertClickedToVirtualPixelXPosition(getEngine()->getWindowWidth() / 2);
            int iOldCentreY = getEngine()->convertClickedToVirtualPixelYPosition(getEngine()->getWindowHeight() / 2);
            m_filterScaling.compress();
            int iNewCentreX = getEngine()->convertClickedToVirtualPixelXPosition(getEngine()->getWindowWidth() / 2);
            int iNewCentreY = getEngine()->convertClickedToVirtualPixelYPosition(getEngine()->getWindowHeight() / 2);
            m_filterTranslation.changeOffset(iNewCentreX - iOldCentreX, iNewCentreY - iOldCentreY);
            mZoomCount++;
        }
        getEngine()->redrawDisplay(); // Force total redraw
        if (mZoomCount == (int)ZOOM_DEPTH)
        {
            mZoomOutSound.stop();
            mZoomOutSound.closeDevice();
            getEngine()->destroyOldObjects(true);
            getEngine()->clearContents();
            static_cast<EzyjddEngineCW4*>(getEngine())->doLevelInstructions(mLevel);
        }
    }
}
