#include "game.h"

#include <assert.h>
#include "common/alloc_or_die.h"


struct {
    char const *name;
    char const *description;
} action_menu_items[] = {
    { "1", "Create Character" },
    { "2", "Generate Treasure" },
    { "3", "Generate Dungeon" },
    { "q", "Quit" }
};
int action_menu_items_count = sizeof action_menu_items
                            / sizeof action_menu_items[0];


struct game *
game_alloc(void)
{
    struct game *game = calloc_or_die(1, sizeof(struct game));
    
    WINDOW *main_window = initscr();
    if (!main_window) {
        game_free(game);
        return NULL;
    }
    
    int result = cbreak();
    if (ERR == result) {
        game_free(game);
        return NULL;
    }
    
    result = noecho();
    if (ERR == result) {
        game_free(game);
        return NULL;
    }
    
    nonl();
    
    result = intrflush(stdscr, FALSE);
    if (ERR == result) {
        game_free(game);
        return NULL;
    }
    
    result = keypad(stdscr, TRUE);
    if (ERR == result) {
        game_free(game);
        return NULL;
    }
    
    ITEM **items = calloc_or_die(action_menu_items_count + 1, sizeof(ITEM *));
    for (int i = 0; i < action_menu_items_count; ++i) {
        items[i] = new_item(action_menu_items[i].name, action_menu_items[i].description);
    }
    
    game->menu = new_menu(items);
    if (!game->menu) {
        game_free(game);
        return NULL;
    }
    
    return game;
}


void
game_free(struct game *game)
{
    if (!game) return;
    
    ITEM **items = menu_items(game->menu);
    for (int i = 0; i < item_count(game->menu); ++i) {
        free_item(items[0]);
    }
    free_or_die(items);
    free_menu(game->menu);
    endwin();
    free_or_die(game);
}


int
game_play(struct game *game)
{
    int result;
    
    wborder(stdscr, '|', '|', '-', '-', '+', '+', '+', '+');
    result = refresh();
    if (ERR == result) return -1;

    int menu_height;
    int menu_width;
    result = scale_menu(game->menu, &menu_height, &menu_width);
    if (E_OK != result) return -1;
    
    int main_window_height;
    int main_window_width;
    getmaxyx(stdscr, main_window_height, main_window_width);
    
    int menu_window_height = menu_height + 4;
    int menu_window_width = menu_width + 5;
    int menu_window_y = (main_window_height - menu_window_height) / 2;
    int menu_window_x = (main_window_width - menu_window_width) / 2;
    
    WINDOW *menu_window = newwin(menu_window_height, menu_window_width,
                                 menu_window_y, menu_window_x);
    if (!menu_window) return -1;
    
    result = keypad(menu_window, TRUE);
    if (ERR == result) {
        delwin(menu_window);
        return -1;
    }
    
    wborder(menu_window, '|', '|', '-', '-', '+', '+', '+', '+');
    
    result = set_menu_win(game->menu, menu_window);
    if (E_OK != result) {
        delwin(menu_window);
        return -1;
    }
    
    int menu_sub_height = menu_height;
    int menu_sub_width = menu_width;
    int menu_sub_y = 2;
    int menu_sub_x = 2;
    
    WINDOW *menu_sub = derwin(menu_window, menu_sub_height, menu_sub_width,
                              menu_sub_y, menu_sub_x);
    if (!menu_sub) {
        delwin(menu_window);
        return -1;
    }
    
    result = set_menu_sub(game->menu, menu_sub);
    if (E_OK != result) {
        delwin(menu_sub);
        delwin(menu_window);
        return -1;
    }
    
    result = set_menu_sub(game->menu, menu_sub);
    if (E_OK != result) {
        delwin(menu_sub);
        delwin(menu_window);
        return -1;
    }
    
    result = post_menu(game->menu);
    if (E_OK != result) {
        delwin(menu_sub);
        delwin(menu_window);
        return -1;
    }
    
    result = wrefresh(menu_window);
    if (ERR == result) {
        unpost_menu(game->menu);
        delwin(menu_sub);
        delwin(menu_window);
        return -1;
    }
    
    bool has_error = false;
    while (true) {
        int ch = wgetch(menu_window);
        
        if (ERR == ch) {
            has_error = true;
            break;
        }
        
        if (KEY_DOWN == ch) {
            result = menu_driver(game->menu, REQ_DOWN_ITEM);
            if (   E_REQUEST_DENIED != result
                && E_OK != result)
            {
                has_error = true;
            }
        }
        
        if (KEY_UP == ch) {
            result = menu_driver(game->menu, REQ_UP_ITEM);
            if (   E_REQUEST_DENIED != result
                && E_OK != result)
            {
                has_error = true;
            }
        }
        
        if ('1' == ch) break;
        if ('2' == ch) break;
        if ('3' == ch) break;
        if ('q' == ch) break;
    }
    
    result = unpost_menu(game->menu);
    if (E_OK != result) has_error = true;
    
    result = delwin(menu_sub);
    if (E_OK != result) has_error = true;

    result = delwin(menu_window);
    if (ERR == result) has_error = true;
    
    return has_error ? -1 : 0;
}
