#ifndef DUNGEONS_TILE_H_INCLUDED
#define DUNGEONS_TILE_H_INCLUDED


#include "point.h"
#include "TileType.h"


struct Tile {
    struct point point;
    enum TileType type;
};


struct Tile *createTile(struct point point, enum TileType type);

void destroyTile(struct Tile *tile);


#endif /* #ifndef DUNGEONS_TILE_H_INCLUDED */
