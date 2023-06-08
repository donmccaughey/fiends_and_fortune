#include "application.hpp"

#include "about_dialog.hpp"
#include "commands.hpp"
#include "desk_top.hpp"
#include "menu_bar.hpp"
#include "status_line.hpp"
#include "treasure_types_table_window.hpp"

#include "generate_treasure/generate_treasure.hpp"


Application::Application() :
    TProgInit(&newStatusLine, &newMenuBar, &newDeskTop)
{
}


void
Application::about()
{
    auto d = newAboutDialog();
    deskTop->execView(d);
    destroy(d);
}


void
Application::handleEvent(TEvent &event)
{
    TApplication::handleEvent(event);
    if (event.what == evCommand) {
        switch(event.message.command) {
            case cmAbout:
                about();
                clearEvent(event);
                break;
            case cmGenerateTreasure:
                generateTreasure(*this, event);
                break;
            case cmTreasureTypesTable:
                treasureTypesTable();
                clearEvent(event);
                break;
            default:
                break;
        }
    }
}


void
Application::treasureTypesTable()
{
    TRect r(0, 0, 60, 20);
    auto treasureTypesTableWindow = new TreasureTypesTableWindow(r);
    deskTop->insert(treasureTypesTableWindow);
}
