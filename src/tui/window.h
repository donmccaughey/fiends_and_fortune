#ifndef FNF_TUI_WINDOW_H_INCLUDED
#define FNF_TUI_WINDOW_H_INCLUDED


#include <ncurses.h>
#include <base/base.h>

#include "rect.h"


inline struct rect
window_get_rect(WINDOW *window)
{
    return (struct rect){
        .x=getbegx(window),
        .y=getbegy(window),
        .width=getmaxx(window),
        .height=getmaxy(window),
    };
}

struct result
window_enable_keyboard(WINDOW *window);

inline struct result
window_invalidate(WINDOW *window)
{
    int code = wnoutrefresh(window);
    return (ERR == code) ? result_ncurses_err() : result_success();
}

            
#endif
