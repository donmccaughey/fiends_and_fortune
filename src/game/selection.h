#ifndef FNF_GAME_MENU_H_INCLUDED
#define FNF_GAME_MENU_H_INCLUDED

#include <ncurses.h>
#include <menu.h>


struct game;
struct result;


typedef struct result
(selection_action_fn)(struct game *game);


struct selection {
    int index;
    ITEM **items;
    int items_count;
    MENU *menu;
    WINDOW *sub_window;
    char *title;
    WINDOW *window;
};


struct selection_item {
    selection_action_fn *action;
};


struct selection *
selection_alloc_or_die(char const *title);

void
selection_free_or_die(struct selection *selection);

struct result
selection_add_item(struct selection *selection,
                   char const *description,
                   selection_action_fn *action);

struct result
selection_show(struct selection *selection, WINDOW *parent);

selection_action_fn *
selection_selected_action(struct selection *selection);


#endif
