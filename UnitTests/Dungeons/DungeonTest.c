#include <assert.h>
#include "Dice.h"
#include "Dungeon.h"
#include "TileStatistics.h"
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
  assert(113 == dungeon.tiles.count);

  struct TileStatistics statistics;
  gatherTileStatistics(&dungeon.tiles, &statistics);

  assert(-7 == statistics.xRange.begin);
  assert(9 == statistics.xRange.end);
  assert(0 == statistics.yRange.begin);
  assert(15 == statistics.yRange.end);
  assert(113 == statistics.count);

  struct Tile *tile = findTileInTilesAt(&dungeon.tiles, 0, 0, 1);
  assert(tile);

  tile = findTileInTilesAt(&dungeon.tiles, 0, 1, 1);
  assert(tile);

  tile = findTileInTilesAt(&dungeon.tiles, -1, -8, 1);
  assert( ! tile);

  finalizeDungeon(&dungeon);
}
