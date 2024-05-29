#include "generate_magic_items.hpp"

#include "generate_magic_items_dialog.hpp"
#include "magic_items_window.hpp"

#include "fiends/application/application.hpp"

#include "fiends/util/ptr.hpp"
#include "fiends/util/text_rect.hpp"

extern "C" {
#include "base/base.h"
#include "treasure/treasure.h"

#include "astr.h"
}


static magic_item *
newMagicItems(int count)
{
    auto rnd = makeUnique(rnd_alloc(), rnd_free);
    auto *magicItems = new magic_item[count];
    for (int i = 0; i < count; ++i) {
        magic_item_initialize(&magicItems[i]);
        magic_item_generate(&magicItems[i], rnd.get(), ANY_MAGIC_ITEM);
    }
    return magicItems;
}


static TextRect
magicItemDetails(magic_item const *magicItems, int count)
{
    vector<string> details;

    details.emplace_back("Magic Items:");
    for (int i = 0; i < count; ++i) {
        char *description = astr_f("  %4i %s", (i + 1), magicItems[i].true_description);
        details.emplace_back(description);
        free(description);
        if (magicItems[i].true_details) {
            int j = 0;
            while (magicItems[i].true_details[j]) {
                char *detail = astr_f("           %s", magicItems[i].true_details[j]);
                details.emplace_back(detail);
                free(detail);
                ++j;
            }
        }
        details.emplace_back("");
    }

    return TextRect(details);
}


void
generateMagicItems(Application &application, TEvent &event)
{
    GenerateMagicItemsDialogData data = {
        .numberOfItems="",
    } ;
    TDialog *d = newGenerateMagicItemsDialog();
    d->setData(&data);
    ushort buttonPressed = Application::deskTop->execView(d);
    if (buttonPressed != cmCancel) {
        d->getData(&data);
    }
    Application::destroy(d);
    if (buttonPressed != cmCancel) {
        errno = 0;
        char *end;
        long numberOfItems = strtol(data.numberOfItems, &end, 10);
        if (!errno && end != data.numberOfItems && numberOfItems > 0) {
            int count = int(numberOfItems);
            TRect bounds(0, 0, 60, 20);

            auto magicItems = newMagicItems(count);
            TextRect textRect = magicItemDetails(magicItems, count);
            for (int i = 0; i < count; ++i) {
                magic_item_finalize(&magicItems[i]);
            }
            delete[] magicItems;

            auto *window = new MagicItemsWindow(
                    bounds, count, move(textRect)
            );
            Application::deskTop->insert(window);
        }
    }
    application.clearEvent(event);
}
