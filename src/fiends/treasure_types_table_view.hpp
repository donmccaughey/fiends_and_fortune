#ifndef FIENDS_TREASURE_TYPES_TABLE_VIEW_HPP_INCLUDED
#define FIENDS_TREASURE_TYPES_TABLE_VIEW_HPP_INCLUDED


#include "fiends.hpp"


class TreasureTypesTableView : public TScroller {
    vector<string> table;
    size_t width{};

public:
    TreasureTypesTableView(
        const TRect &bounds,
        TScrollBar *aHScrollBar,
        TScrollBar *aVScrollBar
    );

    void draw() override;

private:
    void appendLine(char const *source, size_t begin, size_t end);
    void appendLines(char const *source);
    void initializeTable();
};


#endif
