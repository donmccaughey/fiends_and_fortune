#include "generate_treasure.hpp"

#include <cctype>

#include "generate_treasure_dialog.hpp"
#include "treasure_window.hpp"

#include "fiends/application/application.hpp"

#include "fiends/util/ptr.hpp"
#include "fiends/util/text_rect.hpp"

extern "C" {
#include "base/base.h"
#include "treasure/treasure.h"
}


static TextRect
treasureDetails(treasure *treasure)
{
    auto details = makeUnique(
        treasure_alloc_details(treasure),
        [](struct ptr_array *ptr_array) {
            ptr_array_clear(ptr_array, free_or_die);
            ptr_array_free(ptr_array);
        }
    );
    return TextRect(details.get());
}


static unique_ptr<treasure, void(*)(treasure *)>
newTreasure(char letter)
{
    auto rnd = makeUnique(rnd_alloc(), rnd_free);

    auto treasure = makeUnique(
        new struct treasure,
        [](struct treasure *p) {
            treasure_finalize(p);
            delete p;
        }
    );
    treasure_initialize(treasure.get());

    treasure_type_generate(treasure_type_by_letter(letter), rnd.get(), treasure.get());

    return treasure;
}


void
generateTreasure(Application &application, TEvent &event)
{
    GenerateTreasureDialogData data = {
        .treasureType="",
    };
    TDialog *d = newGenerateTreasureDialog();
    d->setData(&data);
    ushort buttonPressed = Application::deskTop->execView(d);
    if (buttonPressed != cmCancel) {
        d->getData(&data);
    }
    Application::destroy(d);
    if (buttonPressed != cmCancel) {
        char letter = char(toupper(data.treasureType[0]));
        if (letter >= 'A' && letter <= 'Z') {
            TRect bounds(0, 0, 60, 20);
            auto aTreasure = newTreasure(letter);
            TextRect textRect = treasureDetails(aTreasure.get());
            auto *window = new TreasureWindow(
                    bounds, letter, move(textRect)
            );
            Application::deskTop->insert(window);
        }
    }
    application.clearEvent(event);
}
