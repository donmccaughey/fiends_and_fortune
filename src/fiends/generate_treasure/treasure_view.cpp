#include "treasure_view.hpp"

#include "fiends/ptr.hpp"

extern "C" {
#include "base/base.h"
#include "treasure/treasure.h"
}


static unique_ptr<treasure, void(*)(treasure *)>
generateTreasure(char letter)
{
    auto rnd = makeUnique(rnd_alloc(), rnd_free);

    auto treasure = makeUnique(
            new struct treasure,
            [](struct treasure *p) {
                treasure_finalize(p);
                delete p;
            }
    );
    treasure_initialize(treasure.get());

    treasure_type_generate(treasure_type_by_letter(letter), rnd.get(), treasure.get());

    return treasure;
}


static TextRect
treasureDetails(treasure *treasure)
{
    auto details = makeUnique(
            treasure_alloc_details(treasure),
            [](struct ptr_array *ptr_array) {
                ptr_array_clear(ptr_array, free_or_die);
                ptr_array_free(ptr_array);
            });
    return TextRect(details.get());
}


TreasureView::TreasureView(
        TRect const &bounds,
        TScrollBar *aHScrollBar,
        TScrollBar *aVScrollBar,
        char letter
) :
        TScroller(bounds, aHScrollBar, aVScrollBar),
        aTreasure(generateTreasure(letter))
{
    growMode = gfGrowHiX | gfGrowHiY;
    options |= ofFramed;
    text = treasureDetails(aTreasure.get());
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
