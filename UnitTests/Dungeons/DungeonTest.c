#include <assert.h>
#include "Dice.h"
#include "Dungeon.h"


void dungeonTest(void);
static void generateDungeonTest(void);
static void pushAndPopTileTest(void);
static void tilesIndexTest(void);


void dungeonTest(void)
{
  generateDungeonTest();
  pushAndPopTileTest();
  tilesIndexTest();
}


static void generateDungeonTest(void)
{
  struct Dice dice;
  initializeDice(&dice);

  struct Dungeon dungeon;
  initializeDungeon(&dungeon);

  generateDungeon(&dungeon, &dice);
  assert(dungeon.mainEntrance);
//  printf("dungeon has %lu tiles\n", dungeon.index.count);
  assert(113 == dungeon.index.count);

  struct DungeonStatistics statistics;
  gatherDungeonStatistics(&dungeon, &statistics);

  assert(-7 == statistics.minX);
  assert(8 == statistics.maxX);
  assert(0 == statistics.minY);
  assert(14 == statistics.maxY);
  assert(113 == statistics.tileCount);

  struct Tile *tile = findTileInDungeonAt(&dungeon, 0, 0, 1);
  assert(tile == dungeon.mainEntrance);

  tile = findTileInDungeonAt(&dungeon, 0, 1, 1);
  assert(tile == dungeon.mainEntrance->north);

  tile = findTileInDungeonAt(&dungeon, -1, -8, 1);
  assert( ! tile);

  finalizeDungeon(&dungeon);
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
