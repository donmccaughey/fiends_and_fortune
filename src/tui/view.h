#ifndef FNF_TUI_VIEW_H_INCLUDED
#define FNF_TUI_VIEW_H_INCLUDED


#include <ncurses.h>

#include "window.h"

#include "common/result.h"


struct app;
struct view;


typedef struct result
(view_create_fn)(struct view *view, struct app *app);

typedef void
(view_destroy_fn)(struct view *view, struct app *app);

typedef struct result
(view_draw_fn)(struct view *view, struct app *app);

typedef struct result
(view_on_key_fn)(struct view *view, struct app *app, int key);


struct view {
    view_create_fn *create;
    view_destroy_fn *destroy;
    view_draw_fn *draw;
    view_on_key_fn *on_key;
    void *user_data;
    WINDOW *window;
};


struct view *
view_alloc(view_create_fn *create,
           view_destroy_fn *destroy,
           view_draw_fn *draw,
           view_on_key_fn *on_key,
           void *user_data);

void
view_free(struct view *view);

inline struct result
view_enable_keyboard(struct view *view)
{
    return window_enable_keyboard(view->window);
}

inline struct result
view_invalidate(struct view *view)
{
    return window_invalidate(view->window);
}

inline int
view_read_key(struct view *view)
{
    return wgetch(view->window);
}

inline struct rect
view_get_rect(struct view *view)
{
    return window_get_rect(view->window);
}


#endif
