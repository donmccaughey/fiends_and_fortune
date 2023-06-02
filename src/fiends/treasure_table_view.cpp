#include "treasure_table_view.hpp"

#include "alloc_ptr.hpp"

extern "C" {
#include "base/base.h"
#include "treasure/treasure.h"
}


TreasureTableView::TreasureTableView(
    const TRect &bounds,
    TScrollBar *aHScrollBar,
    TScrollBar *aVScrollBar
) :
    TScroller(bounds, aHScrollBar, aVScrollBar)
{
    growMode = gfGrowHiX | gfGrowHiY;
    options |= ofFramed;
    initializeTable();
    setLimit(int(width), int(treasureTypes.size()));
}


void
TreasureTableView::appendLine(char const *source, size_t begin, size_t end)
{
    auto start = source + begin;
    auto length = end - begin;
    treasureTypes.emplace_back(start, length);
    width = max(length, width);
}


void
TreasureTableView::appendLines(char const *source)
{
    size_t begin = 0;
    size_t end = 0;
    while (source && source[end]) {
        if ('\n' == source[end]) {
            appendLine(source, begin, end);
            begin = end + 1;
        }
        ++end;
    }
    if (begin < end) {
        appendLine(source, begin, end);
    }
}


void
TreasureTableView::draw()
{
    auto color = getColor(0x0301);
    for (int y = 0; y < size.y; ++y) {
        TDrawBuffer b;
        int i = delta.y + y;
        if (size_t(i) < treasureTypes.size()) {
            auto line = (delta.x < treasureTypes[i].length())
                    ? treasureTypes[i].substr(size_t(delta.x))
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


void
TreasureTableView::initializeTable()
{
    for (char letter = 'A'; letter <= 'Z'; ++letter) {
        struct treasure_type *treasureType = treasure_type_by_letter(letter);
        auto description = makeAllocPtr(
            treasure_type_alloc_description(treasureType, letter == 'A')
        );
        appendLines(description.get());
    }
}
