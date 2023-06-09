#include "treasure_window.hpp"

#include "fiends/util/window_numbers.hpp"

#include "fiends/views/text_rect_view.hpp"

extern "C" {
#include "treasure/treasure.h"
}


static string
makeTitle(char letter)
{
    return string("Treasure ") + string(1, letter);
}


TreasureWindow::TreasureWindow(
    TRect const &bounds,
    char letter,
    TextRect &&textRect
) :
    TWindowInit(&TreasureWindow::initFrame),
    TWindow(bounds, makeTitle(letter), windowNumbers.takeNext())
{
    auto r = getExtent();
    r.grow(-1, -1);
    auto hScrollBar = standardScrollBar(sbHorizontal | sbHandleKeyboard);
    auto vScrollBar = standardScrollBar(sbVertical | sbHandleKeyboard);
    insert(new TextRectView(r, hScrollBar, vScrollBar, std::move(textRect), 0x0301));
}


TreasureWindow::~TreasureWindow()
{
    windowNumbers.release(number);
}
