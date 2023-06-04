#include "fiends_app.hpp"

#include "about_box.hpp"
#include "commands.hpp"
#include "desk_top.hpp"
#include "menu_bar.hpp"
#include "status_line.hpp"
#include "treasure_types_table_window.hpp"


FiendsApp::FiendsApp() :
    TProgInit(&newStatusLine, &newMenuBar, &newDeskTop)
{
}


void
FiendsApp::aboutBox()
{
    auto d = newAboutBox();
    deskTop->execView(d);
    destroy(d);
}


void
FiendsApp::handleEvent(TEvent &event)
{
    TApplication::handleEvent(event);
    if(event.what == evCommand) {
        switch(event.message.command) {
            case cmAboutBox:
                aboutBox();
                clearEvent(event);
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
FiendsApp::treasureTypesTable()
{
    TRect r(0, 0, 60, 20);
    auto *treasureTypesTableWindow = new TreasureTypesTableWindow(r);
    deskTop->insert(treasureTypesTableWindow);
}
