#ifndef FNF_DUNGEON_AREA_H_INCLUDED
#define FNF_DUNGEON_AREA_H_INCLUDED


#include "area_type.h"
#include "box.h"
#include "orientation.h"
#include "tile_type.h"


struct dungeon;
struct tile;


struct area {
    struct box box;
    char *description;
    enum orientation orientation;
    struct tile **tiles;
    int tiles_count;
    enum area_type type;
};


struct area *
area_alloc(struct dungeon *dungeon,
           enum area_type area_type,
           enum orientation orientation,
           struct box box,
           enum tile_type tile_type);

void
area_free(struct area *area);


#endif
