#include "Tile.h"

#include <stdlib.h>

#include "common/alloc_or_die.h"


struct Tile *createTile(struct point point, enum TileType type)
{
  struct Tile *tile = malloc_or_die(sizeof(struct Tile));
  tile->point = point;
  tile->type = type;
  return tile;
}


void destroyTile(struct Tile *tile)
{
  free_or_die(tile);
}
