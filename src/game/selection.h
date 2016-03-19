#ifndef FNF_GAME_MENU_H_INCLUDED
#define FNF_GAME_MENU_H_INCLUDED

#include <ncurses.h>
#include <menu.h>


struct result;


struct selection {
    int index;
    ITEM **items;
    int items_count;
    MENU *menu;
    WINDOW *sub_window;
    char *title;
    WINDOW *window;
};


struct selection *
selection_alloc(char const *title);

void
selection_free(struct selection *selection);

struct result
selection_add_item(struct selection *selection, char const *description);

struct result
selection_show(struct selection *selection, WINDOW *parent);

int
selection_index(struct selection *selection);


#endif
