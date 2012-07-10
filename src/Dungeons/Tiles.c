#include "Tiles.h"

#include <stdlib.h>
#include <string.h>
#include "heap.h"
#include "Tile.h"


static void appendTileToTiles(struct Tiles *tiles, struct Tile *tile);
static int compareTilesByAddress(void const *item1, void const *item2);
static Boolean isTileInTileIndex(struct Tiles *index, struct Tile *tile);
static void pushTileIfNotDiscovered(struct Tiles *stack, struct Tile *tile, struct Tiles *discovered);
static Boolean visitTile(struct Tiles *stack, struct Tiles *discovered, void *context, VisitFunction visit);


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


static int compareTilesByAddress(void const *item1, void const *item2)
{
  struct Tile *const *pointer1 = item1;
  struct Tile *const *pointer2 = item2;
  
  struct Tile *tile1 = *pointer1;
  struct Tile *tile2 = *pointer2;
  
  ptrdiff_t difference = tile1 - tile2;
  if (difference < 0) return -1;
  if (difference > 0) return 1;
  return 0;
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


static Boolean isTileInTileIndex(struct Tiles *index, struct Tile *tile)
{
  void *found = bsearch(&tile, index->tiles, index->count, sizeof(struct Tile *), index->compare);
  return found ? TRUE : FALSE;
}


struct Tile *popTile(struct Tiles *stack)
{
  return stack->count ? stack->tiles[--stack->count] : NULL;
}


void pushTile(struct Tiles *stack, struct Tile *tile)
{
  appendTileToTiles(stack, tile);
}


static void pushTileIfNotDiscovered(struct Tiles *stack, struct Tile *tile, struct Tiles *discovered)
{
  if (tile && ! isTileInTileIndex(discovered, tile)) {
    pushTile(stack, tile);
    addTileToTileIndex(discovered, tile);
  }
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


static Boolean visitTile(struct Tiles *stack, struct Tiles *discovered, void *context, VisitFunction visit)
{
  struct Tile *tile = popTile(stack);
  
  pushTileIfNotDiscovered(stack, tile->north, discovered);
  pushTileIfNotDiscovered(stack, tile->south, discovered);
  pushTileIfNotDiscovered(stack, tile->east, discovered);
  pushTileIfNotDiscovered(stack, tile->west, discovered);
  
  return visit(tile, context);
}


Boolean visitTiles(struct Tile *tile, void *context, VisitFunction visit)
{
  struct Tiles discovered;
  initializeTileIndex(&discovered, compareTilesByAddress);
  
  struct Tiles stack;
  initializeTiles(&stack);
  
  pushTileIfNotDiscovered(&stack, tile, &discovered);
  Boolean stopped;
  do {
    stopped = visitTile(&stack, &discovered, context, visit);
  } while (stack.count && ! stopped);
  
  finalizeTiles(&stack);
  finalizeTiles(&discovered);
  
  return stopped;
}
