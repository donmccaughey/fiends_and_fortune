#include "treasure_view.hpp"

extern "C" {
#include "base/base.h"
#include "treasure/treasure.h"
}


static TextRect
generateTreasure(char letter)
{
    struct rnd *rnd = rnd_alloc();
    struct treasure treasure{};
    treasure_initialize(&treasure);

    treasure_type_generate(treasure_type_by_letter(letter), rnd, &treasure);
    struct ptr_array *details = treasure_alloc_details(&treasure);
    TextRect text = TextRect(details);
    ptr_array_clear(details, free_or_die);
    ptr_array_free(details);

    treasure_finalize(&treasure);
    rnd_free(rnd);

    return text;
}


TreasureView::TreasureView(
        TRect const &bounds,
        TScrollBar *aHScrollBar,
        TScrollBar *aVScrollBar,
        char letter
) :
    TScroller(bounds, aHScrollBar, aVScrollBar),
    text(generateTreasure(letter))
{
    growMode = gfGrowHiX | gfGrowHiY;
    options |= ofFramed;
    setLimit(text.width(), text.height());
}


void
TreasureView::draw()
{
    auto color = getColor(0x0301);
    for (int y = 0; y < size.y; ++y) {
        TDrawBuffer b;
        int i = delta.y + y;
        if (size_t(i) < text.height()) {
            auto line = (size_t(delta.x) < text[i].length())
                    ? text[i].substr(size_t(delta.x))
                    : string();
            if (line.length() < size_t(size.x)) {
                auto count = size.x - line.length();
                line.append(count, ' ');
            } else {
                line.erase(size.x);
            }
            b.moveStr(0, line.c_str(), color);
        } else {
            b.moveChar(0, ' ', color, size.x);
        }
        writeLine(0, short(y), short(size.x), 1, b);
    }
}
