#include "Tile.h"

#include <stdlib.h>

#include "earmark.h"


struct Tile *createTile(struct Point point, enum TileType type)
{
  struct Tile *tile = em_malloc(sizeof(struct Tile));
  tile->point = point;
  tile->type = type;
  return tile;
}


void destroyTile(struct Tile *tile)
{
  free(tile);
}
