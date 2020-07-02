#include "ZoomInState.h"
#include "EzyjddEngineCW4.h"
#include "GameImageObject.h"

using namespace std;

ZoomInState::ZoomInState(BaseEngine* baseEngine) :
    EngineState(baseEngine),
    mLevel(0),
    m_filterScaling(0, 0, baseEngine),
    m_filterTranslation(0, 0, &m_filterScaling),
    mStartTime(0),
    mZoomCount(0)
{
    mZoomInSound.load("resources/ZoomIn.wav");
}

ZoomInState::~ZoomInState()
{

}

void ZoomInState::prepareState(int level)
{
    mLevel = level;
    mStartTime = 0;
    mZoomCount = 0;
}

int ZoomInState::initialiseObjects()
{
    mZoomInSound.openDevice();
    mZoomInSound.play();
    // initialise filters used for zooming
    getEngine()->getBackgroundSurface()->setDrawPointsFilter(&m_filterTranslation);
    getEngine()->getForegroundSurface()->setDrawPointsFilter(&m_filterTranslation);

    // display the background image in the foreground for zooming
    getEngine()->createObjectArray(1);
    std::string url = "images/level" + std::to_string(mLevel + 1) + "background.png";
    getEngine()->storeObjectInArray(0, new GameImageObject(getEngine(), url));
    return 0;
}

void ZoomInState::updateAllObjects(int iCurrentTime)
{
    // zoom into foreground using iCurrentTime to control how quickly the zoom occurs
    // first time in, mStartTime will be 0, so initialise it to the current time
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
        if (mZoomCount == (int)ZOOM_DEPTH)
        {
            // zoom out so we're zoomed out for next time this level is used
            while (mZoomCount-- > 0)
            {
                int iOldCentreX = getEngine()->convertClickedToVirtualPixelXPosition(getEngine()->getWindowWidth() / 2);
                int iOldCentreY = getEngine()->convertClickedToVirtualPixelYPosition(getEngine()->getWindowHeight() / 2);
                m_filterScaling.compress();
                int iNewCentreX = getEngine()->convertClickedToVirtualPixelXPosition(getEngine()->getWindowWidth() / 2);
                int iNewCentreY = getEngine()->convertClickedToVirtualPixelYPosition(getEngine()->getWindowHeight() / 2);
                m_filterTranslation.changeOffset(iNewCentreX - iOldCentreX, iNewCentreY - iOldCentreY);
            }
            getEngine()->destroyOldObjects(true);
            getEngine()->clearContents();
            if (mLevel < 3)
            {
                mZoomInSound.stop();
                mZoomInSound.closeDevice();
                static_cast<EzyjddEngineCW4*>(getEngine())->doZoomOut(mLevel + 1);
            }
            else
            {
                mZoomInSound.stop();
                mZoomInSound.closeDevice();
                static_cast<EzyjddEngineCW4*>(getEngine())->doWinGame();
            }
        }
    }
}
