#include <assert.h>
#include "Dungeon.h"
#include "Tile.h"
#include "Tiles.h"


void tilesTest(void);
static void pushAndPopTileTest(void);
static void tilesIndexTest(void);


void tilesTest(void)
{
  pushAndPopTileTest();
  tilesIndexTest();
}


static void pushAndPopTileTest(void)
{
  struct Tiles stack;
  
  initializeTiles(&stack);
  
  assert(0 == stack.capacity);
  assert(0 == stack.count);
  assert(stack.tiles);
  
  struct Tile tile1 = {};
  pushTile(&stack, &tile1);
  
  assert(1 == stack.count);
  assert(stack.capacity > 0);
  assert(stack.tiles[0] == &tile1);
  
  struct Tile tile2 = {};
  pushTile(&stack, &tile2);
  
  assert(2 == stack.count);
  assert(stack.tiles[1] == &tile2);
  assert(stack.tiles[0] == &tile1);
  
  struct Tile *poppedTile = popTile(&stack);
  
  assert(1 == stack.count);
  assert(stack.tiles[0] == &tile1);
  assert(poppedTile == &tile2);
  
  poppedTile = popTile(&stack);
  
  assert(0 == stack.count);
  assert(poppedTile == &tile1);
  
  poppedTile = popTile(&stack);
  
  assert(0 == stack.count);
  assert(NULL == poppedTile);
  
  finalizeTiles(&stack);
}


static void tilesIndexTest(void)
{
  struct Tiles index;
  
  initializeTileIndex(&index, compareTilesByCoordinate);
  
  assert(0 == index.capacity);
  assert(0 == index.count);
  assert(index.tiles);
  
  struct Tile tile1 = { .x = 0 };
  addTileToTileIndex(&index, &tile1);
  
  assert(index.capacity > 0);
  assert(1 == index.count);
  assert(index.tiles[0] == &tile1);
  
  struct Tile tile2 = { .x = 1 };
  addTileToTileIndex(&index, &tile2);
  
  assert(2 == index.count);
  assert(index.tiles[1] == &tile2);
  
  struct Tile tile3 = { .x = -1 };
  addTileToTileIndex(&index, &tile3);
  
  assert(3 == index.count);
  assert(index.tiles[0] == &tile3);
  assert(index.tiles[1] == &tile1);
  assert(index.tiles[2] == &tile2);
  
  Boolean found = removeTileFromTileIndex(&index, &tile1);
  assert(found);
  assert(2 == index.count);
  assert(index.tiles[0] == &tile3);
  assert(index.tiles[1] == &tile2);
  
  found = removeTileFromTileIndex(&index, &tile2);
  assert(found);
  assert(1 == index.count);
  assert(index.tiles[0] == &tile3);
  
  found = removeTileFromTileIndex(&index, &tile3);
  assert(found);
  assert(0 == index.count);
  
  finalizeTiles(&index);
}
