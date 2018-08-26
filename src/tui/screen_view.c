#include "screen_view.h"

#include <base/base.h>

#include "app.h"
#include "view.h"


static struct result
create(struct view *screen_view, struct app *app)
{
    screen_view->window = initscr();
    if (!screen_view->window) return result_ncurses_err();
    
    int code = raw();
    if (ERR == code) return result_ncurses_err();
    
    code = noecho();
    if (ERR == code) return result_ncurses_err();
    
    nonl();
    
    app_hide_cursor(app);
    
    return result_success();
}


static void
destroy(struct view *screen_view, struct app *app)
{
    endwin();
}


static struct result
draw(struct view *screen_view, struct app *app)
{
    int code;
    
    code = box(screen_view->window, 0, 0);
    if (ERR == code) return result_ncurses_err();
    
    struct rect rect = view_get_rect(screen_view);
    int x = rect.width - 5;
    int y = rect.height - 2;
    code = mvwprintw(screen_view->window, y, x, "tui");
    if (ERR == code) return result_ncurses_err();
    
    return result_success();
}


static struct result
on_key(struct view *screen_view, struct app *app, int key)
{
    static int const ctrl_Q = 'Q' - 64;
    if (ctrl_Q == key) {
        app_quit(app);
        return result_success();
    } else {
        return app_ring_bell(app);
    }
}


struct view *
screen_view_alloc(void)
{
    return view_alloc(create, destroy, draw, on_key, NULL);
}
