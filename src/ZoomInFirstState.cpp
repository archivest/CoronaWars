#include "ZoomInFirstState.h"
#include "EzyjddEngineCW4.h"
#include "GameImageObject.h"

using namespace std;

ZoomInFirstState::ZoomInFirstState(BaseEngine* baseEngine) :
    EngineState(baseEngine),
    m_filterScaling(0, 0, baseEngine),
    m_filterTranslation(0, 0, &m_filterScaling),
    mStartTime(0),
    mZoomCount(0)
{
    mZoomInSound.load("resources/ZoomIn.wav");
}

ZoomInFirstState::~ZoomInFirstState()
{
}

void ZoomInFirstState::prepareState()
{
    mStartTime = 0;
    mZoomCount = 0;
}

void ZoomInFirstState::setupBackgroundBuffer()
{
    getEngine()->fillBackground(BLACK);
}

int ZoomInFirstState::initialiseObjects()
{
    mZoomInSound.openDevice();
    mZoomInSound.play();
    // initialise filters used for zooming
    getEngine()->getBackgroundSurface()->setDrawPointsFilter(&m_filterTranslation);
    getEngine()->getForegroundSurface()->setDrawPointsFilter(&m_filterTranslation);

    // display the background image in the foreground for zooming
    getEngine()->createObjectArray(1);
    getEngine()->storeObjectInArray(0, new GameImageObject(getEngine(), "images/level1background.png"));

    for (int i = 0; i < 20; ++i)
    {
        int iOldCentreX = getEngine()->convertClickedToVirtualPixelXPosition(getEngine()->getWindowWidth() / 2);
        int iOldCentreY = getEngine()->convertClickedToVirtualPixelYPosition(getEngine()->getWindowHeight() / 2);
        m_filterScaling.compress();
        int iNewCentreX = getEngine()->convertClickedToVirtualPixelXPosition(getEngine()->getWindowWidth() / 2);
        int iNewCentreY = getEngine()->convertClickedToVirtualPixelYPosition(getEngine()->getWindowHeight() / 2);
        m_filterTranslation.changeOffset(iNewCentreX - iOldCentreX, iNewCentreY - iOldCentreY);
    }
    return 0;
}

void ZoomInFirstState::updateAllObjects(int iCurrentTime)
{
    // zoom into foreground using iCurrentTime to control how quickly the zoom occurs
    // first time in, mLastTime will be 0, so initialise it to the current time
    if (mStartTime == 0)
    {
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
            m_filterScaling.stretch();
            int iNewCentreX = getEngine()->convertClickedToVirtualPixelXPosition(getEngine()->getWindowWidth() / 2);
            int iNewCentreY = getEngine()->convertClickedToVirtualPixelYPosition(getEngine()->getWindowHeight() / 2);
            m_filterTranslation.changeOffset(iNewCentreX - iOldCentreX, iNewCentreY - iOldCentreY);
            ++mZoomCount;
        }
        getEngine()->redrawDisplay(); // Force total redraw
        if (targetZoom == (int)ZOOM_DEPTH)
        {
            mZoomInSound.stop();
            mZoomInSound.closeDevice();
            getEngine()->destroyOldObjects(true);
            getEngine()->clearContents();
            static_cast<EzyjddEngineCW4*>(getEngine())->doLevelInstructions(0);
        }
    }
}
