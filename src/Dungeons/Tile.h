#ifndef DUNGEONS_TILE_H_INCLUDED
#define DUNGEONS_TILE_H_INCLUDED


#include "Point.h"
#include "TileType.h"


struct Tile {
  struct Point point;
  enum TileType type;
};


#endif /* #ifndef DUNGEONS_TILE_H_INCLUDED */
