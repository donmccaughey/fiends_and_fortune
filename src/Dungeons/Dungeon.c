#include "Dungeon.h"

#include <assert.h>
#include <string.h>
#include "Dice.h"
#include "Tile.h"
#include "Tiles.h"


static void addNewEmptyTileToDungeonAt(struct Dungeon *dungeon, int32_t x, int32_t y, int32_t z);


static void addNewEmptyTileToDungeonAt(struct Dungeon *dungeon, int32_t x, int32_t y, int32_t z)
{
  assert(NULL == findTileInTilesAt(dungeon->tiles, x, y, z));

  struct Tile tile = {
    .point = { x, y, z },
    .type = EmptyTileType,
  };
  addTileToTiles(dungeon->tiles, &tile);
}


void finalizeDungeon(struct Dungeon *dungeon)
{
  destroyTiles(dungeon->tiles);
}


void generateDungeon(struct Dungeon *dungeon, struct Dice *dice)
{
  addNewEmptyTileToDungeonAt(dungeon, 0, 0, 1);
  addNewEmptyTileToDungeonAt(dungeon, 0, 1, 1);

  /* entry chamber */
  addNewEmptyTileToDungeonAt(dungeon, -1, 2, 1);
  addNewEmptyTileToDungeonAt(dungeon, 0, 2, 1);
  addNewEmptyTileToDungeonAt(dungeon, 1, 2, 1);

  addNewEmptyTileToDungeonAt(dungeon, -2, 3, 1);
  addNewEmptyTileToDungeonAt(dungeon, -1, 3, 1);
  addNewEmptyTileToDungeonAt(dungeon, 0, 3, 1);
  addNewEmptyTileToDungeonAt(dungeon, 1, 3, 1);
  addNewEmptyTileToDungeonAt(dungeon, 2, 3, 1);

  addNewEmptyTileToDungeonAt(dungeon, -1, 4, 1);
  addNewEmptyTileToDungeonAt(dungeon, 0, 4, 1);
  addNewEmptyTileToDungeonAt(dungeon, 1, 4, 1);

  addNewEmptyTileToDungeonAt(dungeon, -2, 5, 1);
  addNewEmptyTileToDungeonAt(dungeon, -1, 5, 1);
  addNewEmptyTileToDungeonAt(dungeon, 0, 5, 1);
  addNewEmptyTileToDungeonAt(dungeon, 1, 5, 1);
  addNewEmptyTileToDungeonAt(dungeon, 2, 5, 1);

  addNewEmptyTileToDungeonAt(dungeon, -1, 6, 1);
  addNewEmptyTileToDungeonAt(dungeon, 0, 6, 1);
  addNewEmptyTileToDungeonAt(dungeon, 1, 6, 1);

  addNewEmptyTileToDungeonAt(dungeon, 0, 7, 1);

  /* entry chamber exit north, 60 ft corridor */
  addNewEmptyTileToDungeonAt(dungeon, 0, 8, 1);
  addNewEmptyTileToDungeonAt(dungeon, 0, 9, 1);
  addNewEmptyTileToDungeonAt(dungeon, 0, 10, 1);
  addNewEmptyTileToDungeonAt(dungeon, 0, 11, 1);
  addNewEmptyTileToDungeonAt(dungeon, 0, 12, 1);
  addNewEmptyTileToDungeonAt(dungeon, 0, 13, 1);

  /* right turn */
  addNewEmptyTileToDungeonAt(dungeon, 0, 14, 1);
  addNewEmptyTileToDungeonAt(dungeon, 1, 14, 1);

  /* 60 ft corridor east */
  addNewEmptyTileToDungeonAt(dungeon, 2, 14, 1);
  addNewEmptyTileToDungeonAt(dungeon, 3, 14, 1);
  addNewEmptyTileToDungeonAt(dungeon, 4, 14, 1);
  addNewEmptyTileToDungeonAt(dungeon, 5, 14, 1);
  addNewEmptyTileToDungeonAt(dungeon, 6, 14, 1);
  addNewEmptyTileToDungeonAt(dungeon, 7, 14, 1);

  /* right turn */
  addNewEmptyTileToDungeonAt(dungeon, 8, 14, 1);
  addNewEmptyTileToDungeonAt(dungeon, 8, 13, 1);
  addNewEmptyTileToDungeonAt(dungeon, 8, 12, 1);

  /* chamber */
  addNewEmptyTileToDungeonAt(dungeon, 8, 11, 1);

  addNewEmptyTileToDungeonAt(dungeon, 5, 10, 1);
  addNewEmptyTileToDungeonAt(dungeon, 6, 10, 1);
  addNewEmptyTileToDungeonAt(dungeon, 7, 10, 1);
  addNewEmptyTileToDungeonAt(dungeon, 8, 10, 1);

  addNewEmptyTileToDungeonAt(dungeon, 5, 9, 1);
  addNewEmptyTileToDungeonAt(dungeon, 6, 9, 1);
  addNewEmptyTileToDungeonAt(dungeon, 7, 9, 1);
  addNewEmptyTileToDungeonAt(dungeon, 8, 9, 1);

  addNewEmptyTileToDungeonAt(dungeon, 5, 8, 1);
  addNewEmptyTileToDungeonAt(dungeon, 6, 8, 1);
  addNewEmptyTileToDungeonAt(dungeon, 7, 8, 1);
  addNewEmptyTileToDungeonAt(dungeon, 8, 8, 1);

  addNewEmptyTileToDungeonAt(dungeon, 5, 7, 1);

  /* corridor */
  addNewEmptyTileToDungeonAt(dungeon, 5, 6, 1);

  /* right turn */
  addNewEmptyTileToDungeonAt(dungeon, 4, 5, 1);
  addNewEmptyTileToDungeonAt(dungeon, 5, 5, 1);

  /* corridor */
  addNewEmptyTileToDungeonAt(dungeon, 3, 5, 1);


  /* entry chamber, exit north-most west, right turn */
  addNewEmptyTileToDungeonAt(dungeon, -3, 6, 1);
  addNewEmptyTileToDungeonAt(dungeon, -3, 5, 1);

  /* 40 ft corridor north */
  addNewEmptyTileToDungeonAt(dungeon, -3, 7, 1);
  addNewEmptyTileToDungeonAt(dungeon, -3, 8, 1);
  addNewEmptyTileToDungeonAt(dungeon, -3, 9, 1);
  addNewEmptyTileToDungeonAt(dungeon, -3, 10, 1);

  /* chamber */
  addNewEmptyTileToDungeonAt(dungeon, -3, 11, 1);

  addNewEmptyTileToDungeonAt(dungeon, -4, 12, 1);
  addNewEmptyTileToDungeonAt(dungeon, -3, 12, 1);

  addNewEmptyTileToDungeonAt(dungeon, -4, 13, 1);
  addNewEmptyTileToDungeonAt(dungeon, -3, 13, 1);

  addNewEmptyTileToDungeonAt(dungeon, -4, 14, 1);
  addNewEmptyTileToDungeonAt(dungeon, -3, 14, 1);


  /* entry chamber, exit south-most west, left turn */
  addNewEmptyTileToDungeonAt(dungeon, -3, 3, 1);
  addNewEmptyTileToDungeonAt(dungeon, -3, 2, 1);

  /* right turn */
  addNewEmptyTileToDungeonAt(dungeon, -5, 1, 1);
  addNewEmptyTileToDungeonAt(dungeon, -4, 1, 1);
  addNewEmptyTileToDungeonAt(dungeon, -3, 1, 1);
  
  /* right turn */
  addNewEmptyTileToDungeonAt(dungeon, -5, 2, 1);
  addNewEmptyTileToDungeonAt(dungeon, -5, 3, 1);

  addNewEmptyTileToDungeonAt(dungeon, -6, 4, 1);
  addNewEmptyTileToDungeonAt(dungeon, -5, 4, 1);

  addNewEmptyTileToDungeonAt(dungeon, -6, 5, 1);
  addNewEmptyTileToDungeonAt(dungeon, -5, 5, 1);

  addNewEmptyTileToDungeonAt(dungeon, -6, 6, 1);

  /* corridor north */
  addNewEmptyTileToDungeonAt(dungeon, -6, 7, 1);
  addNewEmptyTileToDungeonAt(dungeon, -6, 8, 1);

  /* chamber */
  addNewEmptyTileToDungeonAt(dungeon, -7, 9, 1);
  addNewEmptyTileToDungeonAt(dungeon, -6, 9, 1);
  addNewEmptyTileToDungeonAt(dungeon, -5, 9, 1);

  addNewEmptyTileToDungeonAt(dungeon, -7, 10, 1);
  addNewEmptyTileToDungeonAt(dungeon, -6, 10, 1);
  addNewEmptyTileToDungeonAt(dungeon, -5, 10, 1);


  /* entry chamber, exit south-most east, chamber */
  addNewEmptyTileToDungeonAt(dungeon, 3, 3, 1);
  addNewEmptyTileToDungeonAt(dungeon, 3, 2, 1);
  addNewEmptyTileToDungeonAt(dungeon, 3, 1, 1);
  addNewEmptyTileToDungeonAt(dungeon, 3, 0, 1);

  addNewEmptyTileToDungeonAt(dungeon, 4, 3, 1);
  addNewEmptyTileToDungeonAt(dungeon, 4, 2, 1);
  addNewEmptyTileToDungeonAt(dungeon, 4, 1, 1);
  addNewEmptyTileToDungeonAt(dungeon, 4, 0, 1);

  addNewEmptyTileToDungeonAt(dungeon, 5, 3, 1);

  addNewEmptyTileToDungeonAt(dungeon, 5, 1, 1);
  addNewEmptyTileToDungeonAt(dungeon, 5, 0, 1);

  addNewEmptyTileToDungeonAt(dungeon, 6, 3, 1);
  addNewEmptyTileToDungeonAt(dungeon, 6, 2, 1);
  addNewEmptyTileToDungeonAt(dungeon, 6, 1, 1);
  addNewEmptyTileToDungeonAt(dungeon, 6, 0, 1);

  addNewEmptyTileToDungeonAt(dungeon, 7, 3, 1);
  addNewEmptyTileToDungeonAt(dungeon, 7, 2, 1);
  addNewEmptyTileToDungeonAt(dungeon, 7, 1, 1);
  addNewEmptyTileToDungeonAt(dungeon, 7, 0, 1);

  addNewEmptyTileToDungeonAt(dungeon, 8, 3, 1);
  addNewEmptyTileToDungeonAt(dungeon, 8, 2, 1);
  addNewEmptyTileToDungeonAt(dungeon, 8, 1, 1);
  addNewEmptyTileToDungeonAt(dungeon, 8, 0, 1);
}


void initializeDungeon(struct Dungeon *dungeon)
{
  memset(dungeon, 0, sizeof(struct Dungeon));
  dungeon->tiles = createTiles();
}
