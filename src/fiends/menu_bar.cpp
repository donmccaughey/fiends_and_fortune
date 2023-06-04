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
                "~A~bout...", cmAboutBox, kbAltA
        )
        + newLine()
        + *new TMenuItem(
                "E~x~it", cmQuit,
                cmQuit, hcNoContext, "Alt-X"
        )

        + *new TSubMenu("~R~eference", kbAltR)
        + *new TMenuItem(
                "~T~reasure Types Table", cmTreasureTypesTable, kbAltT
        )
    );
}
