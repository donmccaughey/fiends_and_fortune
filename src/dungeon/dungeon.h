#ifndef FNF_DUNGEON_DUNGEON_H_INCLUDED
#define FNF_DUNGEON_DUNGEON_H_INCLUDED


#include <stdio.h>

#include "box.h"
#include "tile_type.h"


struct area;
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

void
dungeon_add_area(struct dungeon *dungeon, struct area *area);

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
