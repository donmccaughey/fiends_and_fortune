#include "selection.h"

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <base/base.h>


static struct result
draw_window(struct selection *selection)
{
    int result = wborder(selection->window, '|', '|', '-', '-', '+', '+', '+', '+');
    if (OK != result) return result_ncurses_err();
        
    // TODO: handle UTF-8 correctly
    size_t title_string_length = strlen(selection->title);
    if (title_string_length > (size_t)INT_MAX) {
        return result_set_system_error(ERANGE);
    }
    
    if (title_string_length) {
        int title_string_width = (int)title_string_length;
        int window_height;
        int window_width;
        getmaxyx(selection->window, window_height, window_width);
        
        char const left_frame[] = "- ";
        char const right_frame[] = " -";
        
        int const left_frame_width = sizeof(left_frame) - 1;
        int const right_frame_width = sizeof(right_frame) - 1;
        
        int const left_border_width = 1;
        int const right_border_width = 1;
        
        int window_top_width = window_width - left_border_width - right_border_width;
        int title_width = left_frame_width + title_string_width + right_frame_width;
        if (window_top_width >= title_width) {
            int start = left_border_width + (window_top_width - title_width) / 2;
            result = mvwprintw(selection->window, 0, start, "%s%s%s",
                               left_frame, selection->title, right_frame);
            if (ERR == result) return result_ncurses_err();
        } else {
            // TODO: elide title
        }
    }
    
    result = wrefresh(selection->window);
    if (ERR == result) return result_ncurses_err();
    
    return result_success();
}


static struct result
get_selection(struct selection *selection)
{
    while (true) {
        int ch = wgetch(selection->window);
        if (ERR == ch) {
            return result_ncurses_err();
        }
        
        if (KEY_DOWN == ch) {
            int result = menu_driver(selection->menu, REQ_DOWN_ITEM);
            if (E_OK != result && E_REQUEST_DENIED != result) {
                return result_ncurses_error(result);
            }
        }
        
        if (KEY_UP == ch) {
            int result = menu_driver(selection->menu, REQ_UP_ITEM);
            if (E_OK != result && E_REQUEST_DENIED != result) {
                return result_ncurses_error(result);
            }
        }
        
        if ('\r' == ch) break;
        
        if (isdigit(ch)) {
            for (int i = 0; i < selection->items_count; ++i) {
                char const *name = item_name(selection->items[i]);
                if (name && ch == name[0]) {
                    set_current_item(selection->menu, selection->items[i]);
                    return result_success();
                }
            }
        }
        
        if (isalpha(ch)) {
            ch = tolower(ch);
            for (int i = 0; i < selection->items_count; ++i) {
                struct selection_item *selection_item = item_userptr(selection->items[i]);
                if (ch == selection_item->shortcut_key) {
                    set_current_item(selection->menu, selection->items[i]);
                    return result_success();
                }
            }
        }
    }
    
    return result_success();
}


struct result
selection_add_item(struct selection *selection,
                   char const *description,
                   selection_action_fn *action)
{
    if (!selection) return result_set_system_error(EINVAL);
    if (!description) return result_set_system_error(EINVAL);
    if (!description[0]) return result_set_system_error(EINVAL);
    
    int index = selection->items_count;
    int null_index = index + 1;
    ++selection->items_count;
    selection->items = reallocarray_or_die(selection->items,
                                           selection->items_count + 1,
                                           sizeof(ITEM *));
    
    char *name = str_alloc_formatted("%i", index + 1);
    
    char *description_dup = strdup_or_die(description);
    char *shortcut_start = strchr(description_dup, '&');
    int shortcut_key = 0;
    int shortcut_index = -1;
    if (shortcut_start && '&' != shortcut_start[1]) {
        char *end_of_description = &shortcut_start[1];
        shortcut_key = tolower(end_of_description[0]);
        shortcut_index = (int)(end_of_description - description_dup);
        memmove(shortcut_start, end_of_description, strlen(end_of_description) + 1);
    }
    
    selection->items[index] = new_item(name, description_dup);
    if (!selection->items[index]) {
        free_or_die(description_dup);
        free_or_die(name);
        return result_system_error();
    }
    
    struct selection_item *selection_item = calloc_or_die(1, sizeof(struct selection_item));
    selection_item->action = action;
    selection_item->shortcut_key = shortcut_key;
    selection_item->shortcut_index = shortcut_index;
    set_item_userptr(selection->items[index], selection_item);
    
    selection->items[null_index] = NULL;
    
    return result_success();
}


struct selection *
selection_alloc_or_die(char const *title)
{
    assert(title && title[0]);
    
    struct selection *selection = calloc_or_die(1, sizeof(struct selection));
    selection->index = -1;
    selection->items = calloc_or_die(1, sizeof(ITEM *));
    selection->title = strdup_or_die(title);
    return selection;
}


void
selection_free_or_die(struct selection *selection)
{
    if (!selection) return;
    
    if (selection->menu) {
        unpost_menu(selection->menu);
        free_menu(selection->menu);
    }
    if (selection->sub_window) delwin(selection->sub_window);
    if (selection->window) delwin(selection->window);
    
    for (int i = 0; i < selection->items_count; ++i) {
        free_or_die((char *)item_name(selection->items[i]));
        free_or_die((char *)item_description(selection->items[i]));
        free_or_die(item_userptr(selection->items[i]));
        free_item(selection->items[i]);
    }
    free_or_die(selection->items);
    free_or_die(selection->title);
    free_or_die(selection);
}


selection_action_fn *
selection_selected_action(struct selection *selection)
{
    if (selection->index < 0) return NULL;
    if (selection->index >= selection->items_count) return NULL;
    
    ITEM *item = selection->items[selection->index];
    struct selection_item *selection_item = item_userptr(item);
    return selection_item->action;
}


struct result
selection_show(struct selection *selection, WINDOW *parent)
{
    if (!selection) return result_set_system_error(EINVAL);
    
    selection->menu = new_menu(selection->items);
    if (!selection->menu) return result_ncurses_errno();
    
    int menu_height;
    int menu_width;
    int code = scale_menu(selection->menu, &menu_height, &menu_width);
    if (E_OK != code) return result_ncurses_error(code);
    
    int title_width = (int)strlen(selection->title) + 1;
    if (title_width > menu_width) menu_width = title_width;
    
    int main_window_height;
    int main_window_width;
    getmaxyx(parent, main_window_height, main_window_width);
    
    int menu_window_height = menu_height + 4;
    int menu_window_width = menu_width + 5;
    int menu_window_y = (main_window_height - menu_window_height) / 2;
    int menu_window_x = (main_window_width - menu_window_width) / 2;
    
    selection->window = newwin(menu_window_height, menu_window_width,
                               menu_window_y, menu_window_x);
    if (!selection->window) return result_ncurses_err();
    
    code = keypad(selection->window, TRUE);
    if (ERR == code) return result_ncurses_err();
        
    struct result result = draw_window(selection);
    if (!result_is_success(result)) return result;
    
    code = set_menu_win(selection->menu, selection->window);
    if (E_OK != code) return result_ncurses_error(code);
    
    int menu_sub_height = menu_height;
    int menu_sub_width = menu_width;
    int menu_sub_y = 2;
    int menu_sub_x = 2;
    
    selection->sub_window = derwin(selection->window,
                                   menu_sub_height, menu_sub_width,
                                   menu_sub_y, menu_sub_x);
    if (!selection->sub_window) return result_ncurses_err();
    
    code = set_menu_sub(selection->menu, selection->sub_window);
    if (E_OK != code) return result_ncurses_error(code);
    
    selection->index = 0;
    code = post_menu(selection->menu);
    if (E_OK != code) return result_ncurses_error(code);
    
    code = wrefresh(selection->window);
    if (ERR == code) return result_ncurses_err();
        
    result = get_selection(selection);
    if (!result_is_success(result)) return result;
    
    ITEM *selected_item = current_item(selection->menu);
    if (selected_item) selection->index = item_index(selected_item);
    
    code = unpost_menu(selection->menu);
    if (E_OK != code) return result_ncurses_error(code);
    
    code = wclear(selection->window);
    if (ERR == code) return result_ncurses_err();
        
    code = wrefresh(selection->window);
    if (ERR == code) return result_ncurses_err();
    
    code = free_menu(selection->menu);
    if (E_OK != code) return result_ncurses_error(code);
    selection->menu = NULL;
    
    code = delwin(selection->sub_window);
    if (E_OK != code) return result_ncurses_error(code);
    selection->sub_window = NULL;
        
    code = delwin(selection->window);
    if (E_OK != code) return result_ncurses_error(code);
    selection->window = NULL;
    
    return result_success();
}
