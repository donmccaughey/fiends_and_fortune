#include "game.h"

#include <ctype.h>
#include <form.h>

#include <sys/ioctl.h>

#include "character/character.h"

#include "common/alloc_or_die.h"
#include "common/dice.h"
#include "common/ptr_array.h"
#include "common/result.h"
#include "common/str.h"

#include "treasure/treasure.h"
#include "treasure/gem.h"
#include "treasure/jewelry.h"
#include "treasure/magic_item.h"
#include "treasure/treasure_map.h"
#include "treasure/treasure_type.h"

#include "selection.h"


static void
draw_character_sheet(struct game *game, int characteristics[6])
{
    int x = 2;
    int y = 1;
    mvwprintw(stdscr, y + 0, x, "Strength:     %2i\n", characteristics[0]);
    mvwprintw(stdscr, y + 1, x, "Intelligence: %2i\n", characteristics[1]);
    mvwprintw(stdscr, y + 2, x, "Wisdom:       %2i\n", characteristics[2]);
    mvwprintw(stdscr, y + 3, x, "Dexterity:    %2i\n", characteristics[3]);
    mvwprintw(stdscr, y + 4, x, "Constitution: %2i\n", characteristics[4]);
    mvwprintw(stdscr, y + 5, x, "Charisma:     %2i\n", characteristics[5]);
}


static struct result
create_character_using_simple_method(struct game *game)
{
    uint32_t special_characteristics = STRENGTH;
    int *characteristics = alloc_characteristics(game->rnd,
                                                 characteristic_generation_method_simple,
                                                 special_characteristics);
    draw_character_sheet(game, characteristics);
    free_or_die(characteristics);
    getch();
    
    return result_success();
}


static struct result
create_character_using_method_1(struct game *game)
{
    uint32_t special_characteristics = STRENGTH;
    int *characteristics = alloc_characteristics(game->rnd,
                                                 characteristic_generation_method_1,
                                                 special_characteristics);
    draw_character_sheet(game, characteristics);
    // TODO: player assigns rolls
    free_or_die(characteristics);
    getch();
    
    return result_success();
}


static struct result
create_character_using_method_2(struct game *game)
{
    uint32_t special_characteristics = STRENGTH;
    int *characteristics = alloc_characteristics(game->rnd,
                                                 characteristic_generation_method_2,
                                                 special_characteristics);
    draw_character_sheet(game, characteristics);
    // TODO: player assigns rolls
    free_or_die(characteristics);
    getch();
    
    return result_success();
}


static struct result
create_character_using_method_3(struct game *game)
{
    uint32_t special_characteristics = STRENGTH;
    int *characteristics = alloc_characteristics(game->rnd,
                                                 characteristic_generation_method_3,
                                                 special_characteristics);
    draw_character_sheet(game, characteristics);
    free_or_die(characteristics);
    getch();
    
    return result_success();
}


static struct result
create_character_using_method_4(struct game *game)
{
    uint32_t special_characteristics = STRENGTH;
    int *characteristics = alloc_characteristics(game->rnd,
                                                 characteristic_generation_method_4,
                                                 special_characteristics);
    // TODO: player chooses from 12 characters
    draw_character_sheet(game, characteristics);
    free_or_die(characteristics);
    getch();
    
    return result_success();
}


static struct result
create_character_using_method_general_NPC(struct game *game)
{
    uint32_t special_characteristics = STRENGTH;
    int *characteristics = alloc_characteristics(game->rnd,
                                                 characteristic_generation_method_general_NPC,
                                                 special_characteristics);
    draw_character_sheet(game, characteristics);
    free_or_die(characteristics);
    getch();
    
    return result_success();
}


static struct result
create_character_using_method_special_NPC(struct game *game)
{
    // TODO: choose profession and profession characteristics
    uint32_t special_characteristics = STRENGTH;
    int *characteristics = alloc_characteristics(game->rnd,
                                                 characteristic_generation_method_special_NPC,
                                                 special_characteristics);
    draw_character_sheet(game, characteristics);
    free_or_die(characteristics);
    getch();
    
    return result_success();
}


static struct result
create_character(struct game *game)
{
    struct selection *selection = selection_alloc_or_die("Create Character");
    if (!selection) return result_system_error();
    
    selection_add_item(selection, "Simple", create_character_using_simple_method);
    selection_add_item(selection, "Method 1", create_character_using_method_1);
    selection_add_item(selection, "Method 2", create_character_using_method_2);
    selection_add_item(selection, "Method 3", create_character_using_method_3);
    selection_add_item(selection, "Method 4", create_character_using_method_4);
    selection_add_item(selection, "General NPC", create_character_using_method_general_NPC);
    selection_add_item(selection, "Special NPC", create_character_using_method_special_NPC);
    selection_add_item(selection, "Back", NULL);
    
    struct result result = selection_show(selection, stdscr);
    selection_action_fn *action = selection_selected_action(selection);
    selection_free_or_die(selection);
    if (!result_is_success(result)) return result;
    
    int code = wrefresh(stdscr);
    if (ERR == code) return result_ncurses_err();
    
    if (action) {
        result = action(game);
        if (!result_is_success(result)) return result;
    }
    
    return result_success();
}


static void
enumerate_treasure_items(struct game *game, struct treasure *treasure, struct ptr_array *lines)
{
    if (treasure->gems_count) {
        ptr_array_add(lines, strdup_or_die("Gems: --------------------------------"));
        for (int i = 0; i < treasure->gems_count; ++i) {
            ptr_array_add(lines, str_alloc_formatted("%2i  %s", i + 1, treasure->gems[i].true_description));
        }
    }
    
    if (treasure->jewelry_count) {
        ptr_array_add(lines, strdup_or_die("Jewelry: -----------------------------"));
        for (int i = 0; i < treasure->jewelry_count; ++i) {
            ptr_array_add(lines, str_alloc_formatted("%2i  %s", i + 1, treasure->jewelry[i].true_description));
        }
    }
    
    if (treasure->maps_count) {
        ptr_array_add(lines, strdup_or_die("Maps: --------------------------------"));
        for (int i = 0; i < treasure->maps_count; ++i) {
            ptr_array_add(lines, str_alloc_formatted("%2i  %s", i + 1, treasure->maps[i].true_description));
        }
    }
    
    if (treasure->magic_items_count) {
        ptr_array_add(lines, strdup_or_die("Magic Items: -------------------------"));
        for (int i = 0; i < treasure->magic_items_count; ++i) {
            ptr_array_add(lines, str_alloc_formatted("%2i  %s", i + 1, treasure->magic_items[i].true_description));
            if (treasure->magic_items[i].true_details) {
                int j = 0;
                while (treasure->magic_items[i].true_details[j]) {
                    ptr_array_add(lines, str_alloc_formatted("        %s", treasure->magic_items[i].true_details[j]));
                    ++j;
                }
            }
        }
    }
}


static struct result
generate_dungeon(struct game *game)
{
    mvprintw(1, 2, "Generate Dungeon");
    getch();
    return result_success();
}


static struct result
generate_treasure_type(struct game *game, char letter)
{
    struct ptr_array *lines = ptr_array_alloc();
    
    ptr_array_add(lines, str_alloc_formatted("Treasure type %c", letter));
    
    int individual_count = 0;
    if (letter >= 'J' && letter <= 'N') {
        individual_count = roll("1d10", game->rnd);
        ptr_array_add(lines, str_alloc_formatted("    %i individuals", individual_count));
    }
    ptr_array_add(lines, strdup_or_die(""));
    
    struct treasure treasure;
    treasure_initialize(&treasure);
    treasure_type_generate(treasure_type_by_letter(letter), game->rnd, individual_count, &treasure);
    
    char *description = treasure_alloc_description(&treasure);
    int value_cp = treasure_value_in_cp(&treasure);
    char *value_gp = coins_alloc_gp_cp_description(value_cp);
    ptr_array_add(lines, str_alloc_formatted("%s (total %s)", description, value_gp));
    free_or_die(value_gp);
    free_or_die(description);
    
    enumerate_treasure_items(game, &treasure, lines);
    
    treasure_finalize(&treasure);
    
    int pad_width = 0;
    int pad_height = lines->count;
    for (int i = 0; i < lines->count; ++i) {
        int length = (int)strlen(lines->elements[i]);
        if (length > pad_width) pad_width = length;
    }
    
    int code = ERR;
    
    WINDOW *pad = newpad(pad_height, pad_width);
    if ( ! pad) return result_ncurses_err();
        
        int x = 0;
        int y = 0;
        for (int i = 0; i < lines->count; ++i) {
            code = mvwprintw(pad, y, x, "%s", lines->elements[i]);
            if (ERR == code) {
                mvwprintw(stdscr, y - 1, 0, "*");
            }
            ++y;
        }
    
    ptr_array_clear(lines, free_or_die);
    ptr_array_free(lines);
    
    curs_set(0);
    
    code = werase(stdscr);
    if (ERR == code) return result_ncurses_err();
    
    code = box(stdscr, 0, 0);
    if (ERR == code) return result_ncurses_err();
    
    code = wnoutrefresh(stdscr);
    if (ERR == code) return result_ncurses_err();
    
    int y_offset = 0;
    getmaxyx(stdscr, y, x);
    code = pnoutrefresh(pad, y_offset, 0, 1, 2, y - 2, x - 4);
    if (ERR == code) return result_ncurses_err();
    
    code = doupdate();
    if (ERR == code) return result_ncurses_err();
        
    code = keypad(pad, TRUE);
    if (ERR == code) return result_ncurses_err();
    
    set_escdelay(25);
    
    int ch;
    while (true) {
        ch = wgetch(pad);
        if ('q' == ch || 27 == ch) {
            break;
        }
        
        int x, y;
        getmaxyx(stdscr, y, x);
        int hidden_line_count = pad_height - (y - 2);
        
        if ('k' == ch || KEY_UP == ch) {
            if (hidden_line_count > 0 && y_offset > 0) {
                --y_offset;
                code = prefresh(pad, y_offset, 0, 1, 2, y - 2, x - 4);
                if (ERR == code) return result_ncurses_err();
            }
        }
        if ('j' == ch || KEY_DOWN == ch) {
            if (hidden_line_count > 0 && y_offset < hidden_line_count) {
                ++y_offset;
                code = prefresh(pad, y_offset, 0, 1, 2, y - 2, x - 4);
                if (ERR == code) return result_ncurses_err();
            }
        }
    }
    
    code = keypad(stdscr, TRUE);
    if (ERR == code) return result_ncurses_err();
    
    code = delwin(pad);
    if (E_OK != code) return result_ncurses_error(code);
    
    curs_set(1);
    
    return result_success();
}


static struct result
generate_treasure(struct game *game)
{
    mvprintw(1, 2, "Generate Treasure");
    
    int height = 1;
    int width = 1;
    int x = 18;
    int y = 3;
    int offscreen_rows = 0;
    int buffer_count = 0;
    FIELD *treasure_type = new_field(height, width, y, x, offscreen_rows, buffer_count);
    if (!treasure_type) return result_ncurses_errno();
        
    int code = set_field_back(treasure_type, A_UNDERLINE);
    if (E_OK != code) return result_ncurses_error(code);
    
    height = 1;
    width = 1;
    x = 20;
    y = 3;
    offscreen_rows = 0;
    buffer_count = 0;
    FIELD *done = new_field(height, width, y, x, offscreen_rows, buffer_count);
    if (!done) return result_ncurses_errno();
    
    FIELD *fields[] = {
        treasure_type, done, NULL
    };

    FORM *form = new_form(fields);
    if (!form) return result_ncurses_errno();
    
    code = post_form(form);
    if (E_OK != code) return result_ncurses_error(code);
    
    code = refresh();
    if (ERR == code) return result_ncurses_err();
    
    mvprintw(3, 2, "Treasure Type:");
        
    code = refresh();
    if (ERR == code) return result_ncurses_err();
    
    int ch;
    while ('\r' != (ch = getch())) {
        form_driver(form, ch);
    }
    
    char *buffer = field_buffer(treasure_type, 0);
    if (!buffer) return result_ncurses_errno();
    
    char letter = toupper(buffer[0]);
    if (letter >= 'A' && letter <= 'Z') {
        struct result result = generate_treasure_type(game, letter);
        if (!result_is_success(result)) return result;
    }
    
    code = unpost_form(form);
    if (E_OK != code) return result_ncurses_error(code);

    code = free_form(form);
    if (E_OK != code) return result_ncurses_error(code);
    
    code = free_field(treasure_type);
    if (E_OK != code) return result_ncurses_error(code);
    
    return result_success();
}


static struct result
quit_game(struct game *game)
{
    game->is_running = false;
    return result_success();
}


static void
resize_window(int signal)
{
    struct winsize winsize;
    ioctl(0, TIOCGWINSZ, &winsize);
    resizeterm(winsize.ws_row, winsize.ws_col);
}


struct game *
game_alloc_or_die(struct rnd *rnd)
{
    struct game *game = calloc_or_die(1, sizeof(struct game));
    game->rnd = rnd;
    return game;
}


void
game_free_or_die(struct game *game)
{
    free_or_die(game);
}


void
game_hide(struct game *game)
{
    if (!game) return;
    endwin();
    signal(SIGWINCH, game->previous_signal_handler);
}


struct result
game_run(struct game *game)
{
    game->is_running = true;
    
    struct selection *selection = selection_alloc_or_die("Fiends & Fortune");
    
    selection_add_item(selection, "Create Character", create_character);
    selection_add_item(selection, "Generate Treasure", generate_treasure);
    selection_add_item(selection, "Generate Dungeon", generate_dungeon);
    selection_add_item(selection, "Quit", quit_game);
    
    struct result result = result_success();
    while (game->is_running) {
        result = selection_show(selection, stdscr);
        if (!result_is_success(result)) break;
        
        int code = wrefresh(stdscr);
        if (ERR == code) {
            result = result_ncurses_err();
            break;
        }

        selection_action_fn *action = selection_selected_action(selection);
        if (action) {
            result = action(game);
            if (!result_is_success(result)) break;
        }
        
        code = werase(stdscr);
        if (ERR == code) {
            result = result_ncurses_err();
            break;
        }
        
        code = wrefresh(stdscr);
        if (ERR == code) {
            result = result_ncurses_err();
            break;
        }
    }
    
    selection_free_or_die(selection);
    return result;
}


struct result
game_show(struct game *game)
{
    game->previous_signal_handler = signal(SIGWINCH, resize_window);
    if (SIG_ERR == game->previous_signal_handler) return result_system_error();
    
    WINDOW *window = initscr();
    if (!window) return result_ncurses_err();

    int code = cbreak();
    if (ERR == code) return result_ncurses_err();

    code = noecho();
    if (ERR == code) return result_ncurses_err();

    nonl();

    code = intrflush(stdscr, FALSE);
    if (ERR == code) return result_ncurses_err();

    code = keypad(stdscr, TRUE);
    if (ERR == code) return result_ncurses_err();

    return result_success();
}
