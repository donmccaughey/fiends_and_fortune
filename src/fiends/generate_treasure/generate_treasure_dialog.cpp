#include "generate_treasure_dialog.hpp"


#include "fiends/fiends.hpp"


TDialog *
newGenerateTreasureDialog()
{
    auto d = new TDialog(
        TRect(6, 3, 46, 11),
        "Generate Treasure"
    );

    d->insert(
        new TButton(
            TRect(15, 5, 25, 7),
            "~O~K", cmOK, bfDefault
        )
    );

    d->insert(
        new TButton(
            TRect(28, 5, 38, 7),
            "~C~ancel", cmCancel, bfNormal
        )
    );

    auto inputLine = new TInputLine(TRect(26, 2, 29, 3), 2);
    d->insert(inputLine);

    d->insert(
        new TLabel(
            TRect(10, 2, 24, 3),
            "~T~reasure Type:", inputLine
        )
    );

    return d;
}
