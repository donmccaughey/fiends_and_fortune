#include "digger.h"

#include <assert.h>

#include "common/alloc_or_die.h"
#include "common/fail.h"
#include "common/str.h"

#include "area.h"
#include "areas.h"
#include "dungeon.h"
#include "dungeon_generator.h"
#include "range.h"
#include "reverse_range.h"


struct digger *
digger_alloc(struct point point, enum direction direction)
{
    struct digger *digger = calloc_or_die(1, sizeof(struct digger));
    digger->point = point;
    digger->direction = direction;
    return digger;
}


struct digger *
digger_copy(struct digger *digger)
{
    struct digger *copy = digger_alloc(digger->point, digger->direction);
    dungeon_generator_take_on_digger(digger->generator, copy);
    return copy;
}


void
digger_dig_area(struct digger *digger,
                uint32_t length,
                uint32_t width,
                uint32_t left_offset,
                enum area_type area_type)
{
    assert(left_offset < width);
    struct range x_range;
    struct range y_range;
    
    switch (digger->direction) {
        case North:
            x_range = range_make(digger->point.x - left_offset,
                                 digger->point.x + width - left_offset);
            y_range = range_make(digger->point.y, digger->point.y + length);
            break;
        case South: {
            struct reverse_range x_reverse_range = reverse_range_make(digger->point.x + left_offset,
                                                                      digger->point.x - width + left_offset);
            struct reverse_range y_reverse_range = reverse_range_make(digger->point.y,
                                                                      digger->point.y - length);
            x_range = range_from_reverse_range(x_reverse_range);
            y_range = range_from_reverse_range(y_reverse_range);
        }
            break;
        case East: {
            struct reverse_range y_reverse_range = reverse_range_make(digger->point.y + left_offset,
                                                                      digger->point.y - width + left_offset);
            x_range = range_make(digger->point.x, digger->point.x + length);
            y_range = range_from_reverse_range(y_reverse_range);
        }
            break;
        case West: {
            struct reverse_range x_reverse_range = reverse_range_make(digger->point.x,
                                                                      digger->point.x - length);
            x_range = range_from_reverse_range(x_reverse_range);
            y_range = range_make(digger->point.y - left_offset,
                                 digger->point.y + width - left_offset);
        }
            break;
        default:
            fail("Unrecognized direction %i", digger->direction);
            return;
    }
    
    char *description;
    switch (area_type) {
        case area_type_chamber:
            description = str_alloc_formatted("%u' x %u' chamber",
                                              length * 10, width * 10);
            break;
        case area_type_intersection:
            description = strdup_or_die("intersection");
            break;
        case area_type_passage:
            description = str_alloc_formatted("%u' passage %s",
                                              length * 10,
                                              direction_name(digger->direction));
            break;
        default:
            description = str_alloc_formatted("%u' x %u' area",
                                              length * 10, width * 10);
            break;
    }
    
    struct area *area = area_alloc(description, digger->generator->dungeon->tiles, area_type);
    free_or_die(description);
    areas_append_area(digger->generator->dungeon->areas, area);
    area_add_tiles(area, tile_type_empty, x_range, y_range, digger->point.z);
    
    digger->point = point_move(digger->point, length, digger->direction);
}


void
digger_dig_chamber(struct digger *digger,
                   uint32_t length,
                   uint32_t width,
                   uint32_t left_offset)
{
    digger_dig_area(digger, length, width, left_offset, area_type_chamber);
}


void
digger_dig_intersection(struct digger *digger)
{
    return digger_dig_area(digger, 1, 1, 0, area_type_intersection);
}


void
digger_dig_passage(struct digger *digger, uint32_t distance)
{
    digger_dig_area(digger, distance, 1, 0, area_type_passage);
}


void
digger_drop(struct digger *digger)
{
    dungeon_generator_give_up_digger(digger->generator, digger);
    digger_free(digger);
}


void
digger_free(struct digger *digger)
{
    free_or_die(digger);
}


void
digger_turn_90_degrees_left(struct digger *digger)
{
    digger->point = point_rotate_90_degrees_left(digger->point, digger->direction);
    digger->direction = direction_90_degrees_left(digger->direction);
}


void
digger_turn_90_degrees_right(struct digger *digger)
{
    digger->point = point_rotate_90_degrees_right(digger->point, digger->direction);
    digger->direction = direction_90_degrees_right(digger->direction);
}
