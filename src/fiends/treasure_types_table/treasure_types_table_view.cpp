#include "treasure_types_table_view.hpp"


TreasureTypesTableView::TreasureTypesTableView(
    TRect const &bounds,
    TScrollBar *aHScrollBar,
    TScrollBar *aVScrollBar,
    TextRect &&textRect
) :
        TScroller(bounds, aHScrollBar, aVScrollBar),
        textRect(std::move(textRect))
{
    growMode = gfGrowHiX | gfGrowHiY;
    options |= ofFramed;
    setLimit(this->textRect.width(), this->textRect.height());
}


void
TreasureTypesTableView::draw()
{
    auto color = getColor(0x0301);
    auto rightPad = (textRect.width() < size.x) ? string(size.x - textRect.width(), ' ') : string();
    for (int y = 0; y < size.y; ++y) {
        TDrawBuffer b;
        int j = delta.y + y;
        if (j < textRect.height()) {
            auto line = textRect[j].substr(size_t(delta.x), size_t(size.x));
            b.moveStr(0, line, color);
            if (!rightPad.empty()) b.moveStr(textRect.width(), rightPad.c_str(), color);
        } else {
            b.moveChar(0, ' ', color, size.x);
        }
        writeLine(0, short(y), short(size.x), 1, b);
    }
}
