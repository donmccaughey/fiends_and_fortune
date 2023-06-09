#include "treasure_types_table.hpp"

#include "treasure_types_table_window.hpp"

#include "fiends/application/application.hpp"

#include "fiends/util/ptr.hpp"
#include "fiends/util/text_rect.hpp"

extern "C" {
#include "base/base.h"
#include "treasure/treasure.h"
}


static TextRect
treasureTypesDescription()
{
    auto types = vector<string>();
    for (char letter = 'A'; letter <= 'Z'; ++letter) {
        struct treasure_type *treasureType = treasure_type_by_letter(letter);
        auto description = makeUnique(
            treasure_type_alloc_description(treasureType, letter == 'A'),
            free_or_die
        );
        types.emplace_back(description.get());
    }
    return TextRect(types);
}


void
treasureTypesTable(Application &application, TEvent &event)
{
    TRect bounds(0, 0, 60, 20);
    TextRect textRect = treasureTypesDescription();
    auto treasureTypesTableWindow = new TreasureTypesTableWindow(
            bounds, move(textRect)
    );
    Application::deskTop->insert(treasureTypesTableWindow);
    application.clearEvent(event);
}
