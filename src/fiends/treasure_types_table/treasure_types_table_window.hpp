#ifndef FIENDS_TREASURE_TYPES_TABLE_TREASURE_TYPES_TABLE_WINDOW_HPP_INCLUDED
#define FIENDS_TREASURE_TYPES_TABLE_TREASURE_TYPES_TABLE_WINDOW_HPP_INCLUDED


#include "fiends/fiends.hpp"


class TreasureTypesTableWindow : public TWindow {
public:
    explicit TreasureTypesTableWindow(TRect const &bounds);
    ~TreasureTypesTableWindow() override;
};


#endif
