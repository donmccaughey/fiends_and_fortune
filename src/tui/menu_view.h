#ifndef FNF_TUI_MENU_VIEW_H_INCLUDED
#define FNF_TUI_MENU_VIEW_H_INCLUDED


#include <menu.h>


struct view *
menu_view_alloc(char const *title);

struct result
menu_view_add_item(struct view *menu_view,
                   char const *description);


#endif
