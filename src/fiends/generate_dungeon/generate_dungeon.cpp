#include "generate_dungeon.hpp"

#include "dungeon_window.hpp"

#include "fiends/application/application.hpp"

#include "fiends/util/ptr.hpp"
#include "fiends/util/text_rect.hpp"

extern "C" {
#include "base/base.h"
#include "dungeon/dungeon.h"
}


static TextRect
dungeonMap()
{
    auto dungeon = makeUnique(dungeon_alloc(), dungeon_free);
    dungeon_generate_small(dungeon.get());

    auto levelMap = makeUnique(
            level_map_alloc(dungeon.get(), 1),
            level_map_free);
    auto textRectangle = makeUnique(
            level_map_alloc_text_rectangle(levelMap.get(), true),
            text_rectangle_free);

    return TextRect(textRectangle->chars);
}


void
generateDungeon(Application &application, TEvent &event)
{
    TRect bounds(0, 0, 60, 20);
    string title("A Dungeon");
    TextRect textRect = dungeonMap();
    auto *window = new DungeonWindow(
            bounds, title, move(textRect)
    );
    Application::deskTop->insert(window);
    application.clearEvent(event);
}
