#ifndef FIENDS_GENERATE_TREASURE_TREASURE_VIEW_HPP_INCLUDED
#define FIENDS_GENERATE_TREASURE_TREASURE_VIEW_HPP_INCLUDED


#include "fiends/util/text_rect.hpp"


struct treasure;


class TreasureView : public TScroller {
    unique_ptr<treasure, void(*)(treasure *)> aTreasure;
    TextRect text;

public:
    TreasureView(
        TRect const &bounds,
        TScrollBar *aHScrollBar,
        TScrollBar *aVScrollBar,
        unique_ptr<treasure, void(*)(treasure *)> &&aTreasure,
        TextRect &&textRect
    );

    void draw() override;
};


#endif
