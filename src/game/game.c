#include "game.h"

#include "character/character.h"

#include "common/alloc_or_die.h"
#include "common/result.h"

#include "selection.h"


static void
draw_character_sheet(struct game *game, int characteristics[6])
{
    mvwprintw(stdscr, 0, 0, "Strength:     %2i\n", characteristics[0]);
    mvwprintw(stdscr, 1, 0, "Intelligence: %2i\n", characteristics[1]);
    mvwprintw(stdscr, 2, 0, "Wisdom:       %2i\n", characteristics[2]);
    mvwprintw(stdscr, 3, 0, "Dexterity:    %2i\n", characteristics[3]);
    mvwprintw(stdscr, 4, 0, "Constitution: %2i\n", characteristics[4]);
    mvwprintw(stdscr, 5, 0, "Charisma:     %2i\n", characteristics[5]);
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
    struct selection *selection = selection_alloc("Create Character");
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
    selection_free(selection);
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


struct game *
game_alloc(struct rnd *rnd)
{
    struct game *game = calloc_or_die(1, sizeof(struct game));
    
    game->rnd = rnd;
    
    WINDOW *window = initscr();
    if (!window) {
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
    
    return game;
}


void
game_free(struct game *game)
{
    if (!game) return;
    
    endwin();
    free_or_die(game);
}


int
game_play(struct game *game)
{
    struct selection *selection = selection_alloc("Fiends & Fortune");
    if (!selection) return -1;
    
    selection_add_item(selection, "Create Character", create_character);
    selection_add_item(selection, "Generate Treasure", generate_treasure);
    selection_add_item(selection, "Generate Dungeon", generate_dungeon);
    selection_add_item(selection, "Quit", NULL);
    
    struct result result = selection_show(selection, stdscr);
    selection_action_fn *action = selection_selected_action(selection);
    selection_free(selection);
    if (!result_is_success(result)) return -1;
    
    int code = wrefresh(stdscr);
    if (ERR == code) return -1;
    
    if (action) {
        result = action(game);
        if (!result_is_success(result)) return -1;
    }
    
    return 0;
}
