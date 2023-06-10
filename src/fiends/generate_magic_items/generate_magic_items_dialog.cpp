#include "generate_magic_items_dialog.hpp"

#include "fiends/fiends.hpp"


TDialog *
newGenerateMagicItemsDialog()
{
    auto d = new TDialog(
        TRect(6, 3, 46, 11),
        "Generate Magic Items"
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

    auto inputLine = new TInputLine(TRect(27, 2, 31, 3), 3);
    d->insert(inputLine);

    d->insert(
        new TLabel(
            TRect(8, 2, 25, 3),
            "~N~umber of items:", inputLine
        )
    );

    return d;
}
