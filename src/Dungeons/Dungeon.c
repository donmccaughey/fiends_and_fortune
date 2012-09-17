#include "Dungeon.h"

#include <assert.h>
#include <string.h>
#include "Areas.h"
#include "Dice.h"
#include "Tile.h"
#include "Tiles.h"
#include "unexpected.h"
#include "Range.h"


enum Direction {
  North, South, East, West,
};


static struct Point advancePoint(struct Point start, int32_t steps, enum Direction direction);
static void addNewEmptyTileToDungeonAt(struct Dungeon *dungeon, int32_t x, int32_t y, int32_t z);
static void addNewEmptyTileToDungeonAtPoint(struct Dungeon *dungeon, struct Point point);
static struct Point chamber(struct Dungeon *dungeon, struct Point fromPoint, uint32_t length, uint32_t width, enum Direction direction, uint32_t leftOffset);
static struct Point hallway(struct Dungeon *dungeon, struct Point fromPoint, uint32_t distance, enum Direction direction);


static struct Point advancePoint(struct Point start, int32_t steps, enum Direction direction) {
  switch (direction) {
    case North: return makePoint(start.x, start.y + steps, start.z);
    case South: return makePoint(start.x, start.y - steps, start.z);
    case East: return makePoint(start.x + steps, start.y, start.z);
    case West: return makePoint(start.x - steps, start.y, start.z);
    default: UNEXPECTED("Unrecognized direction %i", direction); break;
  }
  return start;
}


static void addNewEmptyTileToDungeonAt(struct Dungeon *dungeon, int32_t x, int32_t y, int32_t z)
{
  assert(NULL == findTileInTilesAt(dungeon->tiles, x, y, z));

  struct Tile *tile = createTile(makePoint(x, y, z), EmptyTileType);
  addTileToTiles(dungeon->tiles, tile);
}


static void addNewEmptyTileToDungeonAtPoint(struct Dungeon *dungeon, struct Point point)
{
  addNewEmptyTileToDungeonAt(dungeon, point.x, point.y, point.z);
}


static struct Point chamber(struct Dungeon *dungeon, struct Point fromPoint, uint32_t length, uint32_t width, enum Direction direction, uint32_t leftOffset)
{
  assert(leftOffset < width);
  struct Range xRange;
  struct Range yRange;

  switch (direction) {
    case North:
      xRange = makeRange(fromPoint.x - leftOffset, fromPoint.x + width - leftOffset);
      yRange = makeRange(fromPoint.y, fromPoint.y + length);
      break;
    case South: {
        struct ReverseRange xReverseRange = makeReverseRange(fromPoint.x + leftOffset,
                                                             fromPoint.x - width + leftOffset);
        struct ReverseRange yReverseRange = makeReverseRange(fromPoint.y, fromPoint.y - length);
        xRange = makeRangeFromReverseRange(xReverseRange);
        yRange = makeRangeFromReverseRange(yReverseRange);
      }
      break;
    case East: {
        struct ReverseRange yReverseRange = makeReverseRange(fromPoint.y + leftOffset,
                                                             fromPoint.y - width + leftOffset);
        xRange = makeRange(fromPoint.x, fromPoint.x + length);
        yRange = makeRangeFromReverseRange(yReverseRange);
      }
      break;
    case West: {
        struct ReverseRange xReverseRange = makeReverseRange(fromPoint.x, fromPoint.x - length);
        xRange = makeRangeFromReverseRange(xReverseRange);
        yRange = makeRange(fromPoint.y - leftOffset, fromPoint.y + width - leftOffset);
      }
      break;
    default:
      UNEXPECTED("Unrecognized direction %i", direction);
      return fromPoint;
  }

  for (int32_t j = yRange.begin; j < yRange.end; ++j) {
    for (int32_t i = xRange.begin; i < xRange.end; ++i) {
      addNewEmptyTileToDungeonAt(dungeon, i, j, fromPoint.z);
    }
  }

  return advancePoint(fromPoint, length, direction);
}


void finalizeDungeon(struct Dungeon *dungeon)
{
  destroyAreas(dungeon->areas);
  destroyTiles(dungeon->tiles);
}


void generateDungeon(struct Dungeon *dungeon, struct Dice *dice)
{
  generateSmallDungeon(dungeon);
}


void generateSmallDungeon(struct Dungeon *dungeon)
{
  struct Point point = makePoint(0, 0, 1);

  point = hallway(dungeon, point, 2, North);

  point = chamber(dungeon, point, 5, 3, North, 1);

  /* chamber exits */
  struct Point pointInChamber = advancePoint(point, 2, South);
  struct Point northWestExit = advancePoint(pointInChamber, 2, West);
  struct Point southWestExit = advancePoint(northWestExit, 2, South);
  struct Point northEastExit = advancePoint(pointInChamber, 2, East);
  struct Point southEastExit = advancePoint(northEastExit, 2, South);

  point = hallway(dungeon, point, 7, North);
  point = hallway(dungeon, point, 8, East);
  point = hallway(dungeon, point, 4, South);

  point = chamber(dungeon, point, 3, 4, South, 0);
  point = advancePoint(point, 3, West);

  point = hallway(dungeon, point, 2, South);
  point = hallway(dungeon, point, 4, West);

  /* from entry chamber, north west exit */
  point = hallway(dungeon, northWestExit, 1, West);
  point = hallway(dungeon, point, 7, North);

  point = chamber(dungeon, point, 3, 2, North, 1);

  /* from entry chamber, south west exit */
  point = hallway(dungeon, southWestExit, 1, West);
  point = hallway(dungeon, point, 2, South);
  point = hallway(dungeon, point, 2, West);
  point = hallway(dungeon, point, 3, North);

  point = chamber(dungeon, point, 2, 2, North, 1);

  point = advancePoint(point, 1, West);
  point = hallway(dungeon, point, 3, North);

  point = chamber(dungeon, point, 2, 3, North, 1);

  /* from entry chamber, south east exit */
  point = hallway(dungeon, southEastExit, 1, East);

  chamber(dungeon, point, 6, 4, East, 0);
  struct Tile *tile = findTileInTilesAt(dungeon->tiles, 5, 2, 1);
  removeTileFromTiles(dungeon->tiles, tile);
}


static struct Point hallway(struct Dungeon *dungeon, struct Point fromPoint, uint32_t distance, enum Direction direction)
{
  return chamber(dungeon, fromPoint, distance, 1, direction, 0);
}


void initializeDungeon(struct Dungeon *dungeon)
{
  memset(dungeon, 0, sizeof(struct Dungeon));
  dungeon->areas = createAreas();
  dungeon->tiles = createTiles();
}
