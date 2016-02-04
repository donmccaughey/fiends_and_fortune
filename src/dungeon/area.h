#ifndef FNF_DUNGEON_AREA_H_INCLUDED
#define FNF_DUNGEON_AREA_H_INCLUDED


#include <stddef.h>

#include "area_type.h"
#include "orientation.h"
#include "point.h"
#include "range.h"
#include "tile_type.h"


struct tile;
struct tiles;


struct area {
    char *description;
    enum orientation orientation;
    struct tiles *tiles;

    enum area_type type;
};


struct area *
area_alloc(struct tiles *parent_tiles,
           enum area_type area_type,
           enum orientation orientation,
           enum tile_type tile_type,
           struct range x_range,
           struct range y_range,
           int32_t z);

void
area_free(struct area *area);


#endif
