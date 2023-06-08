#include "treasure_window.hpp"

#include "treasure_view.hpp"
#include "fiends/util/window_numbers.hpp"


static string
makeTitle(char letter)
{
    return string("Treasure ") + string(1, letter);
}


TreasureWindow::TreasureWindow(TRect const &bounds, char letter) :
    TWindowInit(&TreasureWindow::initFrame),
    TWindow(bounds, makeTitle(letter), windowNumbers.takeNext())
{
    auto r = getExtent();
    r.grow(-1, -1);
    auto hScrollBar = standardScrollBar(sbHorizontal | sbHandleKeyboard);
    auto vScrollBar = standardScrollBar(sbVertical | sbHandleKeyboard);
    insert(new TreasureView(r, hScrollBar, vScrollBar, letter));
}


TreasureWindow::~TreasureWindow()
{
    windowNumbers.release(number);
}
