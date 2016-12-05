#ifndef FNF_TUI_DESKTOP_H_INCLUDED
#define FNF_TUI_DESKTOP_H_INCLUDED


#include <ncurses.h>

#include "window.h"


struct result;


struct desktop {
    WINDOW *window;
};


struct desktop *
desktop_alloc(void);

void
desktop_free(struct desktop *desktop);

inline struct rect
desktop_get_rect(struct desktop *desktop)
{
    return window_get_rect(desktop->window);
}

struct result
desktop_show(struct desktop *desktop);

void
desktop_hide(struct desktop *desktop);


#endif
