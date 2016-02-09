#ifndef FNF_DUNGEON_DUNGEON_H_INCLUDED
#define FNF_DUNGEON_DUNGEON_H_INCLUDED


#include <stdio.h>

#include "area_type.h"
#include "box.h"
#include "direction.h"
#include "orientation.h"
#include "point.h"
#include "tile_type.h"
#include "wall_type.h"


struct area;
struct areas;
struct rnd;
struct tile;


struct dungeon {
    struct area **areas;
    int areas_count;
    struct tile **tiles;
    int tiles_count;
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
                 struct box box,
                 enum tile_type tile_type);

struct tile *
dungeon_add_tile(struct dungeon *dungeon,
                 struct point point,
                 enum tile_type tile_type);

void
dungeon_fill_box(struct dungeon *dungeon,
                 struct box box,
                 enum tile_type tile_type);

void
dungeon_set_wall(struct dungeon *dungeon,
                 struct point point,
                 enum direction direction,
                 enum wall_type wall_type);

void
dungeon_set_walls(struct dungeon *dungeon,
                  struct box box,
                  enum wall_type wall_type);

bool
dungeon_is_box_excavated(struct dungeon *dungeon, struct box box);

struct box
dungeon_box_for_level(struct dungeon *dungeon, int level);

struct tile **
dungeon_alloc_tiles_for_box(struct dungeon *dungeon, struct box box);

struct tile *
dungeon_tile_at(struct dungeon *dungeon, struct point point);

void
dungeon_print_level(struct dungeon *dungeon, int level, FILE *out);


#endif
