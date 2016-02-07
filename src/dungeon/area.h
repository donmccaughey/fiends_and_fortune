#ifndef FNF_DUNGEON_AREA_H_INCLUDED
#define FNF_DUNGEON_AREA_H_INCLUDED


#include "area_type.h"
#include "orientation.h"
#include "point.h"
#include "range.h"
#include "tile_type.h"


struct dungeon;
struct tile;
struct tiles;


struct area {
    struct dungeon *dungeon;
    char *description;
    enum orientation orientation;
    struct tiles *tiles;
    enum area_type type;
};


void
area_init(struct area *area,
          struct dungeon *dungeon,
          enum area_type area_type,
          enum orientation orientation,
          enum tile_type tile_type,
          struct range x_range,
          struct range y_range,
          int z);

void
area_fin(struct area *area);


#endif
