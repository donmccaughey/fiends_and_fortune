#include "Tiles.h"

#include <stdlib.h>
#include <string.h>
#include "heap.h"
#include "Tile.h"


static void appendTileToTiles(struct Tiles *tiles, struct Tile *tile);


void addTileToTileIndex(struct Tiles *index, struct Tile *tile)
{
  appendTileToTiles(index, tile);
  qsort(index->tiles, index->count, sizeof(struct Tile *), index->compare);
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


void initializeTiles(struct Tiles *tiles)
{
  memset(tiles, 0, sizeof(struct Tiles));
  tiles->tiles = CALLOC_OR_DIE(0, sizeof(struct Tile *));
}


void initializeTileIndex(struct Tiles *index, CompareFunction compare)
{
  initializeTiles(index);
  index->compare = compare;
}


struct Tile *popTile(struct Tiles *stack)
{
  return stack->count ? stack->tiles[--stack->count] : NULL;
}


void pushTile(struct Tiles *stack, struct Tile *tile)
{
  appendTileToTiles(stack, tile);
}


Boolean removeTileFromTileIndex(struct Tiles *index, struct Tile *tile)
{
  struct Tile **found = bsearch(&tile, index->tiles, index->count, sizeof(struct Tile *), index->compare);
  if ( ! found) {
    return FALSE;
  }
  struct Tile **tail = found + 1;
  struct Tile **end = index->tiles + index->count;
  memmove(found, tail, (end - tail) * sizeof(struct Tile *));
  --index->count;
  return TRUE;
}
