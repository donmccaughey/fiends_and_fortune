#include "application.hpp"

#include "commands.hpp"
#include "desk_top.hpp"
#include "menu_bar.hpp"
#include "status_line.hpp"

#include "about/about.hpp"

#include "generate_treasure/generate_treasure.hpp"

#include "treasure_types_table/treasure_types_table.hpp"


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
