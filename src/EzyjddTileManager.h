#pragma once
#include "header.h"
#include "TileManager.h"
#include "BaseEngine.h"

class EzyjddTileManager final :
	public TileManager
{
public:
	EzyjddTileManager(BaseEngine* pEng, int iTileHeight, int iTileWidth);

public:
	void doSomething(int x, int y);

private:
	BaseEngine* m_pEng;
};
