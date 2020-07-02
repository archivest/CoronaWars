#pragma once
#include "header.h"
#include "EngineState.h"
#include "GameImageObject.h"
#include "Ship.h"
#include "Mine.h"
#include "Missile.h"
#include "Virus.h"
#include "RunningState.h"
#include "CollisionCallbacks.h"
#include "VirusCallbacks.h"
#include "Sound.h"

#define SHIP_START_X 470
#define SHIP_START_Y 340

class RunningState final : public EngineState, public CollisionCallbacks, public VirusCallbacks
{
public:
    RunningState(BaseEngine* baseEngine);
    ~RunningState();

// EngineState overrides
public:
    int initialiseObjects() override;
    void setupBackgroundBuffer() override;
    void keyDown(int iKeyCode) override;
    void updateAllObjects(int iCurrentTime) override;
    void drawStringsOnTop() override;
    void mainLoopPostUpdate() override;

public:
    void prepareState(int level);
    bool loadGame(bool fromRunningState);

private:
    void createViruses(int numViruses);
    void saveGame();
    void resetShipToStartPosition();
    void cleanup(bool destroyActiveObjects);
    void openAudioDevices();
    void closeAudioDevices();

// VirusCallbacks
private:
    void onVirusCollision(GameImageObject* pVirus) override;

private:
// CollisionCallbacks
    void onMineCollision(CollidableInterface* pMine) override;
    void onMissileCollision(CollidableInterface* pMissile) override;
    void onShipCollision(CollidableInterface* pShip) override;
    void onMissileMiss(CollidableInterface* pMissile) override;
    void onMineDrop(CollidableInterface* pMine) override;

private:
    int mLevel;
    SimpleImage mBackgroundImage;
    double mSpeedX;
    double mSpeedY;
    int miLastShipTime;
    int miThrustStartTime;
    int miLastKeyUpTime;
    int miLastKeySpaceTime;
    Ship* mpShip1;
    Ship* mpShip2;
    int mMinesLeft;
    int mScore;
    int mLives;
    std::vector<Virus*> mVirusVector;
    std::vector<Missile*> mMissileVector;
    std::vector<Mine*> mMineVector;
    std::vector<Virus*> mDestroyedVirusVector;
    std::vector<Mine*> mDestroyedMineVector;
    std::vector<Missile*> mDestroyedMissileVector;
    static const VirusDirection sVirusDirection[];
    static const double sFlowFactorX[];
    static const double sFlowFactorY[];
    static const int sNumViruses[];
    static const int sNumMines[];
    Sound mMissileSound;
    Sound mPopSound;
    Sound mExplosionShipSound;
    Sound mMineDropSound;
};
