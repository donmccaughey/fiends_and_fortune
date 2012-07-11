#include <assert.h>
#include "Dice.h"
#include "Dungeon.h"
#include "Tile.h"


void dungeonTest(void);
static void generateDungeonTest(void);


void dungeonTest(void)
{
  generateDungeonTest();
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
//  assert(tile == dungeon.mainEntrance->north);
  assert(tile);

  tile = findTileInDungeonAt(&dungeon, -1, -8, 1);
  assert( ! tile);

  finalizeDungeon(&dungeon);
}
