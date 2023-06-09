#include "text_rect_view.hpp"



TextRectView::TextRectView(
    TRect const &bounds,
    TScrollBar *aHScrollBar,
    TScrollBar *aVScrollBar,
    TextRect &&textRect,
    ushort color
) :
        TScroller(bounds, aHScrollBar, aVScrollBar),
        textRect(std::move(textRect)),
        color(color)
{
    growMode = gfGrowHiX | gfGrowHiY;
    options |= ofFramed;
    setLimit(this->textRect.width(), this->textRect.height());
}


void
TextRectView::draw()
{
    auto aColor = getColor(this->color);
    string rightPad = (textRect.width() < size.x)
            ? string(size.x - textRect.width(), ' ')
            : string();
    for (int y = 0; y < size.y; ++y) {
        TDrawBuffer b;
        int j = delta.y + y;
        if (j < textRect.height()) {
            string line = textRect[j].substr(size_t(delta.x), size_t(size.x));
            b.moveStr(0, line, aColor);
            if (!rightPad.empty()) {
                b.moveStr(textRect.width(), rightPad.c_str(), aColor);
            }
        } else {
            b.moveChar(0, ' ', aColor, size.x);
        }
        writeLine(0, short(y), short(size.x), 1, b);
    }
}
