#include "treasure_types_table_window.hpp"

#include "treasure_types_table_view.hpp"
#include "window_numbers.hpp"


TreasureTypesTableWindow::TreasureTypesTableWindow(const TRect &bounds) :
    TWindowInit(&TreasureTypesTableWindow::initFrame),
    TWindow(bounds, "Treasure Types Table", windowNumbers.takeNext())
{
    auto r = getClipRect();
    r.grow(-1, -1);
    auto hScrollBar = standardScrollBar(sbHorizontal | sbHandleKeyboard);
    auto vScrollBar = standardScrollBar(sbVertical | sbHandleKeyboard);
    insert(new TreasureTypesTableView(r, hScrollBar, vScrollBar));
}


TreasureTypesTableWindow::~TreasureTypesTableWindow()
{
    windowNumbers.release(number);
}
