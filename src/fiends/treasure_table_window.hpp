#ifndef FIENDS_TREASURE_TABLE_WINDOW_HPP_INCLUDED
#define FIENDS_TREASURE_TABLE_WINDOW_HPP_INCLUDED


#include "fiends.hpp"


class TreasureTableWindow : public TWindow {
public:
    explicit TreasureTableWindow(const TRect &bounds);
    ~TreasureTableWindow() override;
};


#endif
