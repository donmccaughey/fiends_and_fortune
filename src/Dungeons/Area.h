#ifndef DUNGEONS_AREA_H_INCLUDED
#define DUNGEONS_AREA_H_INCLUDED


#include <stddef.h>

#include "AreaType.h"


struct Tile;
struct Tiles;


struct Area {
    char *description;
    struct Tiles *tiles;
    enum AreaType type;
};


struct Area *createArea(char const *description, struct Tiles *parentTiles, enum AreaType type);

void destroyArea(struct Area *area);


#endif /* #ifndef DUNGEONS_AREA_H_INCLUDED */
