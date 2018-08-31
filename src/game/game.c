#include "game.h"

#include <assert.h>
#include <ctype.h>
#include <form.h>
#include <limits.h>
#include <time.h>
#include <base/base.h>
#include <character/character.h>
#include <mechanics/mechanics.h>
#include <sys/ioctl.h>

#include "dungeon/area.h"
#include "dungeon/dungeon.h"
#include "dungeon/dungeon_options.h"
#include "dungeon/generator.h"
#include "dungeon/level_map.h"
#include "dungeon/size.h"
#include "dungeon/text_rectangle.h"

#include "treasure/treasure.h"
#include "treasure/gem.h"
#include "treasure/jewelry.h"
#include "treasure/magic_item.h"
#include "treasure/treasure_map.h"
#include "treasure/treasure_type.h"

#include "selection.h"


static void
draw_character_sheet(struct game *game,
                     enum characteristic_generation_method method,
                     struct characteristic_rolls *rolls)
{
    int x = 2;
    int y = 1;

    char const *method_name = characteristic_generation_method_name(method);
    mvwprintw(stdscr, y + 0, x, "(%s)", method_name);

    switch (rolls->type) {
        case characteristic_rolls_type_none:
            break;
        case characteristic_rolls_type_six_characteristics:
            mvwprintw(stdscr, y + 2, x, "Strength:     %2i\n", rolls->characteristics.strength);
            mvwprintw(stdscr, y + 3, x, "Intelligence: %2i\n", rolls->characteristics.intelligence);
            mvwprintw(stdscr, y + 4, x, "Wisdom:       %2i\n", rolls->characteristics.wisdom);
            mvwprintw(stdscr, y + 5, x, "Dexterity:    %2i\n", rolls->characteristics.dexterity);
            mvwprintw(stdscr, y + 6, x, "Constitution: %2i\n", rolls->characteristics.constitution);
            mvwprintw(stdscr, y + 7, x, "Charisma:     %2i\n", rolls->characteristics.charisma);
            break;
        case characteristic_rolls_type_six_scores:
            mvwprintw(stdscr, y + 2, x, "%2i, %2i, %2i, %2i, %2i, %2i\n",
                      rolls->scores[0], rolls->scores[1], rolls->scores[2],
                      rolls->scores[3], rolls->scores[4], rolls->scores[5]);
            break;
        case characteristic_rolls_type_twelve_sets_of_six_characteristics:
            mvwprintw(stdscr, y + 2, x, "Strength:     %2i\n", rolls->characteristics_sets[0].strength);
            mvwprintw(stdscr, y + 3, x, "Intelligence: %2i\n", rolls->characteristics_sets[0].intelligence);
            mvwprintw(stdscr, y + 4, x, "Wisdom:       %2i\n", rolls->characteristics_sets[0].wisdom);
            mvwprintw(stdscr, y + 5, x, "Dexterity:    %2i\n", rolls->characteristics_sets[0].dexterity);
            mvwprintw(stdscr, y + 6, x, "Constitution: %2i\n", rolls->characteristics_sets[0].constitution);
            mvwprintw(stdscr, y + 7, x, "Charisma:     %2i\n", rolls->characteristics_sets[0].charisma);

            mvwprintw(stdscr, y + 2, x + 20, "Strength:     %2i\n", rolls->characteristics_sets[1].strength);
            mvwprintw(stdscr, y + 3, x + 20, "Intelligence: %2i\n", rolls->characteristics_sets[1].intelligence);
            mvwprintw(stdscr, y + 4, x + 20, "Wisdom:       %2i\n", rolls->characteristics_sets[1].wisdom);
            mvwprintw(stdscr, y + 5, x + 20, "Dexterity:    %2i\n", rolls->characteristics_sets[1].dexterity);
            mvwprintw(stdscr, y + 6, x + 20, "Constitution: %2i\n", rolls->characteristics_sets[1].constitution);
            mvwprintw(stdscr, y + 7, x + 20, "Charisma:     %2i\n", rolls->characteristics_sets[1].charisma);

            mvwprintw(stdscr, y + 2, x + 40, "Strength:     %2i\n", rolls->characteristics_sets[2].strength);
            mvwprintw(stdscr, y + 3, x + 40, "Intelligence: %2i\n", rolls->characteristics_sets[2].intelligence);
            mvwprintw(stdscr, y + 4, x + 40, "Wisdom:       %2i\n", rolls->characteristics_sets[2].wisdom);
            mvwprintw(stdscr, y + 5, x + 40, "Dexterity:    %2i\n", rolls->characteristics_sets[2].dexterity);
            mvwprintw(stdscr, y + 6, x + 40, "Constitution: %2i\n", rolls->characteristics_sets[2].constitution);
            mvwprintw(stdscr, y + 7, x + 40, "Charisma:     %2i\n", rolls->characteristics_sets[2].charisma);

            mvwprintw(stdscr, y + 2, x + 60, "Strength:     %2i\n", rolls->characteristics_sets[3].strength);
            mvwprintw(stdscr, y + 3, x + 60, "Intelligence: %2i\n", rolls->characteristics_sets[3].intelligence);
            mvwprintw(stdscr, y + 4, x + 60, "Wisdom:       %2i\n", rolls->characteristics_sets[3].wisdom);
            mvwprintw(stdscr, y + 5, x + 60, "Dexterity:    %2i\n", rolls->characteristics_sets[3].dexterity);
            mvwprintw(stdscr, y + 6, x + 60, "Constitution: %2i\n", rolls->characteristics_sets[3].constitution);
            mvwprintw(stdscr, y + 7, x + 60, "Charisma:     %2i\n", rolls->characteristics_sets[3].charisma);


            mvwprintw(stdscr, y +  9, x, "Strength:     %2i\n", rolls->characteristics_sets[4].strength);
            mvwprintw(stdscr, y + 10, x, "Intelligence: %2i\n", rolls->characteristics_sets[4].intelligence);
            mvwprintw(stdscr, y + 11, x, "Wisdom:       %2i\n", rolls->characteristics_sets[4].wisdom);
            mvwprintw(stdscr, y + 12, x, "Dexterity:    %2i\n", rolls->characteristics_sets[4].dexterity);
            mvwprintw(stdscr, y + 13, x, "Constitution: %2i\n", rolls->characteristics_sets[4].constitution);
            mvwprintw(stdscr, y + 14, x, "Charisma:     %2i\n", rolls->characteristics_sets[4].charisma);

            mvwprintw(stdscr, y +  9, x + 20, "Strength:     %2i\n", rolls->characteristics_sets[5].strength);
            mvwprintw(stdscr, y + 10, x + 20, "Intelligence: %2i\n", rolls->characteristics_sets[5].intelligence);
            mvwprintw(stdscr, y + 11, x + 20, "Wisdom:       %2i\n", rolls->characteristics_sets[5].wisdom);
            mvwprintw(stdscr, y + 12, x + 20, "Dexterity:    %2i\n", rolls->characteristics_sets[5].dexterity);
            mvwprintw(stdscr, y + 13, x + 20, "Constitution: %2i\n", rolls->characteristics_sets[5].constitution);
            mvwprintw(stdscr, y + 14, x + 20, "Charisma:     %2i\n", rolls->characteristics_sets[5].charisma);

            mvwprintw(stdscr, y +  9, x + 40, "Strength:     %2i\n", rolls->characteristics_sets[6].strength);
            mvwprintw(stdscr, y + 10, x + 40, "Intelligence: %2i\n", rolls->characteristics_sets[6].intelligence);
            mvwprintw(stdscr, y + 11, x + 40, "Wisdom:       %2i\n", rolls->characteristics_sets[6].wisdom);
            mvwprintw(stdscr, y + 12, x + 40, "Dexterity:    %2i\n", rolls->characteristics_sets[6].dexterity);
            mvwprintw(stdscr, y + 13, x + 40, "Constitution: %2i\n", rolls->characteristics_sets[6].constitution);
            mvwprintw(stdscr, y + 14, x + 40, "Charisma:     %2i\n", rolls->characteristics_sets[6].charisma);

            mvwprintw(stdscr, y +  9, x + 60, "Strength:     %2i\n", rolls->characteristics_sets[7].strength);
            mvwprintw(stdscr, y + 10, x + 60, "Intelligence: %2i\n", rolls->characteristics_sets[7].intelligence);
            mvwprintw(stdscr, y + 11, x + 60, "Wisdom:       %2i\n", rolls->characteristics_sets[7].wisdom);
            mvwprintw(stdscr, y + 12, x + 60, "Dexterity:    %2i\n", rolls->characteristics_sets[7].dexterity);
            mvwprintw(stdscr, y + 13, x + 60, "Constitution: %2i\n", rolls->characteristics_sets[7].constitution);
            mvwprintw(stdscr, y + 14, x + 60, "Charisma:     %2i\n", rolls->characteristics_sets[7].charisma);


            mvwprintw(stdscr, y + 16, x, "Strength:     %2i\n", rolls->characteristics_sets[8].strength);
            mvwprintw(stdscr, y + 17, x, "Intelligence: %2i\n", rolls->characteristics_sets[8].intelligence);
            mvwprintw(stdscr, y + 18, x, "Wisdom:       %2i\n", rolls->characteristics_sets[8].wisdom);
            mvwprintw(stdscr, y + 19, x, "Dexterity:    %2i\n", rolls->characteristics_sets[8].dexterity);
            mvwprintw(stdscr, y + 20, x, "Constitution: %2i\n", rolls->characteristics_sets[8].constitution);
            mvwprintw(stdscr, y + 21, x, "Charisma:     %2i\n", rolls->characteristics_sets[8].charisma);

            mvwprintw(stdscr, y + 16, x + 20, "Strength:     %2i\n", rolls->characteristics_sets[9].strength);
            mvwprintw(stdscr, y + 17, x + 20, "Intelligence: %2i\n", rolls->characteristics_sets[9].intelligence);
            mvwprintw(stdscr, y + 18, x + 20, "Wisdom:       %2i\n", rolls->characteristics_sets[9].wisdom);
            mvwprintw(stdscr, y + 19, x + 20, "Dexterity:    %2i\n", rolls->characteristics_sets[9].dexterity);
            mvwprintw(stdscr, y + 20, x + 20, "Constitution: %2i\n", rolls->characteristics_sets[9].constitution);
            mvwprintw(stdscr, y + 21, x + 20, "Charisma:     %2i\n", rolls->characteristics_sets[9].charisma);

            mvwprintw(stdscr, y + 16, x + 40, "Strength:     %2i\n", rolls->characteristics_sets[10].strength);
            mvwprintw(stdscr, y + 17, x + 40, "Intelligence: %2i\n", rolls->characteristics_sets[10].intelligence);
            mvwprintw(stdscr, y + 18, x + 40, "Wisdom:       %2i\n", rolls->characteristics_sets[10].wisdom);
            mvwprintw(stdscr, y + 19, x + 40, "Dexterity:    %2i\n", rolls->characteristics_sets[10].dexterity);
            mvwprintw(stdscr, y + 20, x + 40, "Constitution: %2i\n", rolls->characteristics_sets[10].constitution);
            mvwprintw(stdscr, y + 21, x + 40, "Charisma:     %2i\n", rolls->characteristics_sets[10].charisma);

            mvwprintw(stdscr, y + 16, x + 60, "Strength:     %2i\n", rolls->characteristics_sets[11].strength);
            mvwprintw(stdscr, y + 17, x + 60, "Intelligence: %2i\n", rolls->characteristics_sets[11].intelligence);
            mvwprintw(stdscr, y + 18, x + 60, "Wisdom:       %2i\n", rolls->characteristics_sets[11].wisdom);
            mvwprintw(stdscr, y + 19, x + 60, "Dexterity:    %2i\n", rolls->characteristics_sets[11].dexterity);
            mvwprintw(stdscr, y + 20, x + 60, "Constitution: %2i\n", rolls->characteristics_sets[11].constitution);
            mvwprintw(stdscr, y + 21, x + 60, "Charisma:     %2i\n", rolls->characteristics_sets[11].charisma);
            break;
    }

}


static struct result
create_character_using_simple_method(struct game *game)
{
    struct characteristic_rolls *rolls = characteristic_rolls_alloc(game->rnd,
                                                                    characteristic_generation_method_simple,
                                                                    characteristic_flag_none);
    draw_character_sheet(game, characteristic_generation_method_simple, rolls);
    characteristic_rolls_free(rolls);
    getch();
    
    return result_success();
}


static struct result
create_character_using_method_1(struct game *game)
{
    struct characteristic_rolls *rolls = characteristic_rolls_alloc(game->rnd,
                                                                    characteristic_generation_method_1,
                                                                    characteristic_flag_none);
    draw_character_sheet(game, characteristic_generation_method_1, rolls);
    // TODO: player assigns rolls
    characteristic_rolls_free(rolls);
    getch();
    
    return result_success();
}


static struct result
create_character_using_method_2(struct game *game)
{
    struct characteristic_rolls *rolls = characteristic_rolls_alloc(game->rnd,
                                                                    characteristic_generation_method_2,
                                                                    characteristic_flag_none);
    draw_character_sheet(game, characteristic_generation_method_2, rolls);
    // TODO: player assigns rolls
    characteristic_rolls_free(rolls);
    getch();
    
    return result_success();
}


static struct result
create_character_using_method_3(struct game *game)
{
    struct characteristic_rolls *rolls = characteristic_rolls_alloc(game->rnd,
                                                                    characteristic_generation_method_3,
                                                                    characteristic_flag_none);
    draw_character_sheet(game, characteristic_generation_method_3, rolls);
    characteristic_rolls_free(rolls);
    getch();
    
    return result_success();
}


static struct result
create_character_using_method_4(struct game *game)
{
    struct characteristic_rolls *rolls = characteristic_rolls_alloc(game->rnd,
                                                                    characteristic_generation_method_4,
                                                                    characteristic_flag_none);
    // TODO: player chooses from 12 characters
    draw_character_sheet(game, characteristic_generation_method_4, rolls);
    characteristic_rolls_free(rolls);
    getch();
    
    return result_success();
}


static struct result
create_character_using_method_general_NPC(struct game *game)
{
    struct characteristic_rolls *rolls = characteristic_rolls_alloc(game->rnd,
                                                                    characteristic_generation_method_general_NPC,
                                                                    characteristic_flag_none);
    draw_character_sheet(game, characteristic_generation_method_general_NPC, rolls);
    characteristic_rolls_free(rolls);
    getch();
    
    return result_success();
}


static struct result
create_character_using_method_special_NPC(struct game *game)
{
    // TODO: choose profession and profession characteristics
    struct characteristic_rolls *rolls = characteristic_rolls_alloc(game->rnd,
                                                                    characteristic_generation_method_special_NPC,
                                                                    characteristic_flag_none);
    draw_character_sheet(game, characteristic_generation_method_special_NPC, rolls);
    characteristic_rolls_free(rolls);
    getch();
    
    return result_success();
}


static struct result
create_character(struct game *game)
{
    struct selection *selection = selection_alloc_or_die("Create Character");
    if (!selection) return result_system_error();
    
    selection_add_item(selection, "Method 1", create_character_using_method_1);
    selection_add_item(selection, "Method 2", create_character_using_method_2);
    selection_add_item(selection, "Method 3", create_character_using_method_3);
    selection_add_item(selection, "Method 4", create_character_using_method_4);
    selection_add_item(selection, "&Simple", create_character_using_simple_method);
    selection_add_item(selection, "&General NPC", create_character_using_method_general_NPC);
    selection_add_item(selection, "Special &NPC", create_character_using_method_special_NPC);
    selection_add_item(selection, "&Back", NULL);
    
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


static WINDOW *
pad_for_dungeon_level(struct dungeon *dungeon, int level)
{
    struct level_map *level_map = level_map_alloc(dungeon, level);
    struct text_rectangle *text_rectangle = level_map_alloc_text_graph(level_map);
    
    int pad_width = text_rectangle->column_count;
    int pad_height = text_rectangle->row_count;
    
    WINDOW *pad = newpad(pad_height, pad_width);
    if ( ! pad) {
        text_rectangle_free(text_rectangle);
        level_map_free(level_map);
        return NULL;
    }
    
    int x = 0;
    int y = 0;
    for (int i = 0; i < pad_height; ++i) {
        char *row = text_rectangle_row_at(text_rectangle, i);
        int code = mvwprintw(pad, y, x, "%*s", pad_width, row);
        if (ERR == code) { /* ignore */ }
        ++y;
    }
    
    text_rectangle_free(text_rectangle);
    level_map_free(level_map);
    
    return pad;
}


static struct result
draw_scroll_window(WINDOW *window, char const *title)
{
    int code = ERR;
    
    code = werase(window);
    if (ERR == code) return result_ncurses_err();
    
    code = wborder(window, '|', '|', '-', '-', '+', '+', '+', '+');
    if (ERR == code) return result_ncurses_err();
    
    if (title) {
        int width, height;
        getmaxyx(window, height, width);
        size_t title_length = strlen(title);
        assert(title_length < (size_t)INT_MAX);
        if (title_length) {
            int title_width = (int)title_length;
            if (title_width <= width - 6) {
                int x = (width - title_width) / 2 - 1;
                mvwprintw(window, 0, x, " %s ", title);
            } else {
                int x = 2;
                mvwprintw(window, 0, x, " %*s ", width = 6, title);
            }
        }
    }
    
    code = wnoutrefresh(window);
    if (ERR == code) return result_ncurses_err();
    
    return result_success();
}


static struct result
draw_scroll_window_for_level(WINDOW *window, int level)
{
    char *title = str_alloc_formatted("Level %i", level);
    struct result result = draw_scroll_window(window, title);
    free_or_die(title);
    return result;
}


static struct result
draw_dungeon_level(struct dungeon *dungeon,
                   int level,
                   WINDOW *window,
                   WINDOW **pad,
                   int *x_offset,
                   int *y_offset)
{
    int code = ERR;
    
    struct result result = draw_scroll_window_for_level(window, level);
    if (!result_is_success(result)) {
        return result_ncurses_err();
    }
    
    if (*pad) {
        code = delwin(*pad);
        if (E_OK != code) {
            return result_ncurses_error(code);
        }
    }
    
    *pad = pad_for_dungeon_level(dungeon, level);
    if ( ! *pad) {
        return result_ncurses_err();
    }
    
    *x_offset = 0;
    *y_offset = 0;
    int width, height;
    getmaxyx(window, height, width);
    code = pnoutrefresh(*pad, *y_offset, *x_offset, 1, 2, height - 2, width - 4);
    if (ERR == code) return result_ncurses_err();
    
    code = doupdate();
    if (ERR == code) return result_ncurses_err();
    
    code = keypad(*pad, TRUE);
    if (ERR == code) return result_ncurses_err();
    
    return result_success();
}


static struct result
list_dungeon_level_areas(struct dungeon *dungeon,
                         int level,
                         WINDOW *window,
                         WINDOW **pad,
                         int *x_offset,
                         int *y_offset)
{
    int code = ERR;
    
    struct result result = draw_scroll_window_for_level(window, level);
    if (!result_is_success(result)) {
        return result_ncurses_err();
    }
    
    if (*pad) {
        code = delwin(*pad);
        if (E_OK != code) {
            return result_ncurses_error(code);
        }
    }
    
    struct ptr_array *lines = ptr_array_alloc();
    ptr_array_add(lines, strdup_or_die("Entrances and Exits:"));
    for (int i = 0; i < dungeon->areas_count; ++i) {
        struct area *area = dungeon->areas[i];
        if (level != area->box.origin.z) continue;
        if (area_is_level_transition(area)) {
            char *location = point_alloc_xy(area_center_point(area));
            char *description = area_alloc_description(area);
            ptr_array_add(lines, str_alloc_formatted("  %-12s %s", location, description));
            free_or_die(location);
            free_or_die(description);
        }
    }
    
    ptr_array_add(lines, strdup_or_die(""));
    ptr_array_add(lines, strdup_or_die("Chambers and Rooms:"));
    
    for (int i = 0; i < dungeon->areas_count; ++i) {
        struct area *area = dungeon->areas[i];
        if (level != area->box.origin.z) continue;
        if (area_is_chamber_or_room(area)) {
            char *location = point_alloc_xy(area_center_point(area));
            char *description = area_alloc_description(area);
            ptr_array_add(lines, str_alloc_formatted("  %-12s %s", location, description));
            free_or_die(location);
            free_or_die(description);
        }
    }
    
    int pad_width = 0;
    int pad_height = lines->count;
    for (int i = 0; i < lines->count; ++i) {
        int length = (int)strlen(lines->elements[i]);
        if (length > pad_width) pad_width = length;
    }
    
    *pad = newpad(pad_height, pad_width);
    if ( ! *pad) return result_ncurses_err();
    
    int x = 0;
    int y = 0;
    for (int i = 0; i < lines->count; ++i) {
        code = mvwprintw(*pad, y, x, "%s", lines->elements[i]);
        if (ERR == code) {
            mvwprintw(*pad, y, 0, "*");
        }
        ++y;
    }
    
    ptr_array_clear(lines, free_or_die);
    ptr_array_free(lines);
    
    *x_offset = 0;
    *y_offset = 0;
    int width, height;
    getmaxyx(window, height, width);
    code = pnoutrefresh(*pad, *y_offset, *x_offset, 1, 2, height - 2, width - 4);
    if (ERR == code) return result_ncurses_err();
    
    code = doupdate();
    if (ERR == code) return result_ncurses_err();
    
    code = keypad(*pad, TRUE);
    if (ERR == code) return result_ncurses_err();
    
    return result_success();
}


static void
dungeon_generation_progress(struct generator *generator, void *user_data)
{
    WINDOW *window = user_data;
    mvwprintw(window, 3, 2, "%i iterations", generator->iteration_count);
    mvwprintw(window, 4, 2, "%i diggers", generator->diggers_count);
    wclrtoeol(window);
    mvwprintw(window, 5, 2, "%i areas", generator->dungeon->areas_count);
    mvwprintw(window, 6, 2, "%i tiles", generator->dungeon->tiles_count);
    
    mvwprintw(window, 8, 2, "%i max depth", generator->max_size.height);
    mvwprintw(window, 9, 2, "%i max width", generator->max_size.width);
    mvwprintw(window, 10, 2, "%i max length", generator->max_size.length);
    
    mvwprintw(window, 12, 2, "%i padding", generator->padding);
    wrefresh(window);
}


static struct result
generate_dungeon(struct game *game)
{
    int code = ERR;
    WINDOW *window = stdscr;
    
    mvwprintw(window, 1, 2, "Generating dungeon...");
    wrefresh(window);
    
    clock_t start_clock = clock();
    
    struct dungeon *dungeon = dungeon_alloc();
    
    unsigned short random_seed[3];
    random_seed[0] = rnd_next_uniform_value_in_range(global_rnd, 0, USHRT_MAX);
    random_seed[1] = rnd_next_uniform_value_in_range(global_rnd, 0, USHRT_MAX);
    random_seed[2] = rnd_next_uniform_value_in_range(global_rnd, 0, USHRT_MAX);
    
    struct rnd *rnd = rnd_alloc_jrand48(random_seed);
    
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon,
                                                  rnd,
                                                  dungeon_options,
                                                  dungeon_generation_progress,
                                                  window);
    generator_generate(generator);
    dungeon_options_free(dungeon_options);
    
    rnd_free(rnd);
    
    clock_t end_clock = clock();
    double generation_time = (double)(end_clock - start_clock) / CLOCKS_PER_SEC;
    
    int starting_level = dungeon_starting_level(dungeon);
    int ending_level = dungeon_ending_level(dungeon);
    
    code = werase(window);
    if (ERR == code) return result_ncurses_err();
    
    mvwprintw(window, 1, 2, "Dungeon generated");
    
    mvwprintw(window, 3, 2, "%i iterations", generator->iteration_count);
    mvwprintw(window, 4, 2, "%i diggers", generator->diggers_count);
    mvwprintw(window, 5, 2, "%i areas", dungeon->areas_count);
    mvwprintw(window, 6, 2, "%i tiles", dungeon->tiles_count);
    
    mvwprintw(window, 8, 2, "%i max depth", generator->max_size.height);
    mvwprintw(window, 9, 2, "%i max width", generator->max_size.width);
    mvwprintw(window, 10, 2, "%i max length", generator->max_size.length);
    
    mvwprintw(window, 12, 2, "%i padding", generator->padding);
    
    mvwprintw(window, 14, 2, "starting level: %i", starting_level);
    mvwprintw(window, 15, 2, "ending level: %i", ending_level);
    
    mvwprintw(window, 17, 2, "%.2f seconds", generation_time);
    mvwprintw(window, 18, 2, "random seed: (%hu, %hu, %hu)",
              random_seed[0], random_seed[1], random_seed[2]);
    wgetch(window);
    
    generator_free(generator);
    
    int level = starting_level;
    
    int x_offset = 0;
    int y_offset = 0;
    WINDOW *pad = NULL;
    
    struct result result = draw_dungeon_level(dungeon, level, window, &pad, &x_offset, &y_offset);
    if (!result_is_success(result)) {
        dungeon_free(dungeon);
        return result_ncurses_err();
    }
    bool showing_map = true;
    
    int ch;
    while (true) {
        ch = wgetch(pad);
        if ('q' == ch || 27 == ch) {
            break;
        }
        
        int width, height;
        getmaxyx(window, height, width);
        int pad_width, pad_height;
        getmaxyx(pad, pad_height, pad_width);
        int hidden_column_count = pad_width - (width - 4);
        int hidden_line_count = pad_height - (height - 2);
        
        if ('k' == ch || KEY_UP == ch) {
            if (hidden_line_count > 0 && y_offset > 0) {
                y_offset -= 2;
                code = prefresh(pad, y_offset, x_offset, 1, 2, height - 2, width - 4);
                if (ERR == code) return result_ncurses_err();
            }
        }
        if ('j' == ch || KEY_DOWN == ch) {
            if (hidden_line_count > 0 && y_offset < hidden_line_count) {
                y_offset += 2;
                code = prefresh(pad, y_offset, x_offset, 1, 2, height - 2, width - 4);
                if (ERR == code) return result_ncurses_err();
            }
        }
        if ('h' == ch || KEY_LEFT == ch) {
            if (hidden_column_count > 0 && x_offset > 0) {
                x_offset -= 4;
                code = prefresh(pad, y_offset, x_offset, 1, 2, height - 2, width - 4);
                if (ERR == code) return result_ncurses_err();
            }
        }
        if ('l' == ch || KEY_RIGHT == ch) {
            if (hidden_column_count > 0 && x_offset < hidden_column_count) {
                x_offset += 4;
                code = prefresh(pad, y_offset, x_offset, 1, 2, height - 2, width - 4);
                if (ERR == code) return result_ncurses_err();
            }
        }
        if ('\r' == ch) {
            showing_map = !showing_map;
            if (showing_map) {
                struct result result = draw_dungeon_level(dungeon, level, window, &pad, &x_offset, &y_offset);
                if (!result_is_success(result)) {
                    dungeon_free(dungeon);
                    return result_ncurses_err();
                }
            } else {
                struct result result = list_dungeon_level_areas(dungeon, level, window, &pad, &x_offset, &y_offset);
                if (!result_is_success(result)) {
                    dungeon_free(dungeon);
                    return result_ncurses_err();
                }
            }
        }
        if ('u' == ch) {
            if (level > starting_level) {
                --level;
                
                if (showing_map) {
                    struct result result = draw_dungeon_level(dungeon, level, window, &pad, &x_offset, &y_offset);
                    if (!result_is_success(result)) {
                        dungeon_free(dungeon);
                        return result_ncurses_err();
                    }
                } else {
                    struct result result = list_dungeon_level_areas(dungeon, level, window, &pad, &x_offset, &y_offset);
                    if (!result_is_success(result)) {
                        dungeon_free(dungeon);
                        return result_ncurses_err();
                    }
                }
            }
        }
        if ('d' == ch) {
            if (level < ending_level) {
                ++level;
                
                if (showing_map) {
                    struct result result = draw_dungeon_level(dungeon, level, window, &pad, &x_offset, &y_offset);
                    if (!result_is_success(result)) {
                        dungeon_free(dungeon);
                        return result_ncurses_err();
                    }
                } else {
                    struct result result = list_dungeon_level_areas(dungeon, level, window, &pad, &x_offset, &y_offset);
                    if (!result_is_success(result)) {
                        dungeon_free(dungeon);
                        return result_ncurses_err();
                    }
                }
            }
        }
    }
    
    code = keypad(window, TRUE);
    if (ERR == code) return result_ncurses_err();
    
    code = delwin(pad);
    if (E_OK != code) return result_ncurses_error(code);
    
    dungeon_free(dungeon);
    
    return result_success();
}


static struct result
generate_treasure_type(struct game *game, char letter)
{
    struct treasure treasure;
    treasure_initialize(&treasure);
    treasure_type_generate(treasure_type_by_letter(letter), game->rnd, &treasure);
    
    struct ptr_array *lines = treasure_alloc_details(&treasure);
    
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
        if (ERR == code) { /* ignore */ }
        ++y;
    }
    
    ptr_array_clear(lines, free_or_die);
    ptr_array_free(lines);
    
    code = werase(stdscr);
    if (ERR == code) return result_ncurses_err();
    
    code = wborder(stdscr, '|', '|', '-', '-', '+', '+', '+', '+');
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
        if (isalpha(ch)) {
            ch = toupper(ch);
        }
        form_driver(form, ch);
        form_driver(form, REQ_FIRST_FIELD);
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
    
    selection_add_item(selection, "Create &Character", create_character);
    selection_add_item(selection, "Generate &Treasure", generate_treasure);
    selection_add_item(selection, "Generate &Dungeon", generate_dungeon);
    selection_add_item(selection, "&Quit", quit_game);
    
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
        
        set_escdelay(25);
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
    
    curs_set(0);

    return result_success();
}
