#ifndef DUNGEONS_TILE_H_INCLUDED
#define DUNGEONS_TILE_H_INCLUDED


#include "TileType.h"


struct Tile {
  enum TileType type;
  int x;
  int y;
  int z;
};


#endif /* #ifndef DUNGEONS_TILE_H_INCLUDED */
