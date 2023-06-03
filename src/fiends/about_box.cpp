#include "about_box.hpp"


#include "fiends.hpp"


TDialog *
newAboutBox()
{
    auto d = new TDialog(
        TRect(6, 3, 36, 11),
        "About Fiends"
    );

    d->insert(
        new TStaticText(
            TRect(6, 2, 25, 3),
            "Fiends and Fortune"
        )
    );

    d->insert(
        new TButton(
            TRect(10, 5, 20, 7),
            "Ok", cmCancel, bfNormal
        )
    );

    return d;
}
