#include "treasure_view.hpp"

#include "ptr.hpp"

extern "C" {
#include "base/base.h"
#include "treasure/treasure.h"
}


static TextRect
generateTreasure(char letter)
{
    auto rnd = makeUnique(rnd_alloc(), rnd_free);

    struct treasure treasure{};
    treasure_initialize(&treasure);

    treasure_type_generate(treasure_type_by_letter(letter), rnd.get(), &treasure);
    auto details = makeUnique(
            treasure_alloc_details(&treasure),
            [](struct ptr_array *ptr_array) {
                ptr_array_clear(ptr_array, free_or_die);
                ptr_array_free(ptr_array);
            });
    TextRect text = TextRect(details.get());

    treasure_finalize(&treasure);

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
    auto rightPad = (text.width() < size.x) ? string(size.x - text.width(), ' ') : string();
    for (int y = 0; y < size.y; ++y) {
        TDrawBuffer b;
        int j = delta.y + y;
        if (j < text.height()) {
            auto line = text[j].substr(size_t(delta.x), size_t(size.x));
            b.moveStr(0, line, color);
            if (!rightPad.empty()) b.moveStr(text.width(), rightPad.c_str(), color);
        } else {
            b.moveChar(0, ' ', color, size.x);
        }
        writeLine(0, short(y), short(size.x), 1, b);
    }
}
