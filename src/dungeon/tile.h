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

struct tile *
tile_alloc_copy(struct tile *tile);

void
tile_free(struct tile *tile);

bool
tile_equals(struct tile const *tile, struct tile const *other);

bool
tile_is_blank(struct tile const *tile);

bool
tile_is_escavated(struct tile const *tile);

bool
tile_is_unescavated(struct tile const *tile);

bool
tile_has_south_exit(struct tile const *tile);

bool
tile_has_south_wall(struct tile const *tile);

bool
tile_has_west_exit(struct tile const *tile);

bool
tile_has_west_wall(struct tile const *tile);

struct tile **
tile_add_to_array_sorted_by_point(struct tile **tiles,
                                  int *count,
                                  struct tile *tile);

struct tile **
tile_find_in_array_sorted_by_point(struct tile **tiles,
                                   int count,
                                   struct point point);

void
tile_sort_array_by_point(struct tile **tiles, int count);


#endif
