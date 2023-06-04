#include "application.hpp"

#include "about_box.hpp"
#include "commands.hpp"
#include "desk_top.hpp"
#include "generate_treasure_dialog.hpp"
#include "menu_bar.hpp"
#include "status_line.hpp"
#include "treasure_types_table_window.hpp"


Application::Application() :
    TProgInit(&newStatusLine, &newMenuBar, &newDeskTop)
{
}


void
Application::aboutBox()
{
    auto d = newAboutBox();
    deskTop->execView(d);
    destroy(d);
}


void
Application::generateTreasure()
{
    struct GenerateTreasureDialog generateTreasureDialog = {
        .treasureType=" ",
    };

    auto d = newGenerateTreasureDialog();
    d->setData(&generateTreasureDialog);
    auto buttonPressed = deskTop->execView(d);
    if (buttonPressed != cmCancel) {
        d->getData(&generateTreasureDialog);
    }
    destroy(d);
    if (buttonPressed != cmCancel) {
        // new treasure window
    }
}


void
Application::handleEvent(TEvent &event)
{
    TApplication::handleEvent(event);
    if(event.what == evCommand) {
        switch(event.message.command) {
            case cmAboutBox:
                aboutBox();
                clearEvent(event);
                break;
            case cmGenerateTreasure:
                generateTreasure();
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
Application::treasureTypesTable()
{
    TRect r(0, 0, 60, 20);
    auto *treasureTypesTableWindow = new TreasureTypesTableWindow(r);
    deskTop->insert(treasureTypesTableWindow);
}