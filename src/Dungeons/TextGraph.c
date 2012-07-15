#include "TextGraph.h"

#include "Dungeon.h"
#include "Range.h"
#include "Tile.h"
#include "Tiles.h"
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


static void printHorizontalBorder(FILE *out, struct Range range);
static void printHorizontalScale(FILE *out, struct Range range);
static enum TileType tileTypeAt(struct Tiles const *tiles, int32_t x, int32_t y, int32_t z);


void graphDungeonLevelUsingText(struct Dungeon *dungeon, int32_t z, FILE *out)
{
  struct Tiles *levelTiles = tilesOnLevel(dungeon->tiles, z);
  struct TileStatistics statistics;
  gatherTileStatistics(levelTiles, &statistics);

  struct Range const xRange = expandRange(statistics.xRange, 1);
  struct Range const yRange = expandRange(statistics.yRange, 1);
  struct ReverseRange const yReverseRange = makeReverseRangeFromRange(yRange);

  // top border
  printHorizontalScale(out, xRange);
  printHorizontalBorder(out, xRange);

  // rows of tiles
  for (int32_t j = yReverseRange.top; j > yReverseRange.bottom; --j) {
    // top line of row
    fprintf(out, LMARGIN_NUM, j);
    for (int32_t i = xRange.begin; i < xRange.end; ++i) {
      enum TileType type = tileTypeAt(levelTiles, i, j, z);
      enum TileType westType = tileTypeAt(levelTiles, i - 1, j, z);
      if (xRange.begin == i || type != westType) {
        fprintf(out, SolidTileType == type ? VWALL_SOLID : VWALL_EMPTY);
      } else {
        fprintf(out, SolidTileType == type ? SOLID : EMPTY);
      }
    }
    fprintf(out, RMARGIN_NUM, j);

    // bottom line of row
    fprintf(out, LMARGIN);
    for (int32_t i = xRange.begin; i < xRange.end; ++i) {
      if (j == yReverseRange.bottom + 1) {
        fprintf(out, CORNER_HWALL);
        continue;
      }

      enum TileType type = tileTypeAt(levelTiles, i, j, z);
      enum TileType southType = tileTypeAt(levelTiles, i, j - 1, z);
      if (xRange.begin == i) {
        if (type == southType) {
          fprintf(out, SolidTileType == type ? CORNER_SOLID : CORNER_EMPTY);
        } else {
          fprintf(out, CORNER_HWALL);
        }
        continue;
      }

      enum TileType westType = tileTypeAt(levelTiles, i - 1, j, z);
      enum TileType southWestType = tileTypeAt(levelTiles, i - 1, j - 1, z);
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

  printHorizontalScale(out, xRange);
  destroyTiles(levelTiles);
}


static void printHorizontalBorder(FILE *out, struct Range range)
{
  fprintf(out, LMARGIN);
  for (int32_t i = range.begin; i < range.end; ++i) {
    fprintf(out, "+---");
  }
  fprintf(out, "+   \n");
}


static void printHorizontalScale(FILE *out, struct Range range)
{
  fprintf(out, LMARGIN);
  for (int32_t i = range.begin; i < range.end; ++i) {
    fprintf(out, "%3i ", i);
  }
  fprintf(out, RMARGIN "\n");
}


static enum TileType tileTypeAt(struct Tiles const *tiles, int32_t x, int32_t y, int32_t z)
{
  struct Tile *tile = findTileInTilesAt(tiles, x, y, z);
  return tile ? tile->type : SolidTileType;
}
