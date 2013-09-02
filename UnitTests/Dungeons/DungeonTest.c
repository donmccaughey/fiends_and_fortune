#include <assert.h>
#include "Dice.h"
#include "Dungeon.h"
#include "Tile.h"
#include "Tiles.h"


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
  assert(113 == tilesCount(dungeon.tiles));

  assert(equalRanges(makeRange(-7, 9), xRangeOfTiles(dungeon.tiles)));
  assert(equalRanges(makeRange(0, 15), yRangeOfTiles(dungeon.tiles)));
  assert(113 == tilesCount(dungeon.tiles));

  struct Tile *tile = findTileInTilesAt(dungeon.tiles, makePoint(0, 0, 1));
  assert(tile);

  tile = findTileInTilesAt(dungeon.tiles, makePoint(0, 1, 1));
  assert(tile);

  tile = findTileInTilesAt(dungeon.tiles, makePoint(-1, -8, 1));
  assert( ! tile);

  finalizeDungeon(&dungeon);
}
