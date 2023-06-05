#ifndef FIENDS_TREASURE_VIEW_HPP_INCLUDED
#define FIENDS_TREASURE_VIEW_HPP_INCLUDED


#include "fiends.hpp"
#include "text_rect.hpp"


class TreasureView : public TScroller {
    TextRect text;

public:
    TreasureView(
        TRect const &bounds,
        TScrollBar *aHScrollBar,
        TScrollBar *aVScrollBar,
        char letter
    );

    void draw() override;
};


#endif
