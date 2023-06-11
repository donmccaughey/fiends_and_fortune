#ifndef FIENDS_GENERATE_DUNGEON_DUNGEON_WINDOW_HPP_INCLUDED
#define FIENDS_GENERATE_DUNGEON_DUNGEON_WINDOW_HPP_INCLUDED


#include "fiends/fiends.hpp"


class TextRect;


class DungeonWindow : public TWindow {
public:
    explicit DungeonWindow(
        TRect const &bounds,
        TStringView title,
        TextRect &&textRect
    );
    ~DungeonWindow() override;
};


#endif
