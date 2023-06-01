#include "treasure_table_window.hpp"

#include "window_numbers.hpp"


TreasureTableWindow::TreasureTableWindow(const TRect &bounds) :
    TWindowInit(&TreasureTableWindow::initFrame),
    TWindow(bounds, "Treasure Table", windowNumbers.takeNext())
{
}


TreasureTableWindow::~TreasureTableWindow()
{
    windowNumbers.release(number);
}
