#include "treasure_table_window.hpp"

#include "treasure_table_view.hpp"
#include "window_numbers.hpp"


TreasureTableWindow::TreasureTableWindow(const TRect &bounds) :
    TWindowInit(&TreasureTableWindow::initFrame),
    TWindow(bounds, "Treasure Table", windowNumbers.takeNext())
{
    auto r = getClipRect();
    r.grow(-1, -1);
    auto hScrollBar = standardScrollBar(sbHorizontal | sbHandleKeyboard);
    auto vScrollBar = standardScrollBar(sbVertical | sbHandleKeyboard);
    insert(new TreasureTableView(r, hScrollBar, vScrollBar));
}


TreasureTableWindow::~TreasureTableWindow()
{
    windowNumbers.release(number);
}
