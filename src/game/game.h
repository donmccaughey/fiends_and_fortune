#ifndef FNF_GAME_GAME_H_INCLUDED
#define FNF_GAME_GAME_H_INCLUDED

#include <ncurses.h>
#include <stdbool.h>

#include "common/result.h"


struct rnd;


struct game {
    bool is_running;
    struct rnd *rnd;
};


struct game *
game_alloc_or_die(struct rnd *rnd);

void
game_free_or_die(struct game *game);

void
game_hide(struct game *game);

struct result
game_run(struct game *game);

struct result
game_show(struct game *game);


#endif
