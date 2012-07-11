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


void addTileToTileIndex(struct Tiles *index, struct Tile *tile);

void finalizeTiles(struct Tiles *tiles);

void initializeTiles(struct Tiles *tiles);

void initializeTileIndex(struct Tiles *index, CompareFunction compare);

Boolean removeTileFromTileIndex(struct Tiles *index, struct Tile *tile);


#endif /* #ifndef DUNGEONS_TILES_H_INCLUDED */
