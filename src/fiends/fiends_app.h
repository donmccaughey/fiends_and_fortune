#ifndef FIENDS_FIENDS_APP_H_INCLUDED
#define FIENDS_FIENDS_APP_H_INCLUDED


#define Uses_TApplication
#define Uses_TButton
#define Uses_TDeskTop
#define Uses_TDialog
#define Uses_TEvent
#define Uses_TKeys
#define Uses_TMenuBar
#define Uses_TMenuItem
#define Uses_TRect
#define Uses_TStaticText
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_TStatusLine
#define Uses_TSubMenu
#include <tvision/tv.h>


class TFiendsApp : public TApplication {
public:
    TFiendsApp();

    void handleEvent(TEvent& event) override;
    static TMenuBar *initMenuBar(TRect r);
    static TStatusLine *initStatusLine(TRect r);

private:
    void greetingBox();
};


#endif
