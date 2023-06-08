#ifndef FIENDS_GENERATE_TREASURE_TREASURE_WINDOW_HPP_INCLUDED
#define FIENDS_GENERATE_TREASURE_TREASURE_WINDOW_HPP_INCLUDED


#include "fiends/fiends.hpp"


class TextRect;
struct treasure;


class TreasureWindow : public TWindow {
public:
    explicit TreasureWindow(
        TRect const &bounds,
        char letter,
        unique_ptr<treasure, void(*)(treasure *)> &&aTreasure,
        TextRect &&textRect
    );
    ~TreasureWindow() override;
};


#endif
