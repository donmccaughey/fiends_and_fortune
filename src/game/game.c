#include "game.h"

#include "character/character.h"

#include "common/alloc_or_die.h"
#include "common/result.h"

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


static struct result
generate_dungeon(struct game *game)
{
    mvprintw(1, 1, "Generate Dungeon");
    getch();
    return result_success();
}


static struct result
generate_treasure(struct game *game)
{
    mvprintw(1, 1, "Generate Treasure");
    getch();
    return result_success();
}


static struct result
quit_game(struct game *game)
{
    game->is_running = false;
    return result_success();
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
    if (game) endwin();
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
