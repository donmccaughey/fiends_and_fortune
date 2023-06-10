#include "magic_items_window.hpp"

#include "fiends/util/window_numbers.hpp"

#include "fiends/views/text_rect_view.hpp"


static string
makeTitle(int count)
{
    string title;
    switch (count) {
        case 1: title += "One"; break;
        case 2: title += "Two"; break;
        case 3: title += "Three"; break;
        case 4: title += "Four"; break;
        case 5: title += "Five"; break;
        case 6: title += "Six"; break;
        case 7: title += "Seven"; break;
        case 8: title += "Eight"; break;
        case 9: title += "Nine"; break;
        case 10: title += "Ten"; break;
        default: title += to_string(count);
    }
    title += " Magic Item";
    if (count > 1) title += "s";
    return title;
}


MagicItemsWindow::MagicItemsWindow(
    TRect const &bounds,
    int count,
    TextRect &&textRect
) :
    TWindowInit(&MagicItemsWindow::initFrame),
    TWindow(bounds, makeTitle(count), windowNumbers.takeNext())
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


MagicItemsWindow::~MagicItemsWindow()
{
    windowNumbers.release(number);
}
