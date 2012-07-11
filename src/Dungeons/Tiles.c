#include "Tiles.h"

#include <stdlib.h>
#include <string.h>
#include "heap.h"
#include "Tile.h"


static void appendTileToTiles(struct Tiles *tiles, struct Tile *tile);


void addTileToTiles(struct Tiles *tiles, struct Tile *tile)
{
  appendTileToTiles(tiles, tile);
  qsort(tiles->tiles, tiles->count, sizeof(struct Tile *), tiles->compare);
}


static void appendTileToTiles(struct Tiles *tiles, struct Tile *tile)
{
  if (tiles->capacity == tiles->count) {
    if (tiles->capacity) {
      tiles->capacity *= 2;
    } else {
      tiles->capacity = 256;
    }
    tiles->tiles = REALLOC_OR_DIE(tiles->tiles, tiles->capacity * sizeof(struct Tile *));
  }
  tiles->tiles[tiles->count] = tile;
  ++tiles->count;
}


void finalizeTiles(struct Tiles *tiles)
{
  free(tiles->tiles);
}


void initializeTiles(struct Tiles *tiles, CompareFunction compare)
{
  memset(tiles, 0, sizeof(struct Tiles));
  tiles->tiles = CALLOC_OR_DIE(0, sizeof(struct Tile *));
  tiles->compare = compare;
}


Boolean removeTileFromTiles(struct Tiles *tiles, struct Tile *tile)
{
  struct Tile **found = bsearch(&tile, tiles->tiles, tiles->count, sizeof(struct Tile *), tiles->compare);
  if ( ! found) {
    return FALSE;
  }
  struct Tile **tail = found + 1;
  struct Tile **end = tiles->tiles + tiles->count;
  memmove(found, tail, (end - tail) * sizeof(struct Tile *));
  --tiles->count;
  return TRUE;
}
