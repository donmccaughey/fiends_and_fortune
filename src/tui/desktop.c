#include "desktop.h"

#include "common/alloc_or_die.h"
#include "common/result.h"


struct desktop *
desktop_alloc(void)
{
    return calloc_or_die(1, sizeof(struct desktop));
}


void
desktop_free(struct desktop *desktop)
{
    if (!desktop) return;
    free_or_die(desktop);
}


void
desktop_hide(struct desktop *desktop)
{
    endwin();
}


struct result
desktop_show(struct desktop *desktop)
{
    desktop->window = initscr();
    if (!desktop->window) return result_ncurses_err();
    
    int code = cbreak();
    if (ERR == code) return result_ncurses_err();

    code = noecho();
    if (ERR == code) return result_ncurses_err();

    nonl();
    
    return result_success();
}


extern inline struct rect
desktop_get_rect(struct desktop *desktop);
