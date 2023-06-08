#include "generate_treasure.hpp"

#include <cctype>

#include "generate_treasure_dialog.hpp"
#include "treasure_window.hpp"

#include "fiends/application.hpp"


void
generateTreasure(Application &application, TEvent &event)
{
    struct GenerateTreasureDialogData data = {
        .letter=" ",
    };

    auto d = newGenerateTreasureDialog();
    d->setData(&data);
    auto buttonPressed = Application::deskTop->execView(d);
    if (buttonPressed != cmCancel) {
        d->getData(&data);
    }
    Application::destroy(d);
    if (buttonPressed != cmCancel) {
        char letter = char(toupper(data.letter[0]));
        if (letter >= 'A' && letter <= 'Z') {
            TRect r(0, 0, 60, 20);
            auto treasureWindow = new TreasureWindow(r, letter);
            Application::deskTop->insert(treasureWindow);
        }
    }
    application.clearEvent(event);
}
