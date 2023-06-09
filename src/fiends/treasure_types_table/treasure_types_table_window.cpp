#include "treasure_types_table_window.hpp"

#include "fiends/util/window_numbers.hpp"

#include "fiends/views/text_rect_view.hpp"


TreasureTypesTableWindow::TreasureTypesTableWindow(
        TRect const &bounds, TextRect &&textRect
) :
    TWindowInit(&TreasureTypesTableWindow::initFrame),
    TWindow(bounds, "Treasure Types Table", windowNumbers.takeNext())
{
    auto r = getExtent();
    r.grow(-1, -1);
    auto hScrollBar = standardScrollBar(sbHorizontal | sbHandleKeyboard);
    auto vScrollBar = standardScrollBar(sbVertical | sbHandleKeyboard);
    insert(new TextRectView(r, hScrollBar, vScrollBar, std::move(textRect), 0x0301));
}


TreasureTypesTableWindow::~TreasureTypesTableWindow()
{
    windowNumbers.release(number);
}
