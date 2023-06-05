#ifndef FIENDS_TREASURE_VIEW_HPP_INCLUDED
#define FIENDS_TREASURE_VIEW_HPP_INCLUDED


#include "fiends.hpp"


class TreasureView : public TScroller {
    vector<string> lines;
    size_t width{};

public:
    TreasureView(
        TRect const &bounds,
        TScrollBar *aHScrollBar,
        TScrollBar *aVScrollBar,
        char letter
    );

    void draw() override;

private:
    void initializeTreasure(char letter);
};


#endif
