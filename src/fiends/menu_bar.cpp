#include "menu_bar.hpp"

#include "commands.hpp"


TMenuBar *
newMenuBar(TRect r)
{
    r.b.y = r.a.y + 1;

    return new TMenuBar(
        r,
        *new TSubMenu("~F~iends", kbAltH)
        + *new TMenuItem(
                "~G~reeting...", cmGreeting, kbAltG
        )
        + newLine()
        + *new TMenuItem(
                "E~x~it", cmQuit,
                cmQuit, hcNoContext, "Alt-X"
        )

        + *new TSubMenu("~T~reasure", kbAltT)
        + *new TMenuItem(
                "Treasure ~T~able", cmTreasureTable, kbAltT
        )
    );
}
