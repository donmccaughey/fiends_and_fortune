#include "Tile.h"

#include <stdlib.h>
#include "heap.h"


struct Tile *createTile(struct Point point, enum TileType type)
{
  struct Tile *tile = MALLOC_OR_DIE(sizeof(struct Tile));
  tile->point = point;
  tile->type = type;
  return tile;
}


void destroyTile(struct Tile *tile)
{
  free(tile);
}
