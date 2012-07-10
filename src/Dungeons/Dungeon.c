#include "Dungeon.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "Dice.h"
#include "heap.h"
#include "Tile.h"


#define CORNER_EMPTY "+   "
#define CORNER_HWALL "+---"
#define CORNER_SOLID "+:::"
#define EMPTY "    "
#define EMPTY_SPAN ".   "
#define HWALL "----"
#define LMARGIN "    "
#define LMARGIN_NUM "%3i "
#define RMARGIN "    "
#define RMARGIN_NUM "| %-3i\n"
#define SOLID "::::"
#define VWALL_EMPTY "|   "
#define VWALL_SOLID "|:::"


static struct Tile *addNewEmptyTileToDungeonAt(struct Dungeon *dungeon, int x, int y, int z);
static struct Tile *createTile(enum TileType type, int x, int y, int z);
static Boolean freeTileOnVisit(struct Tile *tile, void *unused);
static Boolean gatherStatisticsOnVisit(struct Tile *tile, void *statisticsContext);
static void printHorizontalBorder(FILE *out, int x1, int x2);
static void printHorizontalScale(FILE *out, int x1, int x2);
static enum TileType tileTypeAt(struct Dungeon *dungeon, int x, int y, int z);


static struct Tile *addNewEmptyTileToDungeonAt(struct Dungeon *dungeon, int x, int y, int z)
{
  struct Tile *tile = createTile(EmptyTileType, x, y, z);
  addTileToDungeon(dungeon, tile);
  return tile;
}


void addTileToDungeon(struct Dungeon *dungeon, struct Tile *tile)
{
  assert(NULL == findTileInDungeonAt(dungeon, tile->x, tile->y, tile->z));

  struct Tile *north = findTileInDungeonAt(dungeon, tile->x, tile->y + 1, tile->z);
  if (north) {
    assert( ! north->south);
    tile->north = north;
    north->south = tile;
  }
  
  struct Tile *south = findTileInDungeonAt(dungeon, tile->x, tile->y - 1, tile->z);
  if (south) {
    assert( ! south->north);
    tile->south = south;
    south->north = tile;
  }
  
  struct Tile *east = findTileInDungeonAt(dungeon, tile->x + 1, tile->y, tile->z);
  if (east) {
    assert( ! east->west);
    tile->east = east;
    east->west = tile;
  }
  
  struct Tile *west = findTileInDungeonAt(dungeon, tile->x - 1, tile->y, tile->z);
  if (west) {
    assert( ! west->east);
    tile->west = west;
    west->east = tile;
  }

  addTileToTileIndex(&dungeon->index, tile);
}


int compareTilesByCoordinate(void const *item1, void const *item2)
{
  struct Tile *const *pointer1 = item1;
  struct Tile *const *pointer2 = item2;
  
  struct Tile *tile1 = *pointer1;
  struct Tile *tile2 = *pointer2;
  
  if (tile1->z != tile2->z) {
    return tile1->z - tile2->z;
  }
  if (tile1->y != tile2->y) {
    return tile1->y - tile2->y;
  }
  return tile1->x - tile2->x;
}


static struct Tile *createTile(enum TileType type, int x, int y, int z)
{
  struct Tile *tile = CALLOC_OR_DIE(1, sizeof(struct Tile));
  tile->type = type;
  tile->x = x;
  tile->y = y;
  tile->z = z;
  return tile;
}


void destroyTileGraph(struct Tile *tile)
{
  if ( ! tile) {
    return;
  }  
  visitTiles(tile, NULL, freeTileOnVisit);
}


void finalizeDungeon(struct Dungeon *dungeon)
{
  destroyTileGraph(dungeon->mainEntrance);
  finalizeTiles(&dungeon->index);
}


struct Tile *findTileInDungeonAt(struct Dungeon *dungeon, int x, int y, int z)
{
  struct Tile equivalent = { .x = x, .y = y, .z = z };
  struct Tile *tile = &equivalent;

  struct Tile **tileInIndex = bsearch(&tile, dungeon->index.tiles, dungeon->index.count, sizeof(struct Tile *), dungeon->index.compare);
  return tileInIndex ? *tileInIndex : NULL;
}


static Boolean freeTileOnVisit(struct Tile *tile, void *unused)
{
  free(tile);
  return FALSE;
}


static Boolean gatherStatisticsOnVisit(struct Tile *tile, void *statisticsContext)
{
  struct DungeonStatistics *statistics = statisticsContext;
  
  if (tile->x < statistics->minX) {
    statistics->minX = tile->x;
  }
  if (tile->x > statistics->maxX) {
    statistics->maxX = tile->x;
  }
  if (tile->y < statistics->minY) {
    statistics->minY = tile->y;
  }
  if (tile->y > statistics->maxY) {
    statistics->maxY = tile->y;
  }
  
  ++statistics->tileCount;
  
  return FALSE;
}


void gatherDungeonStatistics(struct Dungeon *dungeon, struct DungeonStatistics *statistics)
{
  memset(statistics, 0, sizeof(struct DungeonStatistics));
  visitTiles(dungeon->mainEntrance, statistics, gatherStatisticsOnVisit);
}


void generateDungeon(struct Dungeon *dungeon, struct Dice *dice)
{
  dungeon->mainEntrance = addNewEmptyTileToDungeonAt(dungeon, 0, 0, 1);
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


void graphDungeonUsingText(struct Dungeon *dungeon, FILE *out)
{
  struct DungeonStatistics statistics;
  gatherDungeonStatistics(dungeon, &statistics);
  
  int x1 = statistics.minX - 1;
  int x2 = statistics.maxX + 2;
  int y1 = statistics.maxY + 1;
  int y2 = statistics.minY - 2;
  int z = dungeon->mainEntrance->z;
  
  // top border
  printHorizontalScale(out, x1, x2);
  printHorizontalBorder(out, x1, x2);

  // rows of tiles
  for (int j = y1; j > y2; --j) {
    // top line of row
    fprintf(out, LMARGIN_NUM, j);
    for (int i = x1; i < x2; ++i) {
      enum TileType type = tileTypeAt(dungeon, i, j, z);
      enum TileType westType = tileTypeAt(dungeon, i - 1, j, z);
      if (x1 == i || type != westType) {
        fprintf(out, SolidTileType == type ? VWALL_SOLID : VWALL_EMPTY);
      } else {
        fprintf(out, SolidTileType == type ? SOLID : EMPTY);
      }
    }
    fprintf(out, RMARGIN_NUM, j);

    // bottom line of row
    fprintf(out, LMARGIN);
    for (int i = x1; i < x2; ++i) {
      if (j == y2 + 1) {
        fprintf(out, CORNER_HWALL);
        continue;
      }
      
      enum TileType type = tileTypeAt(dungeon, i, j, z);
      enum TileType southType = tileTypeAt(dungeon, i, j - 1, z);
      if (x1 == i) {
        if (type == southType) {
          fprintf(out, SolidTileType == type ? CORNER_SOLID : CORNER_EMPTY);
        } else {
          fprintf(out, CORNER_HWALL);
        }
        continue;
      }

      enum TileType westType = tileTypeAt(dungeon, i - 1, j, z);
      enum TileType southWestType = tileTypeAt(dungeon, i - 1, j - 1, z);
      if (type == southType) {
        if (type == westType) {
          if (type == southWestType) {
            fprintf(out, SolidTileType == type ? SOLID : EMPTY_SPAN);
          } else {
            fprintf(out, SolidTileType == type ? CORNER_SOLID : CORNER_EMPTY);
          }
        } else {
          if (type == southWestType) {
            fprintf(out, SolidTileType == type ? CORNER_SOLID : CORNER_EMPTY);
          } else {
            fprintf(out, SolidTileType == type ? VWALL_SOLID : VWALL_EMPTY);
          }
        }
      } else {
        if (type == westType) {
          if (type == southWestType) {
            fprintf(out, CORNER_HWALL);
          } else {
            fprintf(out, HWALL);
          }
        } else {
          fprintf(out, CORNER_HWALL);
        }
      }
    }
    fprintf(out, "+\n");
  }

  printHorizontalScale(out, x1, x2);
}


void initializeDungeon(struct Dungeon *dungeon)
{
  memset(dungeon, 0, sizeof(struct Dungeon));
  initializeTileIndex(&dungeon->index, compareTilesByCoordinate);
}


static void printHorizontalBorder(FILE *out, int x1, int x2)
{
  fprintf(out, LMARGIN);
  for (int i = x1; i < x2; ++i) {
    fprintf(out, "+---");
  }
  fprintf(out, "+   \n");
}


static void printHorizontalScale(FILE *out, int x1, int x2)
{
  fprintf(out, LMARGIN);
  for (int i = x1; i < x2; ++i) {
    fprintf(out, "%3i ", i);
  }
  fprintf(out, RMARGIN "\n");
}


static enum TileType tileTypeAt(struct Dungeon *dungeon, int x, int y, int z)
{
  struct Tile *tile = findTileInDungeonAt(dungeon, x, y, z);
  return tile ? tile->type : SolidTileType;
}
