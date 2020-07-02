#include "EzyjddTileManager.h"

EzyjddTileManager::EzyjddTileManager(BaseEngine* pEng, int iTileHeight, int iTileWidth)
	: TileManager(iTileHeight, iTileWidth),
	m_pEng(pEng)
{
}

void EzyjddTileManager::doSomething(int x, int y)
{
	int iTileX = getMapXForScreenX(x);
	int iTileY = getMapYForScreenY(y);
	int iCurrentTile = getMapValue(iTileX, iTileY);
	setAndRedrawMapValueAt(iTileX, iTileY, iCurrentTile + 1, m_pEng, m_pEng->getBackgroundSurface());
}
