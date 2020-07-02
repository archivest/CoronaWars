#pragma once
#include "BaseEngine.h"
#include "EzyjddTileManager.h"

class EzyjddEngine :
	public BaseEngine
{
public:
	EzyjddEngine();

private:
    void virtSetupBackgroundBuffer() override;
	int virtInitialiseObjects() override;

private:
	EzyjddTileManager m_tm;
};

