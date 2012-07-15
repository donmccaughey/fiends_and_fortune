#include "Tiles.h"

#include <stdlib.h>
#include <string.h>
#include "heap.h"
#include "Tile.h"
#include "TileStatistics.h"


static void appendTileToTiles(struct Tiles *tiles, struct Tile const *tile);
static int compareTilesByCoordinate(void const *item1, void const *item2);
static void finalizeTiles(struct Tiles *tiles);
static void gatherStatistics(struct Tile const *tile, struct TileStatistics *statistics);
static void initializeTiles(struct Tiles *tiles);


void addTileToTiles(struct Tiles *tiles, struct Tile const *tile)
{
  appendTileToTiles(tiles, tile);
  qsort(tiles->tiles, tiles->count, sizeof(struct Tile), compareTilesByCoordinate);
}


struct Tile *allTiles(struct Tiles const *tiles)
{
  return tiles->tiles;
}


static void appendTileToTiles(struct Tiles *tiles, struct Tile const *tile)
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

  return comparePoints(&tile1->point, &tile2->point);
}


struct Tiles *createTiles(void)
{
    struct Tiles *tiles = MALLOC_OR_DIE(sizeof(struct Tiles));
    initializeTiles(tiles);
    return tiles;
}


void destroyTiles(struct Tiles *tiles)
{
    finalizeTiles(tiles);
    free(tiles);
}


static void finalizeTiles(struct Tiles *tiles)
{
  free(tiles->tiles);
}


struct Tile *findTileInTilesAt(struct Tiles const *tiles, int32_t x, int32_t y, int32_t z)
{
  struct Tile tile = { .point = { x, y, z} };

  struct Tile *tileInTiles = bsearch(&tile, tiles->tiles, tiles->count, sizeof(struct Tile), compareTilesByCoordinate);
  return tileInTiles ? tileInTiles : NULL;
}


static void gatherStatistics(struct Tile const *tile, struct TileStatistics *statistics)
{
  if (tile->point.x < statistics->xRange.begin) {
    statistics->xRange.begin = tile->point.x;
  }
  if (tile->point.x >= statistics->xRange.end) {
    statistics->xRange.end = tile->point.x + 1;
  }
  if (tile->point.y < statistics->yRange.begin) {
    statistics->yRange.begin = tile->point.y;
  }
  if (tile->point.y >= statistics->yRange.end) {
    statistics->yRange.end = tile->point.y + 1;
  }

  ++statistics->count;
}


void gatherTileStatistics(struct Tiles const *tiles, struct TileStatistics *statistics)
{
  statistics->count = 0;
  statistics->xRange = (struct Range) { INT32_MAX, INT32_MIN };
  statistics->yRange = (struct Range) { INT32_MAX, INT32_MIN };

  for (size_t i = 0; i < tiles->count; ++i) {
    gatherStatistics(&tiles->tiles[i], statistics);
  }
}


static void initializeTiles(struct Tiles *tiles)
{
  memset(tiles, 0, sizeof(struct Tiles));
  tiles->tiles = CALLOC_OR_DIE(0, sizeof(struct Tile));
}


Boolean removeTileFromTiles(struct Tiles *tiles, struct Tile const *tile)
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


size_t tilesCapacity(struct Tiles const *tiles)
{
  return tiles->capacity;
}


size_t tilesCount(struct Tiles const *tiles)
{
  return tiles->count;
}


struct Tiles const tilesOnLevel(struct Tiles const *tiles, int32_t z)
{
  struct Tiles tilesOnLevel = { .capacity = 0, .count = 0, .tiles = NULL };

  // TODO: replace linear search with binary lower bound/upper bound search
  for (size_t i = 0; i < tiles->count; ++i) {
    struct Tile *tile = &tiles->tiles[i];
    if (tile->point.z == z) {
      if ( ! tilesOnLevel.tiles) {
        tilesOnLevel.tiles = tile;
      }
      ++tilesOnLevel.count;
    } else if (tilesOnLevel.tiles) {
      return tilesOnLevel;
    }
  }

  return tilesOnLevel;
}
