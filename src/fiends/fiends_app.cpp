#include "fiends_app.hpp"

#include "about_box.hpp"
#include "commands.hpp"
#include "desk_top.hpp"
#include "menu_bar.hpp"
#include "status_line.hpp"
#include "treasure_table_window.hpp"


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
            case cmAbout:
                aboutBox();
                clearEvent(event);
                break;
            case cmTreasureTable:
                treasureTable();
                clearEvent(event);
                break;
            default:
                break;
        }
    }
}


void
FiendsApp::treasureTable()
{
    TRect r(0, 0, 40, 15);
    auto *treasureTableWindow = new TreasureTableWindow(r);
    deskTop->insert(treasureTableWindow);
}
