#ifndef FIENDS_GENERATE_MAGIC_ITEMS_MAGIC_ITEMS_WINDOW_HPP_INCLUDED
#define FIENDS_GENERATE_MAGIC_ITEMS_MAGIC_ITEMS_WINDOW_HPP_INCLUDED


#include "fiends/fiends.hpp"


class TextRect;
struct treasure;


class MagicItemsWindow : public TWindow {
public:
    explicit MagicItemsWindow(
        TRect const &bounds,
        int count,
        TextRect &&textRect
    );
    ~MagicItemsWindow() override;
};


#endif
