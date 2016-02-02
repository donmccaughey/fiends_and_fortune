#ifndef FNF_DUNGEON_AREA_H_INCLUDED
#define FNF_DUNGEON_AREA_H_INCLUDED


#include <stddef.h>

#include "area_type.h"
#include "point.h"
#include "range.h"
#include "tile_type.h"


struct tile;
struct tiles;


struct area {
    char *description;
    struct tiles *tiles;
    enum area_type type;
};


struct area *
area_alloc(char const *description,
           struct tiles *parent_tiles,
           enum area_type type);

void
area_free(struct area *area);

void
area_add_tiles(struct area *area,
               enum tile_type type,
               struct range x_range,
               struct range y_range,
               int32_t z);


#endif
