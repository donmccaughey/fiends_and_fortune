#include "fiends_app.hpp"

#include "commands.hpp"
#include "desk_top.hpp"
#include "menu_bar.hpp"
#include "status_line.hpp"


TFiendsApp::TFiendsApp() :
    TProgInit(
        &newStatusLine,
        &newMenuBar,
        &newDeskTop
    )
{
}


void
TFiendsApp::greetingBox()
{
    auto *d = new TDialog(
        TRect(25, 5, 55, 16),
        "Hello, World!"
    );

    d->insert(
        new TStaticText(
            TRect( 3, 5, 15, 6),
            "How are you?"
        )
    );

    d->insert(
        new TButton(
            TRect(16, 2, 28, 4),
            "Terrific", cmCancel, bfNormal
        )
    );
    d->insert(
        new TButton(
            TRect(16, 4, 28, 6),
            "Ok", cmCancel, bfNormal
        )
    );
    d->insert(
        new TButton(
            TRect(16, 6, 28, 8),
            "Lousy", cmCancel, bfNormal
        )
    );
    d->insert(
        new TButton(
            TRect(16, 8, 28, 10),
            "Cancel", cmCancel, bfNormal
        )
    );

    deskTop->execView(d);
    destroy(d);
}


void
TFiendsApp::handleEvent(TEvent &event)
{
    TApplication::handleEvent(event);
    if(event.what == evCommand) {
        switch(event.message.command) {
            case cmGreeting:
                greetingBox();
                clearEvent(event);
                break;
            default:
                break;
        }
    }
}
