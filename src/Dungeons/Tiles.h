#ifndef DUNGEONS_TILES_H_INCLUDED
#define DUNGEONS_TILES_H_INCLUDED


#include <stddef.h>
#include "Boolean.h"


struct Tile;


typedef int (*CompareFunction)(void const *, void const *);


struct Tiles
{
  struct Tile **tiles;
  size_t capacity;
  size_t count;
  CompareFunction compare;
};


void addTileToTiles(struct Tiles *tiles, struct Tile *tile);

void finalizeTiles(struct Tiles *tiles);

void initializeTiles(struct Tiles *tiles);

Boolean removeTileFromTiles(struct Tiles *tiles, struct Tile *tile);


#endif /* #ifndef DUNGEONS_TILES_H_INCLUDED */
