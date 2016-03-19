#include "game.h"

#include "common/alloc_or_die.h"
#include "common/result.h"

#include "selection.h"


struct game *
game_alloc(void)
{
    struct game *game = calloc_or_die(1, sizeof(struct game));
    
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
    
    selection_add_item(selection, "Create Character");
    selection_add_item(selection, "Generate Treasure");
    selection_add_item(selection, "Generate Dungeon");
    selection_add_item(selection, "Quit");
    
    struct result result = selection_show(selection, stdscr);
    int index = selection->index;
    selection_free(selection);
    if (!result_is_success(result)) return -1;
    
    switch (index) {
        case -1:
            mvprintw(1, 1, "Error");
            break;
        case 0:
            mvprintw(1, 1, "Character Creation");
            break;
        case 1:
            mvprintw(1, 1, "Treasure Generation");
            break;
        case 2:
            mvprintw(1, 1, "Dungeon Generation");
            break;
        case 3:
            return 0;
        default:
            mvprintw(1, 1, "Unknown");
            break;
    }
    getch();
    
    return 0;
}
