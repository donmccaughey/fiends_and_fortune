#include "game.h"

#include <ncurses.h>
#include <menu.h>

#include "common/alloc_or_die.h"


struct {
    char const *name;
    char const *description;
} action_menu_items[] = {
    { "character", "Create Character" },
    { "treasure", "Generate Treasure" },
    { "dungeon", "Generate Dungeon" },
    { "quit", "Quit" }
};
int action_menu_items_count = sizeof action_menu_items / sizeof action_menu_items[0];


struct game *
game_alloc(void)
{
    return NULL;
}


void
game_free(struct game *game)
{
    
}


int
game_play(struct game *game)
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    
    wborder(stdscr, '|', '|', '-', '-', '+', '+', '+', '+');
    
    ITEM **items = calloc_or_die(action_menu_items_count + 1, sizeof(ITEM *));
    for (int i = 0; i < action_menu_items_count; ++i) {
        items[i] = new_item(action_menu_items[i].name, action_menu_items[i].description);
    }
    
    MENU *menu = new_menu(items);
    mvprintw(LINES - 2, 1, "F1 to Exit");
    post_menu(menu);
    refresh();
    
    int ch;
    while ((ch = getch()) != KEY_F(1)) {
        switch (ch) {
            case KEY_DOWN:
                menu_driver(menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(menu, REQ_UP_ITEM);
                break;
            default:
                break;
        }
    }
    
    free_menu(menu);
    for (int i = 0; i < action_menu_items_count; ++i) {
        free_item(items[0]);
    }
    free_or_die(items);
    endwin();
    return 0;
}
