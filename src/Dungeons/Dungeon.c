#include "Dungeon.h"

#include <assert.h>
#include <string.h>

#include "common/alloc_or_die.h"
#include "common/fail.h"
#include "common/str.h"

#include "area.h"
#include "areas.h"
#include "direction.h"
#include "range.h"
#include "reverse_range.h"
#include "tile.h"
#include "tiles.h"


static struct point advancePoint(struct point start, int32_t steps, enum direction direction);
static void addNewEmptyTileToDungeonAt(struct Dungeon *dungeon, struct area *area, int32_t x, int32_t y, int32_t z);
static struct point area(struct Dungeon *dungeon, struct point fromPoint, uint32_t length, uint32_t width, enum direction direction, uint32_t leftOffset, enum area_type areaType);
static struct point chamber(struct Dungeon *dungeon, struct point fromPoint, uint32_t length, uint32_t width, enum direction direction, uint32_t leftOffset);
static struct point passage(struct Dungeon *dungeon, struct point fromPoint, uint32_t distance, enum direction direction);


static struct point advancePoint(struct point start, int32_t steps, enum direction direction) {
    switch (direction) {
        case North: return point_make(start.x, start.y + steps, start.z);
        case South: return point_make(start.x, start.y - steps, start.z);
        case East: return point_make(start.x + steps, start.y, start.z);
        case West: return point_make(start.x - steps, start.y, start.z);
        default: fail("Unrecognized direction %i", direction); break;
    }
    return start;
}


static void addNewEmptyTileToDungeonAt(struct Dungeon *dungeon, struct area *area, int32_t x, int32_t y, int32_t z)
{
    assert(NULL == tiles_find_tile_at(dungeon->tiles, point_make(x, y, z)));
    
    struct tile *tile = tile_alloc(point_make(x, y, z), tile_type_empty);
    tiles_add_tile(area->tiles, tile);
}


static struct point area(struct Dungeon *dungeon, struct point fromPoint, uint32_t length, uint32_t width, enum direction direction, uint32_t leftOffset, enum area_type areaType)
{
    assert(leftOffset < width);
    struct range xRange;
    struct range yRange;
    
    switch (direction) {
        case North:
            xRange = range_make(fromPoint.x - leftOffset, fromPoint.x + width - leftOffset);
            yRange = range_make(fromPoint.y, fromPoint.y + length);
            break;
        case South: {
            struct reverse_range xReverseRange = reverse_range_make(fromPoint.x + leftOffset,
                                                                   fromPoint.x - width + leftOffset);
            struct reverse_range yReverseRange = reverse_range_make(fromPoint.y, fromPoint.y - length);
            xRange = range_from_reverse_range(xReverseRange);
            yRange = range_from_reverse_range(yReverseRange);
        }
            break;
        case East: {
            struct reverse_range yReverseRange = reverse_range_make(fromPoint.y + leftOffset,
                                                                   fromPoint.y - width + leftOffset);
            xRange = range_make(fromPoint.x, fromPoint.x + length);
            yRange = range_from_reverse_range(yReverseRange);
        }
            break;
        case West: {
            struct reverse_range xReverseRange = reverse_range_make(fromPoint.x, fromPoint.x - length);
            xRange = range_from_reverse_range(xReverseRange);
            yRange = range_make(fromPoint.y - leftOffset, fromPoint.y + width - leftOffset);
        }
            break;
        default:
            fail("Unrecognized direction %i", direction);
            return fromPoint;
    }
    
    char *description;
    switch (areaType) {
        case area_type_chamber:
            description = str_alloc_formatted("%u' x %u' chamber",
                                              length * 10, width * 10);
            break;
        case area_type_passage:
            description = str_alloc_formatted("%u' passage %s",
                                              length * 10,
                                              direction_name(direction));
            break;
        default:
            description = str_alloc_formatted("%u' x %u' area",
                                              length * 10, width * 10);
            break;
    }
    
    struct area *area = area_alloc(description, dungeon->tiles, areaType);
    free_or_die(description);
    areas_append_area(dungeon->areas, area);
    
    for (int32_t j = yRange.begin; j < yRange.end; ++j) {
        for (int32_t i = xRange.begin; i < xRange.end; ++i) {
            addNewEmptyTileToDungeonAt(dungeon, area, i, j, fromPoint.z);
        }
    }
    
    return advancePoint(fromPoint, length, direction);
}


static struct point chamber(struct Dungeon *dungeon, struct point fromPoint, uint32_t length, uint32_t width, enum direction direction, uint32_t leftOffset)
{
    return area(dungeon, fromPoint, length, width, direction, leftOffset, area_type_chamber);
}


char const **dungeonAreaDescriptions(struct Dungeon *dungeon)
{
    size_t descriptionsCount = areas_count(dungeon->areas);
    char const **descriptions = calloc_or_die(descriptionsCount + 1,
                                              sizeof(char const *));
    
    for (size_t i = 0; i < descriptionsCount; ++i) {
        descriptions[i] = areas_area_at_index(dungeon->areas, i)->description;
    }
    return descriptions;
}


void finalizeDungeon(struct Dungeon *dungeon)
{
    areas_free(dungeon->areas);
    tiles_free(dungeon->tiles);
}


void generateDungeon(struct Dungeon *dungeon, struct rnd *rnd)
{
    generateSmallDungeon(dungeon);
}


void generateSmallDungeon(struct Dungeon *dungeon)
{
    struct point point = point_make(0, 0, 1);
    
    point = passage(dungeon, point, 2, North);
    
    point = chamber(dungeon, point, 5, 3, North, 1);
    
    /* chamber exits */
    struct point pointInChamber = advancePoint(point, 2, South);
    struct point northWestExit = advancePoint(pointInChamber, 2, West);
    struct point southWestExit = advancePoint(northWestExit, 2, South);
    struct point northEastExit = advancePoint(pointInChamber, 2, East);
    struct point southEastExit = advancePoint(northEastExit, 2, South);
    
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
    
    struct tile *tile = tiles_find_tile_at(dungeon->tiles, point_make(5, 2, 1));
    tiles_remove_tile(dungeon->tiles, tile);
    tile_free(tile);
}


static struct point passage(struct Dungeon *dungeon, struct point fromPoint, uint32_t distance, enum direction direction)
{
    return area(dungeon, fromPoint, distance, 1, direction, 0, area_type_passage);
}


void initializeDungeon(struct Dungeon *dungeon)
{
    memset(dungeon, 0, sizeof(struct Dungeon));
    dungeon->areas = areas_alloc();
    dungeon->tiles = tiles_alloc();
}
