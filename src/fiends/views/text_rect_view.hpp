#ifndef FIENDS_VIEWS_TEXT_RECT_VIEW_HPP_INCLUDED
#define FIENDS_VIEWS_TEXT_RECT_VIEW_HPP_INCLUDED


#include "fiends/util/text_rect.hpp"


class TextRectView : public TScroller {
    TextRect textRect;
    ushort color;

public:
    TextRectView(
        TRect const &bounds,
        TScrollBar *aHScrollBar,
        TScrollBar *aVScrollBar,
        TextRect &&textRect,
        ushort color
    );

    void draw() override;
};


#endif
