#include <gui/screen_screen/screenView.hpp>
#include <touchgfx/containers/scrollers/ScrollWheel.hpp>

screenView::screenView()
{

}

void screenView::setupScreen()
{
    screenViewBase::setupScreen();
}

void screenView::tearDownScreen()
{
    screenViewBase::tearDownScreen();
}

void screenView::handleTickEvent()
{
    tickCounter++;

    if (tickCounter == 100)
    {
    	tickCounter = 0;

    	countItem > 9 ? countItem = 0 : countItem++;

    	scrollWheel1.animateToItem(countItem, 30);
    }
}
