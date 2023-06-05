#ifndef FIENDS_TREASURE_TYPES_TABLE_VIEW_HPP_INCLUDED
#define FIENDS_TREASURE_TYPES_TABLE_VIEW_HPP_INCLUDED


#include "fiends.hpp"
#include "text_rect.hpp"


class TreasureTypesTableView : public TScroller {
    TextRect table;

public:
    TreasureTypesTableView(
        TRect const &bounds,
        TScrollBar *aHScrollBar,
        TScrollBar *aVScrollBar
    );

    void draw() override;
};


#endif
