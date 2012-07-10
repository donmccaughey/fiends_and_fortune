#ifndef DUNGEONS_TILES_H_INCLUDED
#define DUNGEONS_TILES_H_INCLUDED


#include <stddef.h>
#include "Boolean.h"


struct Tile;


typedef int (*CompareFunction)(void const *, void const *);

/* return TRUE to stop graph traversal */
typedef Boolean (*VisitFunction)(struct Tile *tile, void *context);


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

struct Tile *popTile(struct Tiles *stack);

void pushTile(struct Tiles *stack, struct Tile *tile);

Boolean removeTileFromTileIndex(struct Tiles *index, struct Tile *tile);

Boolean visitTiles(struct Tile *tile, void *context, VisitFunction visit);


#endif /* #ifndef DUNGEONS_TILES_H_INCLUDED */
