#ifndef FNF_DUNGEON_TILE_H_INCLUDED
#define FNF_DUNGEON_TILE_H_INCLUDED


#include <stdbool.h>

#include "point.h"
#include "tile_type.h"
#include "wall_type.h"


struct tile {
    struct point point;
    enum tile_type type;
    struct {
        enum wall_type south;
        enum wall_type west;
    } walls;
};


struct tile *
tile_alloc(struct point point, enum tile_type type);

void
tile_free(struct tile *tile);

bool
tile_equals(struct tile *tile, struct tile *other);

bool
tile_is_blank(struct tile *tile);

bool
tile_is_unescavated(struct tile *tile);


#endif
