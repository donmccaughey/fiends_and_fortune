#include "window.h"

#include <ncurses.h>


struct result
window_enable_keyboard(WINDOW *window)
{
    int code = intrflush(window, FALSE);
    if (ERR == code) return result_ncurses_err();
    
    code = keypad(window, TRUE);
    if (ERR == code) return result_ncurses_err();
    
    return result_success();
}


extern inline struct rect
window_get_rect(WINDOW *window);
