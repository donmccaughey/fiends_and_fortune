#ifndef FIENDS_TREASURE_WINDOW_HPP_INCLUDED
#define FIENDS_TREASURE_WINDOW_HPP_INCLUDED


#include "fiends.hpp"


class TreasureWindow : public TWindow {
public:
    explicit TreasureWindow(TRect const &bounds, char letter);
    ~TreasureWindow() override;
};


#endif
