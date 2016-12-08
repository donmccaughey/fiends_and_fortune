#include "screen.h"

#include "app.h"
#include "view.h"

#include "common/alloc_or_die.h"
#include "common/result.h"


static struct result
create(struct view *screen, struct app *app)
{
    screen->window = initscr();
    if (!screen->window) return result_ncurses_err();
    
    int code = raw();
    if (ERR == code) return result_ncurses_err();
    
    code = noecho();
    if (ERR == code) return result_ncurses_err();
    
    nonl();
    
    app_hide_cursor(app);
    
    return result_success();
}


static void
destroy(struct view *screen, struct app *app)
{
    endwin();
}


static struct result
draw(struct view *screen, struct app *app)
{
    struct rect rect = view_get_rect(screen);
    int x = rect.width - 5;
    int y = rect.height - 2;
    int code = mvwprintw(screen->window, y, x, "tui");
    if (ERR == code) return result_ncurses_err();
    
    code = move(rect.height, rect.width);
    return (ERR == code) ? result_ncurses_err() : result_success();
}


static struct result
on_key(struct view *view, struct app *app, int key)
{
    static int const ctrl_Q = 'Q' - 64;
    if (ctrl_Q == key) {
        app_quit(app);
    } else {
        int code = cbreak();
        if (ERR == code) return result_ncurses_err();
        
        code = beep();
        if (ERR == code) return result_ncurses_err();
        
        code = raw();
        if (ERR == code) return result_ncurses_err();
    }
    return result_success();
}


struct view *
screen_alloc(void)
{
    return view_alloc(create, destroy, draw, on_key, NULL);
}
