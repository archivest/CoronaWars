#include "RunningState.h"
#include "EzyjddEngineCW4.h"
#include "GameImageObject.h"
#include "Mine.h"
#include "Missile.h"
#include "Virus.h"

const VirusDirection RunningState::sVirusDirection[4] = { VirusDirection::Random, VirusDirection::FromLeft, VirusDirection::FromTop, VirusDirection::Random };
const double RunningState::sFlowFactorX[4] = { 0.0, -0.25, 0.0, 0.0 };
const double RunningState::sFlowFactorY[4] = { 0.0, 0.0, -1.0, 0.0 };
const int RunningState::sNumViruses[4] = { 3, 7, 10, 15 };
const int RunningState::sNumMines[4] = { 1, 2, 3, 5 };

RunningState::RunningState(BaseEngine* baseEngine) :
    EngineState(baseEngine),
    mLevel(0),
    mSpeedX(0.0),
    mSpeedY(0.0),
    miLastShipTime(0),
    miThrustStartTime(0),
    miLastKeyUpTime(0),
    miLastKeySpaceTime(0),
    mpShip1(nullptr),
    mpShip2(nullptr),
    mMinesLeft(0),
    mScore(0),
    mLives(NUM_LIVES)
{
    mpShip1 = new Ship(this, getEngine(), "images/ship2.png", SHIP_START_X, SHIP_START_Y, true, BLACK);
    mpShip2 = new Ship(this, getEngine(), "images/ship2thrust.png", SHIP_START_X, SHIP_START_Y, true, BLACK, false, true, false);
    mMissileSound.load("resources/missile.wav");
    mPopSound.load("resources/pop.wav");
    mExplosionShipSound.load("resources/ExplosionShip.wav");
    mMineDropSound.load("resources/MineDrop.wav");
}

RunningState::~RunningState()
{
    cleanup(true);
    delete mpShip1;
    delete mpShip2;
}

void RunningState::cleanup(bool destroyActiveObjects)
{
    for (int i = 0; i < mDestroyedMineVector.size(); ++i)
    {
        delete mDestroyedMineVector[i];
    }
    mDestroyedMineVector.clear();

    for (int i = 0; i < mDestroyedMissileVector.size(); ++i)
    {
        delete mDestroyedMissileVector[i];
    }
    mDestroyedMissileVector.clear();

    for (int i = 0; i < mDestroyedVirusVector.size(); ++i)
    {
        delete mDestroyedVirusVector[i];
    }
    mDestroyedVirusVector.clear();

    if (destroyActiveObjects)
    {
        for (int i = 0; i < mMissileVector.size(); ++i)
        {
            for (int j = 0; j < mVirusVector.size(); ++j)
            {
                mVirusVector[j]->removeCollidableObject(mMissileVector[i]);
            }
            getEngine()->removeDisplayableObject(mMissileVector[i]);
            getEngine()->drawableObjectsChanged();
            delete mMissileVector[i];
        }
        mMissileVector.clear();

        for (int i = 0; i < mMineVector.size(); ++i)
        {
            for (int j = 0; j < mVirusVector.size(); ++j)
            {
                mVirusVector[j]->removeCollidableObject(mMineVector[i]);
            }
            getEngine()->removeDisplayableObject(mMineVector[i]);
            getEngine()->drawableObjectsChanged();
            delete mMineVector[i];
        }
        mMineVector.clear();

        for (int i = 0; i < mVirusVector.size(); ++i)
        {
            mVirusVector[i]->removeCollidableObject(mpShip1);

            getEngine()->removeDisplayableObject(mVirusVector[i]);
            getEngine()->drawableObjectsChanged();
            delete mVirusVector[i];
        }
        mVirusVector.clear();

        getEngine()->destroyOldObjects(false);
        getEngine()->clearContents();
    }
}

void RunningState::closeAudioDevices()
{
    mMissileSound.stop();
    mMissileSound.closeDevice();
    mPopSound.stop();
    mPopSound.closeDevice();
    mMineDropSound.stop();
    mMineDropSound.closeDevice();
    mExplosionShipSound.stop();
    mExplosionShipSound.closeDevice();
}

void RunningState::createViruses(int numViruses)
{
    for (int i = 0; i < numViruses; ++i)
    {
        Virus* pVirus = new Virus(sVirusDirection[mLevel], this, getEngine(), "images/cv1.png", 0, 0, true, BLACK);
        mVirusVector.push_back(pVirus);
        getEngine()->appendObjectToArray(pVirus);
        getEngine()->drawableObjectsChanged(); // dont need to do this here because main loop hasn't started yet
        pVirus->addCollidableObject(mpShip1);
    }
}

void RunningState::drawStringsOnTop()
{
    getEngine()->drawForegroundString(150, 25, "Lives", YELLOW);
    getEngine()->drawForegroundString(350, 25, "Mines", YELLOW);
    getEngine()->drawForegroundString(550, 25, "Level", YELLOW);
    getEngine()->drawForegroundString(750, 25, "Score", YELLOW);
    getEngine()->drawForegroundString(150, 75, std::to_string(mLives).c_str(), WHITE);
    getEngine()->drawForegroundString(350, 75, std::to_string(mMinesLeft).c_str(), WHITE);
    getEngine()->drawForegroundString(550, 75, std::to_string(mLevel + 1).c_str(), WHITE);
    getEngine()->drawForegroundString(750, 75, std::to_string(mScore).c_str(), WHITE);
}

int RunningState::initialiseObjects()
{
    if (mLevel == 0)
    {
        mLives = NUM_LIVES;
    }
    createViruses(sNumViruses[mLevel]);
    mMinesLeft = sNumMines[mLevel];

    getEngine()->appendObjectToArray(mpShip1);
    getEngine()->appendObjectToArray(mpShip2);

    getEngine()->notifyObjectsAboutKeys(false); // tell the main engine when keys are pressed rather than individual objects
    getEngine()->notifyObjectsAboutMouse(true);
    return 0;
}

void RunningState::keyDown(int iKeyCode)
{
    switch (iKeyCode)
    {
    case SDLK_q: // quit - end the game
        cleanup(true);
        closeAudioDevices();
        static_cast<EzyjddEngineCW4*>(getEngine())->doLoseGame();
        break;

    case SDLK_n: // next level
        cleanup(true);
        closeAudioDevices();
        static_cast<EzyjddEngineCW4*>(getEngine())->setScore(mScore);
        static_cast<EzyjddEngineCW4*>(getEngine())->doZoomIn(mLevel);
        break;

    case SDLK_p: // pause the game
        static_cast<EzyjddEngineCW4*>(getEngine())->doPause();
        break;

    case SDLK_s: // save the game
        saveGame();
        break;

    case SDLK_l: // load the game
        loadGame(true);
        break;

    case SDLK_m: // mine
        if (mMinesLeft > 0)
        {
            mMineDropSound.play();
            Mine* pMine = new Mine(this, getEngine(), "images/mine2.png", mpShip1->getX() + 25, mpShip1->getY() + 30, true, BLACK, true);
            mMineVector.push_back(pMine);

            // make sure ship is displayed on top of mine
            getEngine()->removeDisplayableObject(mpShip1);
            getEngine()->removeDisplayableObject(mpShip2);
            getEngine()->appendObjectToArray(pMine);
            getEngine()->appendObjectToArray(mpShip1);
            getEngine()->appendObjectToArray(mpShip2);

            getEngine()->drawableObjectsChanged();
            mMinesLeft--;
            for (int i = 0; i < mVirusVector.size(); i++)
            {
                mVirusVector[i]->addCollidableObject(pMine);
            }
        }
        break;

    default: // do nothing
        break;
    }
}

bool RunningState::loadGame(bool fromRunningState)
{
    std::fstream file;
    file.open("SavedGame.txt", std::ios::in);
    if (!file.is_open())
    {
        return false;
    }
    if (fromRunningState)
    {
        cleanup(true);
    }
    else
    {
        getEngine()->destroyOldObjects(true);
        getEngine()->clearContents();
        openAudioDevices();
    }
    double rShip;
    bool v1;
    int shipX, shipY;
    int virusesLeft, missilesLeft, minesLeft;
    file >> mLevel >> mLives >> mScore >> shipX >> shipY >> rShip >> mSpeedX >> mSpeedY >> virusesLeft >> missilesLeft>> minesLeft;
    static_cast<EzyjddEngineCW4*>(getEngine())->setScore(mScore);

    getEngine()->lockBackgroundForDrawing();
    setupBackgroundBuffer();
    getEngine()->unlockBackgroundForDrawing();

    mpShip1->setPositionDbl(shipX, shipY);
    mpShip2->setPositionDbl(shipX, shipY);
    mpShip1->setVisible(true);
    mpShip2->setVisible(false);
    mpShip1->getRotator().setRotation(rShip);
    mpShip2->getRotator().setRotation(rShip);
    miLastShipTime = getEngine()->getRawTime();
    miThrustStartTime = 0;

    for (int i = 0; i < virusesLeft; ++i)
    {
        int virusX, virusY, virusToX, virusToY, virusMovementTimeLeft;
        double rVirus, virusRotationSpeed;
        file >> virusX >> virusY >> virusToX >> virusToY >> virusMovementTimeLeft >> rVirus >> virusRotationSpeed;
        Virus* pVirus = new Virus(sVirusDirection[mLevel], this, getEngine(), "images/cv1.png", virusX, virusY, virusToX, virusToY, virusRotationSpeed, virusMovementTimeLeft, true);
        mVirusVector.push_back(pVirus);
        pVirus->addCollidableObject(mpShip1);
        getEngine()->appendObjectToArray(pVirus);
    }
    for (int i = 0; i < missilesLeft; ++i)
    {
        int missileX, missileY;
        double missileAngle;
        file >> missileX >> missileY >> missileAngle;
        Missile* pMissile = new Missile(missileAngle, this, getEngine(), missileX, missileY, mpShip1->getImageWidth(), mpShip1->getImageHeight());
        for (int j = 0; j < mVirusVector.size(); ++j)
        {
            mVirusVector[j]->addCollidableObject(pMissile);
        }
        mMissileVector.push_back(pMissile);
        getEngine()->appendObjectToArray(pMissile);
    }
    for (int i = 0; i < minesLeft; ++i)
    {
        int mineX, mineY;
        file >> mineX >> mineY;
        Mine* pMine = new Mine(this, getEngine(), "images/mine2.png", mineX, mineY, true);
        for (int j = 0; j < mVirusVector.size(); ++j)
        {
            mVirusVector[j]->addCollidableObject(pMine);
        }
        mMineVector.push_back(pMine);
        getEngine()->appendObjectToArray(pMine);
    }
    getEngine()->appendObjectToArray(mpShip1);
    getEngine()->appendObjectToArray(mpShip2);
    getEngine()->redrawDisplay();
    getEngine()->notifyObjectsAboutKeys(false); // tell the main engine when keys are pressed rather than individual objects
    getEngine()->notifyObjectsAboutMouse(true);
    getEngine()->drawableObjectsChanged();

    file.close();
    return true;
}

void RunningState::mainLoopPostUpdate()
{
    if (mLives == 0)
    {
        // lose game
        cleanup(true);
        closeAudioDevices();
        static_cast<EzyjddEngineCW4*>(getEngine())->doLoseGame();
        return;
    }
    if (mVirusVector.size() == 0)
    {
        // end of level
        cleanup(true);
        closeAudioDevices();
        static_cast<EzyjddEngineCW4*>(getEngine())->setScore(mScore);
        static_cast<EzyjddEngineCW4*>(getEngine())->doZoomIn(mLevel);
    }
}

void RunningState::onMineCollision(CollidableInterface* pMineCI)
{
    Mine* pMine = static_cast<Mine*>(pMineCI);

    // remove this mine from all viruses
    for (int i = 0; i < mVirusVector.size(); ++i)
    {
        mVirusVector[i]->removeCollidableObject(pMineCI);
    }

    // remove it from the mine vector
    for (auto it = mMineVector.begin(); it != mMineVector.end(); ++it)
    {
        Mine* pMine_ = *it;
        if (pMine_ == pMine)
        {
            // can't delete the virus at this point as this callback was called by it
            // so add it to my destroyed virus vector so I can delete it at the end of the main loop
            mMineVector.erase(it);
            mDestroyedMineVector.push_back(pMine_);
            break;
        }
    }
    // remove it from the array
    getEngine()->removeDisplayableObject(pMine);
    getEngine()->drawableObjectsChanged();

    // update game state
    mScore += 20;
}

void RunningState::onMineDrop(CollidableInterface* pMine)
{
    mMineDropSound.play();
}

void RunningState::onMissileCollision(CollidableInterface* pMissileCI)
{
    Missile* pMissile = static_cast<Missile*>(pMissileCI);

    // remove missile from all viruses
    for (int i = 0; i < mVirusVector.size(); ++i)
    {
        mVirusVector[i]->removeCollidableObject(pMissileCI);
    }

    // remove it from the missile vector
    for (auto it = mMissileVector.begin(); it != mMissileVector.end(); ++it)
    {
        Missile* pMissile_ = *it;
        if (pMissile_ == pMissile)
        {
            // can't delete the virus at this point as this callback was called by it
            // so add it to my destroyed virus vector so I can delete it at the end of the main loop
            mMissileVector.erase(it);
            mDestroyedMissileVector.push_back(pMissile_);
            break;
        }
    }
    // remove it from the array
    getEngine()->removeDisplayableObject(pMissile);
    getEngine()->drawableObjectsChanged();

    mScore += 10;
}

void RunningState::onMissileMiss(CollidableInterface* pMissileCI)
{
    Missile* pMissile = static_cast<Missile*>(pMissileCI);

    // remove missile from all viruses
    for (int i = 0; i < mVirusVector.size(); ++i)
    {
        mVirusVector[i]->removeCollidableObject(pMissileCI);
    }

    // remove it from the missile vector
    for (auto it = mMissileVector.begin(); it != mMissileVector.end(); ++it)
    {
        Missile* pMissile_ = *it;
        if (pMissile_ == pMissile)
        {
            // can't delete the virus at this point as this callback was called by it
            // so add it to my destroyed virus vector so I can delete it at the end of the main loop
            mMissileVector.erase(it);
            mDestroyedMissileVector.push_back(pMissile_);
            break;
        }
    }

    // remove it from the array
    getEngine()->removeDisplayableObject(pMissile);
    getEngine()->drawableObjectsChanged();
}

void RunningState::onShipCollision(CollidableInterface* pShip)
{
    if (mLives > 0)
    {
        mExplosionShipSound.stop();
        mExplosionShipSound.play();
        mLives--;
        resetShipToStartPosition();
    }
}

void RunningState::onVirusCollision(GameImageObject* pVirusGameObject)
{
    mPopSound.stop();
    mPopSound.play();
    Virus* pVirus = static_cast<Virus*>(pVirusGameObject);
    for (auto it = mVirusVector.begin(); it != mVirusVector.end(); ++it)
    {
        Virus* pVirus_ = *it;
        if (pVirus_ == pVirus)
        {
            // can't delete the virus at this point as this callback was called by it
            // so add it to my destroyed virus vector so I can delete it at the end of the main loop
            mVirusVector.erase(it);
            pVirus_->clearCollidableObjects();
            mDestroyedVirusVector.push_back(pVirus_);
            break;
        }
    }
    getEngine()->removeDisplayableObject(pVirus);
    getEngine()->drawableObjectsChanged();
}

void RunningState::openAudioDevices()
{
    mMissileSound.openDevice();
    mPopSound.openDevice();
    mExplosionShipSound.openDevice();
    mMineDropSound.openDevice();
}

void RunningState::prepareState(int level)
{
    openAudioDevices();
    resetShipToStartPosition();
    mLevel = level;
    mScore = static_cast<EzyjddEngineCW4*>(getEngine())->getScore();
    mSpeedX = 0.0;
    mSpeedY = 0.0;
    miLastShipTime = getEngine()->getRawTime();
    miThrustStartTime = 0;
    miLastKeyUpTime = 0;
    miLastKeySpaceTime = 0;
    mMinesLeft = 0;
}

void RunningState::resetShipToStartPosition()
{
    mpShip1->setPositionDbl(SHIP_START_X, SHIP_START_Y);
    mpShip2->setPositionDbl(SHIP_START_X, SHIP_START_Y);
    mpShip1->getRotator().setRotation(0.0);
    mpShip2->getRotator().setRotation(0.0);
    mpShip1->setVisible(true);
    mpShip2->setVisible(false);
    mSpeedX = 0.0;
    mSpeedY = 0.0;
    miThrustStartTime = 0;
    miLastShipTime = 0;
    miLastKeyUpTime = 0;
    miLastKeySpaceTime = 0;
}

void RunningState::saveGame()
{
    std::fstream file;
    file.open("SavedGame.txt", std::ios::out | std::ios::trunc);
    file << mLevel << " " << mLives << " " << mScore << " " << mpShip1->getX() << " " << mpShip1->getY() << " "
         << mpShip1->getRotator().getRotation() << " " << mSpeedX << " " << mSpeedY << " "
         << mVirusVector.size() << " " << mMissileVector.size() << " " << mMineVector.size() << " ";
    for (int i = 0; i < mVirusVector.size(); ++i)
    {
        Virus* pVirus = mVirusVector[i];
        file << pVirus->getMovementPositionCalc().getX() << " " << pVirus->getMovementPositionCalc().getY() << " "
             << pVirus->getToX() << " " << pVirus->getToY() << " " << pVirus->getMovementTimeLeft() << " "
             << pVirus->getRotator().getRotation() << " " << pVirus->getRotationSpeed() << " ";
    }
    for (int i = 0; i < mMissileVector.size(); ++i)
    {
        Missile* pMissile = mMissileVector[i];
        file << pMissile->getX() << " " << pMissile->getY() << " " << pMissile->getAngle() << " ";
    }
    for (int i = 0; i < mMineVector.size(); ++i)
    {
        Mine* pMine = mMineVector[i];
        file << pMine->getX() << " " << pMine->getY() << " ";
    }
    file << std::endl;
    file.close();
}   

void RunningState::setupBackgroundBuffer()
{
    // load correct background for the level
    std::string url = "images/level" + std::to_string(mLevel + 1) + "background.png";
    mBackgroundImage = ImageManager::loadImage(url.c_str(), true);

    mBackgroundImage.renderImage(getEngine()->getBackgroundSurface(), 0, 0, 0, 0,
        mBackgroundImage.getWidth(), mBackgroundImage.getHeight());
}

void RunningState::updateAllObjects(int iCurrentTime)
{
    // move the ship
    if (miLastShipTime != 0)
    {
        double diff = (double)iCurrentTime - (double)miLastShipTime;
        double distanceX = (diff / 1000.0 * mSpeedX);
        double distanceY = (diff / 1000.0 * mSpeedY);

        // adjust the speed by the flow factor. This will make the ship accelerate according to the x and y flow factors
        mSpeedX += sFlowFactorX[mLevel];
        mSpeedY += sFlowFactorY[mLevel];

        // calculate the new ship position based on the distance moved in each axis
        double shipX = mpShip1->getXDbl() - distanceX;
        double shipY = mpShip1->getYDbl() - distanceY;

        // make ship reappear if it goes off screen by more than its own width/height
        // if it's gone off the left of the windows
        if (shipX <= -mpShip1->getImageWidth())
        {
            // place it off-screen to the right
            shipX = (double)getEngine()->getWindowWidth();
        }
        else if (shipY <= -mpShip1->getImageHeight()) // if it's gone off the top
        {
           // place it off-screen at the bottom
            shipY = (double)getEngine()->getWindowHeight();
        }
        else if (shipX >= (double)getEngine()->getWindowWidth()) // if it's gone off the right
        {
            // place it off-screen to the left
            shipX = -mpShip1->getImageWidth();
        }
        else if (shipY >= (double)getEngine()->getWindowHeight()) // if it's gone off the bottom
        {
            // place it off-screen at the top
            shipY = -mpShip1->getImageHeight();
        }
        mpShip1->setPositionDbl(shipX, shipY);
        mpShip2->setPositionDbl(shipX, shipY);
    }
    miLastShipTime = iCurrentTime;

    // turn off the flame if more than 250ms have passed
    if (miThrustStartTime > 0 && iCurrentTime - miThrustStartTime > 250)
    {
        miThrustStartTime = 0;
        mpShip1->setVisible(true);
        mpShip2->setVisible(false);
    }
    if (getEngine()->isKeyPressed(SDLK_LEFT))
    {
        double r = mpShip1->getRotator().getRotation() + 0.04; // rotate anti-clockwise by 0.04 radians each time the key is pressed
        if (r >= (2.0 * M_PI))
        {
            r = 0.0;
        }
        mpShip1->getRotator().setRotation(r);
        mpShip2->getRotator().setRotation(r);
        getEngine()->redrawDisplay();
    }
    else if (getEngine()->isKeyPressed(SDLK_RIGHT))
    {
        double r = mpShip1->getRotator().getRotation();
        if (r == 0.0)
        {
            r = (2.0 * M_PI);
        }
        r -= 0.04; // rotate clockwise by 0.04 radians at a time
        mpShip1->getRotator().setRotation(r);
        mpShip2->getRotator().setRotation(r);
        getEngine()->redrawDisplay();
    }
    if (getEngine()->isKeyPressed(SDLK_UP) && iCurrentTime > miLastKeyUpTime + 50)
    {
        double r = mpShip1->getRotator().getRotation();
        mpShip2->getRotator().setRotation(r);
        mSpeedX += sin(r) * 16.0;
        mSpeedY += cos(r) * 16.0;
        mpShip1->setVisible(false);
        mpShip2->setVisible(true);
        miThrustStartTime = iCurrentTime;
        getEngine()->redrawDisplay();
        miLastKeyUpTime = iCurrentTime;
    }
    if (getEngine()->isKeyPressed(SDLK_SPACE) && iCurrentTime > miLastKeySpaceTime + 250)
    {
        mMissileSound.stop();
        mMissileSound.play();
        double r = mpShip1->getRotator().getRotation();
        Missile* pMissile = new Missile(r, this, getEngine(), mpShip1->getX(), mpShip1->getY(), mpShip1->getImageWidth(), mpShip1->getImageHeight());
        mMissileVector.push_back(pMissile);
        getEngine()->appendObjectToArray(pMissile);
        getEngine()->drawableObjectsChanged();
        for (int i = 0; i < mVirusVector.size(); ++i)
        {
            mVirusVector[i]->addCollidableObject(pMissile);
        }
        miLastKeySpaceTime = iCurrentTime;
    }
    getEngine()->redrawDisplay();
}
