#ifndef DUNGEONS_TILE_H_INCLUDED
#define DUNGEONS_TILE_H_INCLUDED


#include "TileType.h"


struct Tile {
  enum TileType type;
  int x;
  int y;
  int z;
  struct Tile *north;
  struct Tile *south;
  struct Tile *east;
  struct Tile *west;
};


#endif /* #ifndef DUNGEONS_TILE_H_INCLUDED */
