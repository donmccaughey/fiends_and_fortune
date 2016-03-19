#include "game.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

#include "common/alloc_or_die.h"
#include "common/result.h"


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


static struct result
get_menu_selection(MENU *menu, WINDOW *window);


static struct result
draw_window(WINDOW *window, char const *title)
{
    int result = wborder(window, '|', '|', '-', '-', '+', '+', '+', '+');
    if (OK != result) return result_make_ncurses_err();
    
    // TODO: handle UTF-8 correctly
    size_t title_string_length = strlen(title);
    if (title_string_length > (size_t)INT_MAX) {
        errno = ERANGE;
        return result_make_system_error();
    }
    
    if (title_string_length) {
        int title_string_width = (int)title_string_length;
        int window_height;
        int window_width;
        getmaxyx(window, window_height, window_width);
        
        char const left_frame[] = "| ";
        char const right_frame[] = " |";
        
        int const left_frame_width = sizeof(left_frame) - 1;
        int const right_frame_width = sizeof(right_frame) - 1;
        
        int const left_border_width = 1;
        int const right_border_width = 1;
        
        int window_top_width = window_width - left_border_width - right_border_width;
        int title_width = left_frame_width + title_string_width + right_frame_width;
        if (window_top_width >= title_width) {
            int start = left_border_width + (window_top_width - title_width) / 2;
            mvwprintw(window, 0, start, "%s%s%s", left_frame, title, right_frame);
        } else {
            // TODO: elide title
        }
    }
    
    result = wrefresh(window);
    return (ERR == result) ? result_make_ncurses_err() : result_make_success();
}


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
    struct result result = draw_window(stdscr, "Fiends and Fortune");
    if (!result_is_success(result)) return -1;
    
    int menu_height;
    int menu_width;
    int result_code = scale_menu(game->menu, &menu_height, &menu_width);
    if (E_OK != result_code) return -1;
    
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
    
    result_code = keypad(menu_window, TRUE);
    if (ERR == result_code) {
        delwin(menu_window);
        return -1;
    }
    
    result = draw_window(menu_window, "Action");
    if (!result_is_success(result)) {
        delwin(menu_window);
        return -1;
    }
    
    result_code = set_menu_win(game->menu, menu_window);
    if (E_OK != result_code) {
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
    
    result_code = set_menu_sub(game->menu, menu_sub);
    if (E_OK != result_code) {
        delwin(menu_sub);
        delwin(menu_window);
        return -1;
    }
    
    result_code = set_menu_sub(game->menu, menu_sub);
    if (E_OK != result_code) {
        delwin(menu_sub);
        delwin(menu_window);
        return -1;
    }
    
    result_code = post_menu(game->menu);
    if (E_OK != result_code) {
        delwin(menu_sub);
        delwin(menu_window);
        return -1;
    }
    
    result_code = wrefresh(menu_window);
    if (ERR == result_code) {
        unpost_menu(game->menu);
        delwin(menu_sub);
        delwin(menu_window);
        return -1;
    }
    
    result = get_menu_selection(game->menu, menu_window);
    if (!result_is_success(result)) return -1;
    
    bool has_error = false;
    result_code = unpost_menu(game->menu);
    if (E_OK != result_code) has_error = true;
    
    result_code = delwin(menu_sub);
    if (E_OK != result_code) has_error = true;

    result_code = delwin(menu_window);
    if (ERR == result_code) has_error = true;
    
    return has_error ? -1 : 0;
}


static struct result
get_menu_selection(MENU *menu, WINDOW *window)
{
    while (true) {
        int ch = wgetch(window);
        
        if (ERR == ch) return result_make_ncurses_err();
        
        if (KEY_DOWN == ch) {
            int result = menu_driver(menu, REQ_DOWN_ITEM);
            if (   E_REQUEST_DENIED != result
                && E_OK != result)
            {
                return result_make_ncurses_error(result);
            }
        }
        
        if (KEY_UP == ch) {
            int result = menu_driver(menu, REQ_UP_ITEM);
            if (   E_REQUEST_DENIED != result
                && E_OK != result)
            {
                return result_make_ncurses_error(result);
            }
        }
        
        if ('\r' == ch) break;
        
        if (!isalnum(ch)) continue;
        
        ITEM **items = menu_items(menu);
        if (items) {
            for (int i = 0; i < item_count(menu); ++i) {
                char const *name = item_name(items[i]);
                if (name && ch == name[0]) {
                    set_current_item(menu, items[i]);
                    return result_make_success();
                }
            }
        }
    }
    
    return result_make_success();
}
