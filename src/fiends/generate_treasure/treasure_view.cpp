#include "treasure_view.hpp"



TreasureView::TreasureView(
    TRect const &bounds,
    TScrollBar *aHScrollBar,
    TScrollBar *aVScrollBar,
    unique_ptr<treasure, void(*)(treasure *)> &&aTreasure,
    TextRect &&textRect
) :
        TScroller(bounds, aHScrollBar, aVScrollBar),
        aTreasure(std::move(aTreasure)),
        text(std::move(textRect))
{
    growMode = gfGrowHiX | gfGrowHiY;
    options |= ofFramed;
    setLimit(text.width(), text.height());
}


void
TreasureView::draw()
{
    auto color = getColor(0x0301);
    auto rightPad = (text.width() < size.x) ? string(size.x - text.width(), ' ') : string();
    for (int y = 0; y < size.y; ++y) {
        TDrawBuffer b;
        int j = delta.y + y;
        if (j < text.height()) {
            auto line = text[j].substr(size_t(delta.x), size_t(size.x));
            b.moveStr(0, line, color);
            if (!rightPad.empty()) b.moveStr(text.width(), rightPad.c_str(), color);
        } else {
            b.moveChar(0, ' ', color, size.x);
        }
        writeLine(0, short(y), short(size.x), 1, b);
    }
}
