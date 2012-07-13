#include "TextGraph.h"

#include "Dungeon.h"
#include "Tile.h"
#include "TileStatistics.h"


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


static void printHorizontalBorder(FILE *out, int x1, int x2);
static void printHorizontalScale(FILE *out, int x1, int x2);
static enum TileType tileTypeAt(struct Tiles const *tiles, int x, int y, int z);


void graphDungeonLevelUsingText(struct Dungeon *dungeon, int z, FILE *out)
{
  struct Tiles const levelTiles = tilesOnLevel(&dungeon->tiles, z);
  struct TileStatistics statistics;
  gatherTileStatistics(&levelTiles, &statistics);

  int x1 = statistics.minX - 1;
  int x2 = statistics.maxX + 2; /* exclusive */
  int y1 = statistics.maxY + 1;
  int y2 = statistics.minY - 2; /* exclusive */

  // top border
  printHorizontalScale(out, x1, x2);
  printHorizontalBorder(out, x1, x2);

  // rows of tiles
  for (int j = y1; j > y2; --j) {
    // top line of row
    fprintf(out, LMARGIN_NUM, j);
    for (int i = x1; i < x2; ++i) {
      enum TileType type = tileTypeAt(&levelTiles, i, j, z);
      enum TileType westType = tileTypeAt(&levelTiles, i - 1, j, z);
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

      enum TileType type = tileTypeAt(&levelTiles, i, j, z);
      enum TileType southType = tileTypeAt(&levelTiles, i, j - 1, z);
      if (x1 == i) {
        if (type == southType) {
          fprintf(out, SolidTileType == type ? CORNER_SOLID : CORNER_EMPTY);
        } else {
          fprintf(out, CORNER_HWALL);
        }
        continue;
      }

      enum TileType westType = tileTypeAt(&levelTiles, i - 1, j, z);
      enum TileType southWestType = tileTypeAt(&levelTiles, i - 1, j - 1, z);
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


static enum TileType tileTypeAt(struct Tiles const *tiles, int x, int y, int z)
{
  struct Tile *tile = findTileInTilesAt(tiles, x, y, z);
  return tile ? tile->type : SolidTileType;
}
