#include "fiends_app.h"


int const GREET_THEM_CMD = 100;


TFiendsApp::TFiendsApp() :
    TProgInit(
        &TFiendsApp::initStatusLine,
        &TFiendsApp::initMenuBar,
        &TFiendsApp::initDeskTop
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
            case GREET_THEM_CMD:
                greetingBox();
                clearEvent(event);
                break;
            default:
                break;
        }
    }
}


TMenuBar *
TFiendsApp::initMenuBar(TRect r)
{
    r.b.y = r.a.y + 1;

    return new TMenuBar(
        r,
        *new TSubMenu("~F~iends", kbAltH)
        + *new TMenuItem(
            "~G~reeting...", GREET_THEM_CMD,
            kbAltG
        )
        + newLine()
        + *new TMenuItem(
            "E~x~it", cmQuit,
            cmQuit, hcNoContext, "Alt-X"
        )
    );
}


TStatusLine *
TFiendsApp::initStatusLine(TRect r)
{
    r.a.y = r.b.y - 1;
    return new TStatusLine(
        r,
        *new TStatusDef(0, 0xFFFF)
        + *new TStatusItem("~Alt-X~ Exit", kbAltX, cmQuit)
        + *new TStatusItem(0, kbF10, cmMenu )
    );
}
