#include "treasure_types_table_view.hpp"


TreasureTypesTableView::TreasureTypesTableView(
    TRect const &bounds,
    TScrollBar *aHScrollBar,
    TScrollBar *aVScrollBar,
    TextRect &&textRect
) :
    TScroller(bounds, aHScrollBar, aVScrollBar),
    table(std::move(textRect))
{
    growMode = gfGrowHiX | gfGrowHiY;
    options |= ofFramed;
    setLimit(table.width(), table.height());
}


void
TreasureTypesTableView::draw()
{
    auto color = getColor(0x0301);
    auto rightPad = (table.width() < size.x) ? string(size.x - table.width(), ' ') : string();
    for (int y = 0; y < size.y; ++y) {
        TDrawBuffer b;
        int j = delta.y + y;
        if (j < table.height()) {
            auto line = table[j].substr(size_t(delta.x), size_t(size.x));
            b.moveStr(0, line, color);
            if (!rightPad.empty()) b.moveStr(table.width(), rightPad.c_str(), color);
        } else {
            b.moveChar(0, ' ', color, size.x);
        }
        writeLine(0, short(y), short(size.x), 1, b);
    }
}
