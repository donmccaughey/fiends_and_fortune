#include "desk_top.hpp"


TDeskTop *
newDeskTop(TRect r)
{
    r.a.y++;
    r.b.y--;
    return new TDeskTop(r);
}
