#include "screen.h"

#include "view.h"

#include "common/alloc_or_die.h"
#include "common/result.h"


static struct result
create(struct view *screen, struct app *app)
{
    screen->window = initscr();
    if (!screen->window) return result_ncurses_err();
    
    int code = cbreak();
    if (ERR == code) return result_ncurses_err();
    
    code = noecho();
    if (ERR == code) return result_ncurses_err();
    
    nonl();
    
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
    int code = box(screen->window, 0, 0);
    if (ERR == code) return result_ncurses_err();
    
    struct rect rect = view_get_rect(screen);
    int x = rect.width - 5;
    int y = rect.height - 2;
    code = mvwprintw(screen->window, y, x, "tui");
    if (ERR == code) return result_ncurses_err();
    
    code = move(1, 2);
    return (ERR == code) ? result_ncurses_err() : result_success();
}


struct view *
screen_alloc(void)
{
    return view_alloc(create, destroy, draw, NULL);
}
