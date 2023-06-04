#include "treasure_window.hpp"

#include "window_numbers.hpp"


static string
makeTitle(char letter)
{
    return string("Treasure ") + string(1, letter);
}


TreasureWindow::TreasureWindow(TRect const &bounds, char letter) :
    TWindowInit(&TreasureWindow::initFrame),
    TWindow(bounds, makeTitle(letter), windowNumbers.takeNext())
{
    auto r = getExtent();
    r.grow(-1, -1);
}


TreasureWindow::~TreasureWindow()
{
    windowNumbers.release(number);
}
