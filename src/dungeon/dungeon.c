#include "dungeon.h"

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


static struct point
advance_point(struct point start, int32_t steps, enum direction direction);

static void
add_new_empty_tile_to_dungeon_at(struct dungeon *dungeon,
                                 struct area *area,
                                 int32_t x,
                                 int32_t y,
                                 int32_t z);

static struct point
area(struct dungeon *dungeon,
     struct point from_point,
     uint32_t length,
     uint32_t width,
     enum direction direction,
     uint32_t left_offset,
     enum area_type area_type);

static struct point
chamber(struct dungeon *dungeon,
        struct point from_point,
        uint32_t length,
        uint32_t width,
        enum direction direction,
        uint32_t left_offset);

static struct point
passage(struct dungeon *dungeon,
        struct point from_point,
        uint32_t distance,
        enum direction direction);


static struct point
advance_point(struct point start, int32_t steps, enum direction direction)
{
    switch (direction) {
        case North: return point_make(start.x, start.y + steps, start.z);
        case South: return point_make(start.x, start.y - steps, start.z);
        case East: return point_make(start.x + steps, start.y, start.z);
        case West: return point_make(start.x - steps, start.y, start.z);
        default: fail("Unrecognized direction %i", direction); break;
    }
    return start;
}


static void
add_new_empty_tile_to_dungeon_at(struct dungeon *dungeon,
                                 struct area *area,
                                 int32_t x,
                                 int32_t y,
                                 int32_t z)
{
    assert(NULL == tiles_find_tile_at(dungeon->tiles, point_make(x, y, z)));
    
    struct tile *tile = tile_alloc(point_make(x, y, z), tile_type_empty);
    tiles_add_tile(area->tiles, tile);
}


static struct point
area(struct dungeon *dungeon,
     struct point from_point,
     uint32_t length,
     uint32_t width,
     enum direction direction,
     uint32_t left_offset,
     enum area_type area_type)
{
    assert(left_offset < width);
    struct range x_range;
    struct range y_range;
    
    switch (direction) {
        case North:
            x_range = range_make(from_point.x - left_offset,
                                 from_point.x + width - left_offset);
            y_range = range_make(from_point.y, from_point.y + length);
            break;
        case South: {
                struct reverse_range xReverseRange = reverse_range_make(from_point.x + left_offset,
                                                                       from_point.x - width + left_offset);
                struct reverse_range yReverseRange = reverse_range_make(from_point.y,
                                                                        from_point.y - length);
                x_range = range_from_reverse_range(xReverseRange);
                y_range = range_from_reverse_range(yReverseRange);
            }
            break;
        case East: {
                struct reverse_range yReverseRange = reverse_range_make(from_point.y + left_offset,
                                                                       from_point.y - width + left_offset);
                x_range = range_make(from_point.x, from_point.x + length);
                y_range = range_from_reverse_range(yReverseRange);
            }
            break;
        case West: {
                struct reverse_range xReverseRange = reverse_range_make(from_point.x, from_point.x - length);
                x_range = range_from_reverse_range(xReverseRange);
                y_range = range_make(from_point.y - left_offset, from_point.y + width - left_offset);
            }
            break;
        default:
            fail("Unrecognized direction %i", direction);
            return from_point;
    }
    
    char *description;
    switch (area_type) {
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
    
    struct area *area = area_alloc(description, dungeon->tiles, area_type);
    free_or_die(description);
    areas_append_area(dungeon->areas, area);
    
    for (int32_t j = y_range.begin; j < y_range.end; ++j) {
        for (int32_t i = x_range.begin; i < x_range.end; ++i) {
            add_new_empty_tile_to_dungeon_at(dungeon, area, i, j, from_point.z);
        }
    }
    
    return advance_point(from_point, length, direction);
}


static struct point chamber(struct dungeon *dungeon,
                            struct point from_point,
                            uint32_t length,
                            uint32_t width,
                            enum direction direction,
                            uint32_t left_offset)
{
    return area(dungeon, from_point, length, width, direction, left_offset, area_type_chamber);
}


char const **
dungeon_alloc_area_descriptions(struct dungeon *dungeon)
{
    size_t descriptionsCount = areas_count(dungeon->areas);
    char const **descriptions = calloc_or_die(descriptionsCount + 1,
                                              sizeof(char const *));
    
    for (size_t i = 0; i < descriptionsCount; ++i) {
        descriptions[i] = areas_area_at_index(dungeon->areas, i)->description;
    }
    return descriptions;
}


void
dungeon_finalize(struct dungeon *dungeon)
{
    areas_free(dungeon->areas);
    tiles_free(dungeon->tiles);
}


void
dungeon_generate(struct dungeon *dungeon, struct rnd *rnd)
{
    dungeon_generate_small(dungeon);
}


void
dungeon_generate_small(struct dungeon *dungeon)
{
    struct point point = point_make(0, 0, 1);
    
    point = passage(dungeon, point, 2, North);
    
    point = chamber(dungeon, point, 5, 3, North, 1);
    
    /* chamber exits */
    struct point pointInChamber = advance_point(point, 2, South);
    struct point northWestExit = advance_point(pointInChamber, 2, West);
    struct point southWestExit = advance_point(northWestExit, 2, South);
    struct point northEastExit = advance_point(pointInChamber, 2, East);
    struct point southEastExit = advance_point(northEastExit, 2, South);
    
    point = passage(dungeon, point, 7, North);
    point = passage(dungeon, point, 8, East);
    point = passage(dungeon, point, 4, South);
    
    point = chamber(dungeon, point, 3, 4, South, 0);
    point = advance_point(point, 3, West);
    
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
    
    point = advance_point(point, 1, West);
    point = passage(dungeon, point, 3, North);
    
    chamber(dungeon, point, 2, 3, North, 1);
    
    /* from entry chamber, south east exit */
    point = passage(dungeon, southEastExit, 1, East);
    
    chamber(dungeon, point, 6, 4, East, 0);
    
    struct tile *tile = tiles_find_tile_at(dungeon->tiles, point_make(5, 2, 1));
    tiles_remove_tile(dungeon->tiles, tile);
    tile_free(tile);
}


static struct point
passage(struct dungeon *dungeon, struct point from_point, uint32_t distance, enum direction direction)
{
    return area(dungeon, from_point, distance, 1, direction, 0, area_type_passage);
}


void
dungeon_initialize(struct dungeon *dungeon)
{
    memset(dungeon, 0, sizeof(struct dungeon));
    dungeon->areas = areas_alloc();
    dungeon->tiles = tiles_alloc();
}
