#include "application.hpp"

#include "commands.hpp"
#include "desk_top.hpp"
#include "menu_bar.hpp"
#include "status_line.hpp"

#include "fiends/about/about.hpp"
#include "fiends/generate_dungeon/generate_dungeon.hpp"
#include "fiends/generate_magic_items/generate_magic_items.hpp"
#include "fiends/generate_treasure/generate_treasure.hpp"
#include "fiends/treasure_types_table/treasure_types_table.hpp"


Application::Application() :
    TProgInit(&newStatusLine, &newMenuBar, &newDeskTop)
{
}


void
Application::handleEvent(TEvent &event)
{
    TApplication::handleEvent(event);
    if (event.what == evCommand) {
        switch(event.message.command) {
            case cmAbout:
                about(*this, event);
                break;
            case cmGenerateDungeon:
                generateDungeon(*this, event);
                break;
            case cmGenerateMagicItems:
                generateMagicItems(*this, event);
                break;
            case cmGenerateTreasure:
                generateTreasure(*this, event);
                break;
            case cmTreasureTypesTable:
                treasureTypesTable(*this, event);
                break;
            default:
                break;
        }
    }
}
