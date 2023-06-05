#include "treasure_types_table_view.hpp"

#include "alloc_ptr.hpp"

extern "C" {
#include "base/base.h"
#include "treasure/treasure.h"
}


static TextRect
treasureTable()
{
    auto lines = vector<string>();
    for (char letter = 'A'; letter <= 'Z'; ++letter) {
        struct treasure_type *treasureType = treasure_type_by_letter(letter);
        auto description = makeAllocPtr(
            treasure_type_alloc_description(treasureType, letter == 'A')
        );
        lines.emplace_back(description.get());
    }
    return TextRect(lines);
}


TreasureTypesTableView::TreasureTypesTableView(
    const TRect &bounds,
    TScrollBar *aHScrollBar,
    TScrollBar *aVScrollBar
) :
    TScroller(bounds, aHScrollBar, aVScrollBar),
    table(treasureTable())
{
    growMode = gfGrowHiX | gfGrowHiY;
    options |= ofFramed;
    setLimit(table.width(), table.height());
}


void
TreasureTypesTableView::draw()
{
    auto color = getColor(0x0301);
    for (int y = 0; y < size.y; ++y) {
        TDrawBuffer b;
        int i = delta.y + y;
        if (i < table.height()) {
            auto line = (size_t(delta.x) < table[i].length())
                    ? table[i].substr(size_t(delta.x))
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
