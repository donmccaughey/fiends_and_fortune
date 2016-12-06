#ifndef FNF_TUI_APP_H_INCLUDED
#define FNF_TUI_APP_H_INCLUDED


#include <stdbool.h>

#include "rect.h"
#include "view.h"
#include "window.h"

#include "common/result.h"


struct app {
    struct view *active_view;
    bool is_running;
    struct view **views;
    int views_count;
};


struct app *
app_alloc(void);

void
app_free(struct app *app);

struct result
app_run(struct app *app);

struct result
app_activate_view(struct app *app, struct view *view);

void
app_quit(struct app *app);

inline struct view *
app_get_screen(struct app *app)
{
    return app->views[0];
}

inline struct rect
app_get_screen_rect(struct app *app)
{
    return window_get_rect(app_get_screen(app)->window);
}

#endif
