#ifndef FNF_GAME_GAME_H_INCLUDED
#define FNF_GAME_GAME_H_INCLUDED

#include <ncurses.h>
#include <menu.h>


struct game {
    MENU *menu;
};


struct game *
game_alloc(void);

void
game_free(struct game *game);

int
game_play(struct game *game);


#endif
