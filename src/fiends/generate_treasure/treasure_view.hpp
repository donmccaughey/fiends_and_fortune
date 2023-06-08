#ifndef FIENDS_TREASURE_VIEW_HPP_INCLUDED
#define FIENDS_TREASURE_VIEW_HPP_INCLUDED


#include "fiends/text_rect.hpp"


struct treasure;


class TreasureView : public TScroller {
    TextRect text;
    unique_ptr<treasure, void(*)(treasure *)> aTreasure;

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
