#pragma once
#include "header.h"
#include "BaseEngine.h"

class EngineState
{
public:
    EngineState(BaseEngine* baseEngine);
    virtual ~EngineState();

    // these are for each state's implementation of the engine virtual functions that I implement
    virtual void prepareState() {}
    virtual int initialiseObjects() { return 0; }
    virtual void setupBackgroundBuffer() {}
    virtual void keyDown(int iKeyCode) {}
    virtual void updateAllObjects(int iCurrentTime) {}
    virtual void drawStringsOnTop() {}
    virtual void mainLoopPostUpdate() {}

    BaseEngine* getEngine() { return mBaseEngine; }

private:
    BaseEngine* mBaseEngine;
};
