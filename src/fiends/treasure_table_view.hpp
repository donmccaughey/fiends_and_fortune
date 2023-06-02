#ifndef FIENDS_TREASURE_TABLE_VIEW_HPP_INCLUDED
#define FIENDS_TREASURE_TABLE_VIEW_HPP_INCLUDED


#include "fiends.hpp"


class TreasureTableView : public TScroller {
    vector<string> treasureTypes;
    size_t width{};

public:
    TreasureTableView(
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
