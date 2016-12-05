#ifndef FNF_TUI_WINDOW_H_INCLUDED
#define FNF_TUI_WINDOW_H_INCLUDED


#include <ncurses.h>

#include "rect.h"

#include "common/result.h"


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

            
#endif
