#ifndef FIENDS_TREASURE_TYPES_TABLE_WINDOW_HPP_INCLUDED
#define FIENDS_TREASURE_TYPES_TABLE_WINDOW_HPP_INCLUDED


#include "fiends.hpp"


class TreasureTypesTableWindow : public TWindow {
public:
    explicit TreasureTypesTableWindow(const TRect &bounds);
    ~TreasureTypesTableWindow() override;
};


#endif
