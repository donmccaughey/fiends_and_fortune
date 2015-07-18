#include "Dungeon.h"

#include <assert.h>
#include <string.h>

#include "alloc_or_die.h"
#include "Area.h"
#include "Areas.h"
#include "Dice.h"
#include "Direction.h"
#include "Tile.h"
#include "Tiles.h"
#include "unexpected.h"


static struct Point advancePoint(struct Point start, int32_t steps, enum Direction direction);
static void addNewEmptyTileToDungeonAt(struct Dungeon *dungeon, struct Area *area, int32_t x, int32_t y, int32_t z);
static struct Point area(struct Dungeon *dungeon, struct Point fromPoint, uint32_t length, uint32_t width, enum Direction direction, uint32_t leftOffset, enum AreaType areaType);
static struct Point chamber(struct Dungeon *dungeon, struct Point fromPoint, uint32_t length, uint32_t width, enum Direction direction, uint32_t leftOffset);
static struct Point passage(struct Dungeon *dungeon, struct Point fromPoint, uint32_t distance, enum Direction direction);


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


static void addNewEmptyTileToDungeonAt(struct Dungeon *dungeon, struct Area *area, int32_t x, int32_t y, int32_t z)
{
  assert(NULL == findTileInTilesAt(dungeon->tiles, makePoint(x, y, z)));

  struct Tile *tile = createTile(makePoint(x, y, z), EmptyTileType);
  addTileToTiles(area->tiles, tile);
}


static struct Point area(struct Dungeon *dungeon, struct Point fromPoint, uint32_t length, uint32_t width, enum Direction direction, uint32_t leftOffset, enum AreaType areaType)
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

  char *description;
  switch (areaType) {
    case ChamberAreaType:
      asprintf_or_die(&description, "%u' x %u' chamber",
                      length * 10, width * 10);
      break;
    case PassageAreaType:
      asprintf_or_die(&description, "%u' passage %s",
                      length * 10, directionName(direction));
      break;
    default:
      asprintf_or_die(&description, "%u' x %u' area", length * 10, width * 10);
      break;
  }

  struct Area *area = createArea(description, dungeon->tiles, areaType);
  addAreaToAreas(dungeon->areas, area);

  for (int32_t j = yRange.begin; j < yRange.end; ++j) {
    for (int32_t i = xRange.begin; i < xRange.end; ++i) {
      addNewEmptyTileToDungeonAt(dungeon, area, i, j, fromPoint.z);
    }
  }

  return advancePoint(fromPoint, length, direction);
}


static struct Point chamber(struct Dungeon *dungeon, struct Point fromPoint, uint32_t length, uint32_t width, enum Direction direction, uint32_t leftOffset)
{
  return area(dungeon, fromPoint, length, width, direction, leftOffset, ChamberAreaType);
}


char const **dungeonAreaDescriptions(struct Dungeon *dungeon)
{
  size_t descriptionsCount = areasCount(dungeon->areas);
  char const **descriptions = calloc_or_die(descriptionsCount + 1,
                                            sizeof(char const *));

  for (size_t i = 0; i < descriptionsCount; ++i) {
    descriptions[i] = areaInAreasAtIndex(dungeon->areas, i)->description;
  }
  return descriptions;
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

  point = passage(dungeon, point, 2, North);

  point = chamber(dungeon, point, 5, 3, North, 1);

  /* chamber exits */
  struct Point pointInChamber = advancePoint(point, 2, South);
  struct Point northWestExit = advancePoint(pointInChamber, 2, West);
  struct Point southWestExit = advancePoint(northWestExit, 2, South);
  struct Point northEastExit = advancePoint(pointInChamber, 2, East);
  struct Point southEastExit = advancePoint(northEastExit, 2, South);

  point = passage(dungeon, point, 7, North);
  point = passage(dungeon, point, 8, East);
  point = passage(dungeon, point, 4, South);

  point = chamber(dungeon, point, 3, 4, South, 0);
  point = advancePoint(point, 3, West);

  point = passage(dungeon, point, 2, South);
  passage(dungeon, point, 4, West);

  /* from entry chamber, north west exit */
  point = passage(dungeon, northWestExit, 1, West);
  point = passage(dungeon, point, 7, North);

  chamber(dungeon, point, 3, 2, North, 1);

  /* from entry chamber, south west exit */
  point = passage(dungeon, southWestExit, 1, West);
  point = passage(dungeon, point, 2, South);
  point = passage(dungeon, point, 2, West);
  point = passage(dungeon, point, 3, North);

  point = chamber(dungeon, point, 2, 2, North, 1);

  point = advancePoint(point, 1, West);
  point = passage(dungeon, point, 3, North);

  chamber(dungeon, point, 2, 3, North, 1);

  /* from entry chamber, south east exit */
  point = passage(dungeon, southEastExit, 1, East);

  chamber(dungeon, point, 6, 4, East, 0);
  struct Tile *tile = findTileInTilesAt(dungeon->tiles, makePoint(5, 2, 1));
  removeTileFromTiles(dungeon->tiles, tile);
}


static struct Point passage(struct Dungeon *dungeon, struct Point fromPoint, uint32_t distance, enum Direction direction)
{
  return area(dungeon, fromPoint, distance, 1, direction, 0, PassageAreaType);
}


void initializeDungeon(struct Dungeon *dungeon)
{
  memset(dungeon, 0, sizeof(struct Dungeon));
  dungeon->areas = createAreas();
  dungeon->tiles = createTiles();
}
