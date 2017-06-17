#include "menu_view.h"

#include <ctype.h>
#include <limits.h>
#include <menu.h>

#include "app.h"
#include "rect.h"
#include "view.h"

#include "common/alloc_or_die.h"
#include "common/result.h"


struct menu {
    ITEM **items;
    int items_count;
    MENU *menu;
    int selection;
    WINDOW *sub_window;
    char *title;
};


static struct result
menu_add_item(struct menu *menu, char const *description)
{
    int index = menu->items_count;
    int null_index = index + 1;
    ++menu->items_count;
    menu->items = reallocarray_or_die(menu->items,
                                      menu->items_count + 1,
                                      sizeof(ITEM *));
    char name[] = "1";
    name[0] += index;
    char *name_dup = strdup_or_die(name);
    char *description_dup = strdup_or_die(description);
    menu->items[index] = new_item(name_dup, description_dup);
    if (!menu->items[index]) {
        free_or_die(description_dup);
        free_or_die(name_dup);
        return result_system_error();
    }
    menu->items[null_index] = NULL;
    
    return result_success();
}


static struct menu *
menu_alloc(char const *title)
{
    struct menu *menu = calloc_or_die(1, sizeof(struct menu));
    menu->items = calloc_or_die(1, sizeof(ITEM *));
    menu->selection = -1;
    menu->title = strdup_or_die(title);
    return menu;
}


static void
menu_free(struct menu *menu)
{
    if (!menu) return;
    
    for (int i = 0; i < menu->items_count; ++i) {
        free_or_die((char *)item_name(menu->items[i]));
        free_or_die((char *)item_description(menu->items[i]));
        free_item(menu->items[i]);
    }
    free_or_die(menu->title);
    free_or_die(menu->items);
    free_or_die(menu);
}


static struct result
create(struct view *menu_view, struct app *app)
{
    struct menu *menu = menu_view->user_data;
    menu->menu = new_menu(menu->items);
    
    int menu_height;
    int menu_width;
    int code = scale_menu(menu->menu, &menu_height, &menu_width);
    if (E_OK != code) return result_ncurses_error(code);
        
    int title_width = (int)strlen(menu->title) + 1;
    if (title_width > menu_width) menu_width = title_width;
        
    struct rect screen_rect = app_get_screen_view_rect(app);
    
    int window_height = menu_height + 4;
    int window_width = menu_width + 5;
    int window_y = (screen_rect.height - menu_height) / 2;
    int window_x = (screen_rect.width - menu_width) / 2;
    
    menu_view->window = newwin(window_height, window_width,
                               window_y, window_x);
    if (!menu_view->window) return result_ncurses_err();
    
    code = set_menu_win(menu->menu, menu_view->window);
    if (E_OK != code) return result_ncurses_error(code);
    
    int menu_sub_height = menu_height;
    int menu_sub_width = menu_width;
    int menu_sub_y = 2;
    int menu_sub_x = 2;
    
    menu->sub_window = derwin(menu_view->window,
                              menu_sub_height, menu_sub_width,
                              menu_sub_y, menu_sub_x);
    if (!menu->sub_window) return result_ncurses_err();
            
    code = set_menu_sub(menu->menu, menu->sub_window);
    if (E_OK != code) return result_ncurses_error(code);
    
    menu->selection = 0;
    
    code = post_menu(menu->menu);
    if (E_OK != code) return result_ncurses_error(code);
    
    return result_success();
}


static void
destroy(struct view *menu_view, struct app *app)
{
    struct menu *menu = menu_view->user_data;
    
    int code = unpost_menu(menu->menu);
    if (E_OK != code) return;
    
    code = free_menu(menu->menu);
    if (E_OK != code) return;
    
    code = delwin(menu->sub_window);
    if (E_OK != code) return;
    
    code = delwin(menu_view->window);
    if (E_OK != code) return;
    
    menu_free(menu);
}


static struct result
draw(struct view *menu_view, struct app *app)
{
    struct menu *menu = menu_view->user_data;
    
    int code = box(menu_view->window, 0, 0);
    if (OK != code) return result_ncurses_err();
        
    code = mvwprintw(menu_view->window, 0, 2, " %s ", menu->title);
    if (OK != code) return result_ncurses_err();
    
    return result_success();
}


static struct result
on_key(struct view *menu_view, struct app *app, int key)
{
    struct menu *menu = menu_view->user_data;
    
    if (KEY_DOWN == key) {
        int result = menu_driver(menu->menu, REQ_DOWN_ITEM);
        if (E_OK != result && E_REQUEST_DENIED != result) {
            return result_ncurses_error(result);
        }
    }
    
    if (KEY_UP == key) {
        int result = menu_driver(menu->menu, REQ_UP_ITEM);
        if (E_OK != result && E_REQUEST_DENIED != result) {
            return result_ncurses_error(result);
        }
    }
    
    if ('\r' == key) {
        app_quit(app);
        return result_success();
    }
    
    if (!isalnum(key)) return result_success();
    
    for (int i = 0; i < menu->items_count; ++i) {
        char const *name = item_name(menu->items[i]);
        if (name && key == name[0]) {
            set_current_item(menu->menu, menu->items[i]);
            return result_success();
        }
    }
    
    return result_success();
}


struct view *
menu_view_alloc(char const *title)
{
    struct menu *menu = menu_alloc(title);
    return view_alloc(create, destroy, draw, on_key, menu);
}


struct result
menu_view_add_item(struct view *menu_view,
                   char const *description)
{
    struct menu *menu = menu_view->user_data;
    return menu_add_item(menu, description);
}
