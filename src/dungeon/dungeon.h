#ifndef FNF_DUNGEON_DUNGEON_H_INCLUDED
#define FNF_DUNGEON_DUNGEON_H_INCLUDED


#include <stdio.h>

#include "area_type.h"
#include "orientation.h"
#include "point.h"
#include "range.h"
#include "tile_type.h"


struct area;
struct areas;
struct rnd;
struct tile;
struct tiles;


struct dungeon {
    struct area **areas;
    int areas_count;
    struct tile **tiles;
    int tiles_count;
    struct tiles *xtiles;
};


struct dungeon *
dungeon_alloc(void);

void
dungeon_free(struct dungeon *dungeon);

void
dungeon_generate(struct dungeon *dungeon, struct rnd *rnd);

void
dungeon_generate_small(struct dungeon *dungeon);

struct area *
dungeon_add_area(struct dungeon *dungeon,
                 enum area_type area_type,
                 enum orientation orientation,
                 enum tile_type tile_type,
                 struct range x_range,
                 struct range y_range,
                 int level);

struct tile *
dungeon_add_tile(struct dungeon *dungeon,
                 struct point point,
                 enum tile_type type);

void
dungeon_print_level(struct dungeon *dungeon, int level, FILE *out);


#endif
