#include "Tiles.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"
#include "Tile.h"


static void appendTileToTiles(struct Tiles *tiles, struct Tile  *tile);
static int compareTilesByCoordinate(void const *item1, void const *item2);
static struct Tile **find(struct Tiles const *tiles, struct Tile const *criteria);
static void sort(struct Tiles *tiles);
static void updateRanges(struct Tiles *tiles);


struct Tiles {
  size_t capacity;
  int (*compare)(void const *, void const *);
  size_t count;
  struct Tiles *parent;
  struct Tile **tiles;
  struct Range xRange;
  struct Range yRange;
  struct Range zRange;
};


void addTileToTiles(struct Tiles *tiles, struct Tile *tile)
{
  if (tiles->parent) addTileToTiles(tiles->parent, tile);
  appendTileToTiles(tiles, tile);
  sort(tiles);
}


static void appendTileToTiles(struct Tiles *tiles, struct Tile  *tile)
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
  tiles->xRange = extendRangeToIncludeValue(tiles->xRange, tile->point.x);
  tiles->yRange = extendRangeToIncludeValue(tiles->yRange, tile->point.y);
  tiles->zRange = extendRangeToIncludeValue(tiles->zRange, tile->point.z);
}


static int compareTilesByCoordinate(void const *item1, void const *item2)
{
  struct Tile *const *pointer1 = item1;
  struct Tile *const *pointer2 = item2;
  struct Tile *tile1 = *pointer1;
  struct Tile *tile2 = *pointer2;

  return comparePoints(tile1->point, tile2->point);
}


struct Tiles *createEmptyTilesWithParent(struct Tiles *tiles)
{
  struct Tiles *emptyTiles = createTiles();
  emptyTiles->parent = tiles;
  return emptyTiles;
}


struct Tiles *createTiles(void)
{
  struct Tiles *tiles = CALLOC_OR_DIE(1, sizeof(struct Tiles));
  tiles->compare = compareTilesByCoordinate;
  tiles->tiles = CALLOC_OR_DIE(0, sizeof(struct Tile *));
  return tiles;
}


struct Tiles *createTilesOnLevel(struct Tiles *tiles, int32_t z)
{
  struct Tiles *tilesOnLevel = createEmptyTilesWithParent(tiles);

  // TODO: replace linear search with binary lower/upper bound search
  // and memcpy the whole block of tiles
  for (size_t i = 0; i < tiles->count; ++i) {
    struct Tile *tile = tiles->tiles[i];
    if (tile->point.z == z) {
      appendTileToTiles(tilesOnLevel, tile);
    } else if (tile->point.z > z) {
      break;
    }
  }

  return tilesOnLevel;
}


void destroyTiles(struct Tiles *tiles)
{
  if ( ! tiles->parent) {
    for (size_t i = 0; i < tiles->count; ++i) {
      destroyTile(tiles->tiles[i]);
    }
  }
  free(tiles->tiles);
  free(tiles);
}


static struct Tile **find(struct Tiles const *tiles, struct Tile const *criteria)
{
  return bsearch(&criteria, tiles->tiles, tiles->count, sizeof(struct Tile *), tiles->compare);
}


struct Tile *findTileInTilesAt(struct Tiles const *tiles, int32_t x, int32_t y, int32_t z)
{
  struct Tile tile = { .point = { x, y, z} };
  struct Tile **tileInTiles = find(tiles, &tile);
  return tileInTiles ? *tileInTiles : NULL;
}


Boolean removeTileFromTiles(struct Tiles *tiles, struct Tile const *tile)
{
  // TODO: if tile isn't unique by compare criteria, the wrong tile may be removed
  // is this a problem?
  struct Tile **found = find(tiles, tile);
  if ( ! found) {
    // TODO: should we search the parent in this case?
    return tiles->parent ? removeTileFromTiles(tiles->parent, tile) : FALSE;
  }

  struct Tile **tail = found + 1;
  struct Tile **end = tiles->tiles + tiles->count;
  memmove(found, tail, (end - tail) * sizeof(struct Tile));
  --tiles->count;
  updateRanges(tiles);

  return tiles->parent ? removeTileFromTiles(tiles->parent, tile) : TRUE;
}


static void sort(struct Tiles *tiles)
{
  qsort(tiles->tiles, tiles->count, sizeof(struct Tile *), tiles->compare);
}


struct Tile *tileInTilesAtIndex(struct Tiles const *tiles, size_t index)
{
  assert(index < tiles->count);
  return tiles->tiles[index];
}


size_t tilesCount(struct Tiles const *tiles)
{
  return tiles->count;
}


static void updateRanges(struct Tiles *tiles)
{
  tiles->xRange = makeRange(0, 0);
  tiles->yRange = makeRange(0, 0);
  tiles->zRange = makeRange(0, 0);
  for (size_t i = 0; i < tiles->count; ++i) {
    struct Tile *tile = tiles->tiles[i];
    tiles->xRange = extendRangeToIncludeValue(tiles->xRange, tile->point.x);
    tiles->yRange = extendRangeToIncludeValue(tiles->yRange, tile->point.y);
    tiles->zRange = extendRangeToIncludeValue(tiles->zRange, tile->point.z);
  }
}


struct Range xRangeOfTiles(struct Tiles const *tiles)
{
  return tiles->xRange;
}


struct Range yRangeOfTiles(struct Tiles const *tiles)
{
  return tiles->yRange;
}


struct Range zRangeOfTiles(struct Tiles const *tiles)
{
  return tiles->zRange;
}
