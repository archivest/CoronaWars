#include "HighScoresState.h"
#include "EzyjddEngineCW4.h"
#include "Sound.h"

using namespace std;

HighScoresState::HighScoresState(BaseEngine* baseEngine) :
    EngineState(baseEngine),
    mScore(0),
    mEnterYPos(0)
{
    mHighScoreSound.load("resources/HighScore.wav");
}

HighScoresState::~HighScoresState()
{
}

void HighScoresState::displayInitials(bool lockBackground)
{
    std::string initialsForDisplay(mInitials);
    if (lockBackground)
    {
        getEngine()->lockBackgroundForDrawing();
    }
    getEngine()->drawBackgroundRectangle(350, mEnterYPos, 500, mEnterYPos + 40, BLACK);
    if (mInitials.size() < 3)
    {
        initialsForDisplay += '_';
    }
    getEngine()->drawBackgroundString(350, mEnterYPos, initialsForDisplay.c_str(), YELLOW);
    if (lockBackground)
    {
        getEngine()->unlockBackgroundForDrawing();
    }
}

void HighScoresState::keyDown(int iKeyCode)
{
    switch (iKeyCode)
    {
    case SDLK_BACKSPACE:
    case SDLK_DELETE:
        if (mEnterYPos > 0)
        {
            if (mInitials.empty())
            {
                return;
            }
            mInitials.pop_back();
            displayInitials(true);
            getEngine()->redrawDisplay();
        }
        break;

    case SDLK_RETURN:
        if (mEnterYPos > 0)
        {
            if (mInitials.empty())
            {
                return;
            }
            static_cast<EzyjddEngineCW4*>(getEngine())->setHighScore(mInitials);
        }
        getEngine()->destroyOldObjects(true);
        getEngine()->clearContents();
        mHighScoreSound.stop();
        mHighScoreSound.closeDevice();
        static_cast<EzyjddEngineCW4*>(getEngine())->doStartup();
        break;

    default:
        if (mEnterYPos > 0 && (iKeyCode >= 'a' && iKeyCode <= 'z') && mInitials.size() < 3)
        {
            char key = iKeyCode - 0x20; // convert to upper case
            mInitials += key;
            displayInitials(true);
            getEngine()->redrawDisplay();
        }
        break;
    }
}

void HighScoresState::prepareState()
{
    mHighScoreSound.openDevice();
    mInitials.clear();
    mScore = static_cast<EzyjddEngineCW4*>(getEngine())->getScore();
    mEnterYPos = 0;
}

void HighScoresState::setupBackgroundBuffer()
{
    getEngine()->fillBackground(BLACK);
    mBackgroundImage = ImageManager::loadImage("images/CVBanner.png", true);
    mBackgroundImage.renderImage(getEngine()->getBackgroundSurface(), 0, 0, 0, 0,
        mBackgroundImage.getWidth(), mBackgroundImage.getHeight());

    const std::vector<HighScore>& highScores = static_cast<EzyjddEngineCW4*>(getEngine())->getHighScores();
    getEngine()->drawBackgroundString(350, 200, "INITIALS", WHITE);
    getEngine()->drawBackgroundString(550, 200, "SCORE", WHITE);
    int yPos = 250;
    bool hsDone = false;

    // go through high scores table
    for (int i = 0, j = 0; j < NUM_HIGH_SCORES; ++j)
    {
        // draw score ranking number
        getEngine()->drawBackgroundString(300, yPos, std::to_string(j + 1).c_str(), WHITE);

        // if I've not checked before (hsDone), check if the score is higher than this one
        if (!hsDone && // have already set the initials
            mScore > 0 &&  // there is a score
            (highScores.empty() // either there's no high score to check against
                || (i < highScores.size() && mScore > highScores[i].getScore()) // OR there is a high score and this score is > than that score
                || // OR
                (i == highScores.size() && j < NUM_HIGH_SCORES))) // there are no more high scores and there are empty places left
        {
            mHighScoreSound.play();
            // yes - set up for entering initials
            mEnterYPos = yPos;
            displayInitials(false);
            getEngine()->drawBackgroundString(550, yPos, std::to_string(mScore).c_str(), YELLOW);
            hsDone = true;
            yPos += 50;
            getEngine()->drawBackgroundString(150, 650, "Enter initials then press ENTER", WHITE);
            continue;
        }

        // display high score
        if (i < highScores.size())
        {
            getEngine()->drawBackgroundString(350, yPos, highScores[i].getInitials().c_str(), WHITE);
            std::string scoreStr = std::to_string(highScores[i].getScore());
            getEngine()->drawBackgroundString(550, yPos, scoreStr.c_str(), WHITE);
            i++;
        }
        yPos += 50;
    }
    if (!hsDone)
    {
        getEngine()->drawBackgroundString(150, 650, "Press ENTER to return to Start", WHITE);
    }
}
