#ifndef FNF_DUNGEON_TILES_H_INCLUDED
#define FNF_DUNGEON_TILES_H_INCLUDED


#include <stdbool.h>

#include "point.h"


struct tile;


struct tiles {
    int count;
    struct tile **members;
};


void
tiles_add_tile(struct tiles *tiles, struct tile *tile);

struct tiles *
tiles_alloc(void);

void
tiles_free(struct tiles *tiles);

struct tile *
tiles_find_tile_at(struct tiles const *tiles, struct point point);

bool
tiles_remove_tile(struct tiles *tiles, struct tile const *tile);


#endif
