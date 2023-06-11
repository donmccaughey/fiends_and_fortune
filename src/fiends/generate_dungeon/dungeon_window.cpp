#include "dungeon_window.hpp"

#include "fiends/util/window_numbers.hpp"

#include "fiends/views/text_rect_view.hpp"


DungeonWindow::DungeonWindow(
    TRect const &bounds, TStringView title, TextRect &&textRect
) :
    TWindowInit(&DungeonWindow::initFrame),
    TWindow(bounds, title, windowNumbers.takeNext())
{
    auto viewBounds = getExtent();
    viewBounds.grow(-1, -1);
    insert(
            new TextRectView(
                    viewBounds,
                    standardScrollBar(sbHorizontal | sbHandleKeyboard),
                    standardScrollBar(sbVertical | sbHandleKeyboard),
                    move(textRect),
                    0x0301
            )
    );
}


DungeonWindow::~DungeonWindow()
{
    windowNumbers.release(number);
}
