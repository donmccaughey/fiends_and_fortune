#include "game.h"

#include <ncurses.h>

#include "common/alloc_or_die.h"


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
    printw("Hello, world!");
    refresh();
    getch();
    endwin();
    return 0;
}
