#include "view.h"

#include "common/alloc_or_die.h"
#include "common/result.h"


struct view *
view_alloc(view_create_fn *create,
           view_destroy_fn *destroy,
           view_draw_fn *draw,
           void *user_data)
{
    struct view *view = calloc_or_die(1, sizeof(struct view));
    view->create = create;
    view->destroy = destroy;
    view->draw = draw;
    view->user_data = user_data;
    return view;
}


void
view_free(struct view *view)
{
    if (!view) return;
    free_or_die(view);
}


extern inline struct rect
view_get_rect(struct view *view);

extern inline struct result
view_enable_keyboard(struct view *view);

extern inline struct result
view_invalidate(struct view *view);

extern inline int
view_read_char(struct view *view);
