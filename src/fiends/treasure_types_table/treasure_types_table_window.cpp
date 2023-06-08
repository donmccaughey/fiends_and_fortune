#include "treasure_types_table_window.hpp"

#include "treasure_types_table_view.hpp"

#include "fiends/window_numbers.hpp"


TreasureTypesTableWindow::TreasureTypesTableWindow(TRect const &bounds) :
    TWindowInit(&TreasureTypesTableWindow::initFrame),
    TWindow(bounds, "Treasure Types Table", windowNumbers.takeNext())
{
    auto r = getExtent();
    r.grow(-1, -1);
    auto hScrollBar = standardScrollBar(sbHorizontal | sbHandleKeyboard);
    auto vScrollBar = standardScrollBar(sbVertical | sbHandleKeyboard);
    insert(new TreasureTypesTableView(r, hScrollBar, vScrollBar));
}


TreasureTypesTableWindow::~TreasureTypesTableWindow()
{
    windowNumbers.release(number);
}
