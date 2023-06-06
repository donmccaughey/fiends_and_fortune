#include "application.hpp"

#include <cctype>

#include "about_dialog.hpp"
#include "commands.hpp"
#include "desk_top.hpp"
#include "generate_treasure_dialog.hpp"
#include "menu_bar.hpp"
#include "status_line.hpp"
#include "treasure_types_table_window.hpp"
#include "treasure_window.hpp"


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
Application::generateTreasure()
{
    struct GenerateTreasureDialogData data = {
        .letter=" ",
    };

    auto d = newGenerateTreasureDialog();
    d->setData(&data);
    auto buttonPressed = deskTop->execView(d);
    if (buttonPressed != cmCancel) {
        d->getData(&data);
    }
    destroy(d);
    if (buttonPressed != cmCancel) {
        char letter = char(toupper(data.letter[0]));
        if (letter >= 'A' && letter <= 'Z') {
            TRect r(0, 0, 60, 20);
            auto treasureWindow = new TreasureWindow(r, letter);
            deskTop->insert(treasureWindow);
        }
    }
}


void
Application::handleEvent(TEvent &event)
{
    TApplication::handleEvent(event);
    if(event.what == evCommand) {
        switch(event.message.command) {
            case cmAbout:
                about();
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
    auto treasureTypesTableWindow = new TreasureTypesTableWindow(r);
    deskTop->insert(treasureTypesTableWindow);
}
