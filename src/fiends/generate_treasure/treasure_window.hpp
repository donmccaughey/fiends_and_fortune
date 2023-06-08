#ifndef FIENDS_GENERATE_TREASURE_TREASURE_WINDOW_HPP_INCLUDED
#define FIENDS_GENERATE_TREASURE_TREASURE_WINDOW_HPP_INCLUDED


#include "fiends/fiends.hpp"


class TreasureWindow : public TWindow {
public:
    explicit TreasureWindow(TRect const &bounds, char letter);
    ~TreasureWindow() override;
};


#endif
