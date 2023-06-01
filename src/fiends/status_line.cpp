#include "status_line.hpp"


TStatusLine *
newStatusLine(TRect r)
{
    r.a.y = r.b.y - 1;
    return new TStatusLine(
        r,
        *new TStatusDef(0, 0xFFFF)
        + *new TStatusItem("~Alt-X~ Exit", kbAltX, cmQuit)
        + *new TStatusItem(0, kbF10, cmMenu )
    );
}
