#ifndef FIENDS_TREASURE_TYPES_TABLE_TREASURE_TYPES_TABLE_VIEW_HPP_INCLUDED
#define FIENDS_TREASURE_TYPES_TABLE_TREASURE_TYPES_TABLE_VIEW_HPP_INCLUDED


#include "fiends/fiends.hpp"

#include "fiends/util/text_rect.hpp"


class TreasureTypesTableView : public TScroller {
    TextRect const textRect;

public:
    TreasureTypesTableView(
        TRect const &bounds,
        TScrollBar *aHScrollBar,
        TScrollBar *aVScrollBar,
        TextRect &&textRect
    );

    void draw() override;
};


#endif
