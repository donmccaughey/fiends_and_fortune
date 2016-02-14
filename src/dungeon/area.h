#ifndef FNF_DUNGEON_AREA_H_INCLUDED
#define FNF_DUNGEON_AREA_H_INCLUDED


#include "area_type.h"
#include "box.h"
#include "tile_type.h"


struct tile;


struct area {
    struct box box;
    enum direction direction;
    enum area_type type;
};


struct area *
area_alloc(enum area_type area_type,
           enum direction direction,
           struct box box,
           enum tile_type tile_type);

void
area_free(struct area *area);

char *
area_alloc_description(struct area const *area);


#endif
