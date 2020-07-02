#include "header.h"
#include "EzyjddEngine.h"
#include "ImageManager.h"
#include "MyTriangle.h"
#include "MySquare.h"
#include "MyChangingText.h"

EzyjddEngine::EzyjddEngine() :
	m_tm(this, 30, 30)
{
	m_tm.setMapSize(25, 10);
	m_tm.setTopLeftPositionOnScreen(100, 100);
	std::srand(getRawTime());
}

void EzyjddEngine::virtSetupBackgroundBuffer()
{
	SimpleImage image = ImageManager::loadImage("images/CW3ApocBkgrd2.jpg", true);

	image.renderImage(getBackgroundSurface(), 0, 0, 0, 0,
		image.getWidth(), image.getHeight());
}

int EzyjddEngine::virtInitialiseObjects()
{
	// Record the fact that we are about to change the array - so it doesn't get used elsewhere without reloading it
	drawableObjectsChanged();

	// Destroy any existing objects
	destroyOldObjects(true);

	createObjectArray(5);
	MySquare* pSquare1 = new MySquare(this, 0x4C47B3, MySquare::Behaviour::collision);
	MySquare* pSquare2 = new MySquare(this, 0x96B386, MySquare::Behaviour::collision);
	pSquare1->setOtherSquare(pSquare2);
	pSquare2->setOtherSquare(pSquare1);
	storeObjectInArray(0, new MyTriangle(this, m_tm));
	storeObjectInArray(1, new MySquare(this, 0x00B000, MySquare::Behaviour::no_collision));
	storeObjectInArray(2, new MyChangingText(this));
	storeObjectInArray(3, pSquare1);
	storeObjectInArray(4, pSquare2);
	drawBackgroundString(550, 0, "Ezyjdd Coursework 3", 0xB0B000);
	notifyObjectsAboutMouse(true);
	m_tm.drawAllTiles(this, getBackgroundSurface());
	return 0;
}
