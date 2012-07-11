#include "Tiles.h"

#include <stdlib.h>
#include <string.h>
#include "heap.h"
#include "Tile.h"


static void appendTileToTiles(struct Tiles *tiles, struct Tile *tile);
static int compareTilesByCoordinate(void const *item1, void const *item2);


void addTileToTiles(struct Tiles *tiles, struct Tile *tile)
{
  appendTileToTiles(tiles, tile);
  qsort(tiles->tiles, tiles->count, sizeof(struct Tile), compareTilesByCoordinate);
}


static void appendTileToTiles(struct Tiles *tiles, struct Tile *tile)
{
  if (tiles->capacity == tiles->count) {
    if (tiles->capacity) {
      tiles->capacity *= 2;
    } else {
      tiles->capacity = 256;
    }
    tiles->tiles = REALLOC_OR_DIE(tiles->tiles, tiles->capacity * sizeof(struct Tile));
  }
  tiles->tiles[tiles->count] = *tile;
  ++tiles->count;
}


static int compareTilesByCoordinate(void const *item1, void const *item2)
{
  struct Tile const *tile1 = item1;
  struct Tile const *tile2 = item2;

  if (tile1->point.z != tile2->point.z) {
    return tile1->point.z - tile2->point.z;
  }
  if (tile1->point.y != tile2->point.y) {
    return tile1->point.y - tile2->point.y;
  }
  return tile1->point.x - tile2->point.x;
}


void finalizeTiles(struct Tiles *tiles)
{
  free(tiles->tiles);
}


struct Tile *findTileInTilesAt(struct Tiles *tiles, int x, int y, int z)
{
  struct Tile tile = { .point = { x, y, z} };

  struct Tile *tileInTiles = bsearch(&tile, tiles->tiles, tiles->count, sizeof(struct Tile), compareTilesByCoordinate);
  return tileInTiles ? tileInTiles : NULL;
}


void initializeTiles(struct Tiles *tiles)
{
  memset(tiles, 0, sizeof(struct Tiles));
  tiles->tiles = CALLOC_OR_DIE(0, sizeof(struct Tile));
}


Boolean removeTileFromTiles(struct Tiles *tiles, struct Tile *tile)
{
  struct Tile *found = bsearch(tile, tiles->tiles, tiles->count, sizeof(struct Tile), compareTilesByCoordinate);
  if ( ! found) {
    return FALSE;
  }
  struct Tile *tail = found + 1;
  struct Tile *end = tiles->tiles + tiles->count;
  memmove(found, tail, (end - tail) * sizeof(struct Tile));
  --tiles->count;
  return TRUE;
}
