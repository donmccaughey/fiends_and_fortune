#include "generate_dungeon.hpp"

#include "dungeon_window.hpp"

#include "fiends/application/application.hpp"

#include "fiends/util/text_rect.hpp"


static TextRect
dungeonMap()
{
    vector<string> map;

    return TextRect(map);
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
