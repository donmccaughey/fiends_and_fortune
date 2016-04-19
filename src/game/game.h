#ifndef FNF_GAME_GAME_H_INCLUDED
#define FNF_GAME_GAME_H_INCLUDED

#include <ncurses.h>


struct rnd;


struct game {
    struct rnd *rnd;
};


struct game *
game_alloc(struct rnd *rnd);

void
game_free(struct game *game);

int
game_play(struct game *game);


#endif
