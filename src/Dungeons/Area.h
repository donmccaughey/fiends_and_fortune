#ifndef DUNGEONS_AREA_H_INCLUDED
#define DUNGEONS_AREA_H_INCLUDED


#include <stddef.h>

#include "AreaType.h"


struct tile;
struct tiles;


struct Area {
    char *description;
    struct tiles *tiles;
    enum AreaType type;
};


struct Area *createArea(char const *description, struct tiles *parentTiles, enum AreaType type);

void destroyArea(struct Area *area);


#endif /* #ifndef DUNGEONS_AREA_H_INCLUDED */
