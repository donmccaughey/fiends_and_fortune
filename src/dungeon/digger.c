#include "digger.h"

#include <assert.h>

#include "common/alloc_or_die.h"
#include "common/dice.h"
#include "common/fail.h"
#include "common/rnd.h"
#include "common/str.h"

#include "area.h"
#include "dungeon.h"
#include "dungeon_generator.h"
#include "range.h"
#include "reverse_range.h"
#include "tiles.h"


static struct range
x_range_for_area(struct digger *digger,
                 uint32_t length,
                 uint32_t width,
                 uint32_t left_offset);

static struct range
y_range_for_area(struct digger *digger,
                 uint32_t length,
                 uint32_t width,
                 uint32_t left_offset);


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
    dungeon_generator_add_digger(digger->generator, copy);
    return copy;
}


struct area *
digger_dig_area(struct digger *digger,
                uint32_t length,
                uint32_t width,
                uint32_t left_offset,
                enum area_type area_type)
{
    struct range x_range = x_range_for_area(digger, length, width, left_offset);
    struct range y_range = y_range_for_area(digger, length, width, left_offset);
    struct range z_range = range_make(digger->point.z, digger->point.z + 1);
    if (tiles_has_tile_in_range(digger->generator->dungeon->tiles,
                                x_range, y_range, z_range))
    {
        return NULL;
    }
    
    struct area *area = area_alloc(digger->generator->dungeon->tiles,
                                   area_type,
                                   orientation_from_direction(digger->direction),
                                   tile_type_empty,
                                   x_range,
                                   y_range,
                                   digger->point.z);
    dungeon_add_area(digger->generator->dungeon, area);
    
    digger->point = point_move(digger->point, length, digger->direction);
    return area;
}


struct area *
digger_dig_chamber(struct digger *digger,
                   uint32_t length,
                   uint32_t width,
                   uint32_t left_offset)
{
    return digger_dig_area(digger, length, width, left_offset, area_type_chamber);
}


struct area *
digger_dig_intersection(struct digger *digger)
{
    return digger_dig_area(digger, 1, 1, 0, area_type_intersection);
}


struct area *
digger_dig_passage(struct digger *digger, uint32_t distance)
{
    return digger_dig_area(digger, distance, 1, 0, area_type_passage);
}


void
digger_drop(struct digger *digger)
{
    dungeon_generator_remove_digger(digger->generator, digger);
    digger_free(digger);
}


void
digger_free(struct digger *digger)
{
    free_or_die(digger);
}


void
digger_generate_chamber(struct digger *digger)
{
    int score = roll("1d20", digger->generator->rnd);
    if (score <= 2) {
        // square 20x20
        digger_dig_chamber(digger, 2, 2, 0);
    } else if (score <= 4) {
        // square 20x20
        digger_dig_chamber(digger, 2, 2, 1);
    } else if (score <= 6) {
        // square 30x30
        uint32_t left_offset = rnd_next_uniform_value(digger->generator->rnd, 3);
        digger_dig_chamber(digger, 3, 3, left_offset);
    } else if (score <= 8) {
        // square 40x40
        uint32_t left_offset = rnd_next_uniform_value(digger->generator->rnd, 4);
        digger_dig_chamber(digger, 4, 4, left_offset);
    } else if (score <= 10) {
        // rectangular 20x30
        uint32_t left_offset = rnd_next_uniform_value(digger->generator->rnd, 3);
        digger_dig_chamber(digger, 2, 3, left_offset);
    } else if (score <= 13) {
        // rectangular 20x30
        uint32_t left_offset = rnd_next_uniform_value(digger->generator->rnd, 2);
        digger_dig_chamber(digger, 3, 2, left_offset);
    } else if (score <= 15) {
        // rectangular 30x50
        uint32_t orientation = rnd_next_uniform_value(digger->generator->rnd, 2);
        if (orientation) {
            uint32_t left_offset = rnd_next_uniform_value(digger->generator->rnd, 3);
            digger_dig_chamber(digger, 5, 3, left_offset);
        } else {
            uint32_t left_offset = rnd_next_uniform_value(digger->generator->rnd, 5);
            digger_dig_chamber(digger, 3, 5, left_offset);
        }
    } else if (score <= 17) {
        // rectangular 40x60
        uint32_t orientation = rnd_next_uniform_value(digger->generator->rnd, 2);
        if (orientation) {
            uint32_t left_offset = rnd_next_uniform_value(digger->generator->rnd, 4);
            digger_dig_chamber(digger, 6, 4, left_offset);
        } else {
            uint32_t left_offset = rnd_next_uniform_value(digger->generator->rnd, 6);
            digger_dig_chamber(digger, 4, 6, left_offset);
        }
    } else {
        // unusual shape and size
    }
    // TODO: exits
    digger_drop(digger);
}


void
digger_generate_side_passage(struct digger *digger)
{
    int score = roll("1d20", digger->generator->rnd);
    if (score <= 2) {
        // left 90 degrees
        if (!digger_dig_intersection(digger)) return;
        
        struct digger *side_digger = digger_copy(digger);
        digger_turn_90_degrees_left(side_digger);
        digger_dig_passage(side_digger, 3);
        
        digger_dig_passage(digger, 3);
    } else if (score <= 4) {
        // right 90 degrees
        if (!digger_dig_intersection(digger)) return;
        
        struct digger *side_digger = digger_copy(digger);
        digger_turn_90_degrees_right(side_digger);
        digger_dig_passage(side_digger, 3);
        
        digger_dig_passage(digger, 3);
    } else if (score == 5) {
        // left 45 degrees ahead
    } else if (score == 6) {
        // right 45 degrees ahead
    } else if (score == 7) {
        // left 45 degrees behind
    } else if (score == 8) {
        // right 45 degrees behind
    } else if (score == 9) {
        // left curve 45 degrees ahead
    } else if (score == 10) {
        // right curve 45 degrees ahead
    } else if (score <= 13) {
        // passage T's
        if (!digger_dig_intersection(digger)) return;
        
        struct digger *left_digger = digger_copy(digger);
        digger_turn_90_degrees_left(left_digger);
        digger_dig_passage(left_digger, 3);
        
        struct digger *right_digger = digger_copy(digger);
        digger_turn_90_degrees_right(right_digger);
        digger_dig_passage(right_digger, 3);
        
        digger_drop(digger);
    } else if (score <= 15) {
        // passage Y's
    } else if (score < 19) {
        // four way intersection
        if (!digger_dig_intersection(digger)) return;
        
        struct digger *left_digger = digger_copy(digger);
        digger_turn_90_degrees_left(left_digger);
        digger_dig_passage(left_digger, 3);
        
        struct digger *right_digger = digger_copy(digger);
        digger_turn_90_degrees_right(right_digger);
        digger_dig_passage(right_digger, 3);
        
        digger_dig_passage(digger, 3);
    } else {
        // passage X's
    }
}


void
digger_generate_turn(struct digger *digger)
{
    int score = roll("1d20", digger->generator->rnd);
    if (score <= 8) {
        // left 90 degrees
        digger_turn_90_degrees_left(digger);
        digger_dig_passage(digger, 3);
    } else if (score == 9) {
        // left 45 degrees ahead
    } else if (score == 10) {
        // left 45 degrees behind
    } else if (score <= 18) {
        // right 90 degrees
        digger_turn_90_degrees_right(digger);
        digger_dig_passage(digger, 3);
    } else if (score == 19) {
        // right 45 degrees ahead
    } else {
        // right 45 degrees behind
    }
}


void
digger_move(struct digger *digger, int32_t steps, enum direction direction)
{
    digger->point = point_move(digger->point, steps, direction);
}


void
digger_periodic_check(struct digger *digger)
{
    int score = roll("1d20", digger->generator->rnd);
    if (score <= 2) {
        digger_dig_passage(digger, 6);
    } else if (score <= 5) {
        // door
    } else if (score <= 10) {
        // side passage
        digger_generate_side_passage(digger);
    } else if (score <= 13) {
        // passage turns
        digger_generate_turn(digger);
    } else if (score <= 16) {
        // chamber
        digger_generate_chamber(digger);
    } else if (score == 17) {
        // stairs
    } else if (score == 18) {
        // dead end
        digger_drop(digger);
    } else if (score == 19) {
        // trick/trap
    } else {
        // wandering monster
    }
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


static struct range
x_range_for_area(struct digger *digger,
                 uint32_t length,
                 uint32_t width,
                 uint32_t left_offset)
{
    assert(left_offset < width);
    switch (digger->direction) {
        case direction_north:
            return range_make(digger->point.x - left_offset,
                              digger->point.x + width - left_offset);
        case direction_south: {
            struct reverse_range x_reverse_range = reverse_range_make(digger->point.x + left_offset,
                                                                      digger->point.x - width + left_offset);
            return range_from_reverse_range(x_reverse_range);
        }
        case direction_east:
            return range_make(digger->point.x, digger->point.x + length);
        case direction_west: {
            struct reverse_range x_reverse_range = reverse_range_make(digger->point.x,
                                                                      digger->point.x - length);
            return range_from_reverse_range(x_reverse_range);
        }
        default:
            fail("Unrecognized direction %i", digger->direction);
            return range_make(0, 0);
    }
}


static struct range
y_range_for_area(struct digger *digger,
                 uint32_t length,
                 uint32_t width,
                 uint32_t left_offset)
{
    assert(left_offset < width);
    switch (digger->direction) {
        case direction_north:
            return range_make(digger->point.y, digger->point.y + length);
        case direction_south: {
            struct reverse_range y_reverse_range = reverse_range_make(digger->point.y,
                                                                      digger->point.y - length);
            return range_from_reverse_range(y_reverse_range);
        }
        case direction_east: {
            struct reverse_range y_reverse_range = reverse_range_make(digger->point.y + left_offset,
                                                                      digger->point.y - width + left_offset);
            return range_from_reverse_range(y_reverse_range);
        }
            break;
        case direction_west:
            return range_make(digger->point.y - left_offset,
                              digger->point.y + width - left_offset);
        default:
            fail("Unrecognized direction %i", digger->direction);
            return range_make(0, 0);
    }
}
