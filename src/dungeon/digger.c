#include "digger.h"

#include <assert.h>
#include <base/base.h>

#include "generator.h"
#include "tile.h"


static struct box
box_for_area(struct digger *digger,
             int length,
             int width,
             int left_offset,
             int padding)
{
    assert(left_offset < width);
    struct point origin;
    struct size size;
    switch (digger->direction) {
        case direction_north:
            origin = point_make(digger->point.x - left_offset - padding,
                                digger->point.y,
                                digger->point.z);
            size = size_make(width + (2 * padding), length + padding, 1);
            break;
        case direction_south:
            origin = point_make(digger->point.x + left_offset + padding,
                                digger->point.y,
                                digger->point.z);
            size = size_make(-width - (2 * padding), -length - padding, 1);
            break;
        case direction_east:
            origin = point_make(digger->point.x,
                                digger->point.y + left_offset + padding,
                                digger->point.z);
            size = size_make(length + padding, -width - (2 * padding), 1);
            break;
        case direction_west:
            origin = point_make(digger->point.x,
                                digger->point.y - left_offset - padding,
                                digger->point.z);
            size = size_make(-length - padding, width + (2 * padding), 1);
            break;
        default:
            fail("Unrecognized direction %i", digger->direction);
            origin = point_make(0, 0, 0);
            size = size_make(0, 0, 0);
            break;
    }
    return box_normalize(box_make(origin, size));
}


struct digger *
digger_alloc(struct generator *generator,
             struct point point,
             enum direction direction)
{
    struct digger *digger = calloc_or_die(1, sizeof(struct digger));
    digger->generator = generator;
    digger->point = point;
    digger->direction = direction;
    return digger;
}


void
digger_ascend(struct digger *digger, int levels)
{
    digger->point.z -= levels;
}


void
digger_descend(struct digger *digger, int levels)
{
    digger->point.z += levels;
}


struct area *
digger_dig_area(struct digger *digger,
                int length,
                int width,
                int left_offset,
                enum wall_type entrance_type,
                enum area_type area_type)
{
    int const padding = 0;
    struct box box_to_dig = box_for_area(digger,
                                         length,
                                         width,
                                         left_offset,
                                         padding);
    if (generator_is_box_excavated(digger->generator, box_to_dig)) {
        return NULL;
    }
    
    struct box box_for_level = generator_box_for_level(digger->generator,
                                                       digger->point.z);
    if (box_for_level.size.width && box_for_level.size.height) {
        struct box new_box_for_level = box_for_level = box_make_from_boxes(box_for_level,
                                                                           box_to_dig);
        if (   new_box_for_level.size.width > digger->generator->max_size.width
            || new_box_for_level.size.length > digger->generator->max_size.length)
        {
            return NULL;
        }
    }
    
    enum tile_type tile_type;
    switch (area_type) {
        case area_type_stairs_up: tile_type = tile_type_stairs_up; break;
        case area_type_stairs_down: tile_type = tile_type_stairs_down; break;
        default: tile_type = tile_type_empty; break;
    }
    struct area *area = generator_add_area(digger->generator,
                                           area_type,
                                           digger->direction,
                                           box_to_dig,
                                           tile_type);
    
    struct tile *entrance_tile;
    switch (digger->direction) {
        case direction_north:
            entrance_tile = generator_tile_at(digger->generator, digger->point);
            entrance_tile->walls.south = entrance_type;
            break;
        case direction_south:
            entrance_tile = generator_tile_at(digger->generator,
                                              point_north(digger->point));
            entrance_tile->walls.south = entrance_type;
            break;
        case direction_east:
            entrance_tile = generator_tile_at(digger->generator, digger->point);
            entrance_tile->walls.west = entrance_type;
            break;
        case direction_west:
            entrance_tile = generator_tile_at(digger->generator,
                                              point_east(digger->point));
            entrance_tile->walls.west = entrance_type;
            break;
        default:
            fail("Unrecognized direction %i", digger->direction);
            break;
    }
    
    return area;
}


struct area *
digger_dig_chamber(struct digger *digger,
                   int length,
                   int width,
                   int left_offset,
                   enum wall_type entrance_type)
{
    struct box padded_box = box_for_area(digger,
                                         length,
                                         width,
                                         left_offset,
                                         digger->generator->padding);
    if (generator_is_box_excavated(digger->generator, padded_box)) {
        return NULL;
    }
    struct area *chamber = digger_dig_area(digger,
                                           length,
                                           width,
                                           left_offset,
                                           entrance_type,
                                           area_type_chamber);
    return chamber;
}


struct area *
digger_dig_intersection(struct digger *digger)
{
    int const length = 1;
    int const width = 1;
    int const left_offset = 0;
    struct box padded_box = box_for_area(digger,
                                         length,
                                         width,
                                         left_offset,
                                         digger->generator->padding);
    if (generator_is_box_excavated(digger->generator, padded_box)) {
        return NULL;
    }
    struct area *intersection = digger_dig_area(digger,
                                                length,
                                                width,
                                                left_offset,
                                                wall_type_none,
                                                area_type_intersection);
    digger_move_forward(digger, length);
    return intersection;
}


struct area *
digger_dig_passage(struct digger *digger,
                   int distance,
                   enum wall_type entrance_type)
{
    int const length = distance;
    int const width = 1;
    int const left_offset = 0;
    struct box padded_box = box_for_area(digger,
                                         length,
                                         width,
                                         left_offset,
                                         digger->generator->padding);
    if (generator_is_box_excavated(digger->generator, padded_box)) {
        return NULL;
    }
    struct area *passage = digger_dig_area(digger,
                                           length,
                                           width,
                                           left_offset,
                                           entrance_type,
                                           area_type_passage);
    digger_move_forward(digger, length);
    return passage;
}


struct area *
digger_dig_room(struct digger *digger,
                int length,
                int width,
                int left_offset,
                enum wall_type entrance_type)
{
    struct box padded_box = box_for_area(digger,
                                         length,
                                         width,
                                         left_offset,
                                         digger->generator->padding);
    if (generator_is_box_excavated(digger->generator, padded_box)) {
        return NULL;
    }
    struct area *room = digger_dig_area(digger,
                                        length,
                                        width,
                                        left_offset,
                                        entrance_type,
                                        area_type_room);
    return room;
}


void
digger_dig_starting_stairs(struct digger *digger)
{
    digger_move_forward(digger, 1);
    digger_spin_180_degrees(digger);
    digger_dig_area(digger, 2, 1, 0, wall_type_none, area_type_stairs_up);
    digger_spin_180_degrees(digger);
    digger_move_forward(digger, 1);
}


struct area *
digger_dig_stairs_down(struct digger *digger,
                       int distance,
                       enum wall_type entrance_type)
{
    int const length = distance;
    int const width = 1;
    int const left_offset = 0;
    struct box padded_box = box_for_area(digger,
                                         length,
                                         width,
                                         left_offset,
                                         digger->generator->padding);
    if (generator_is_box_excavated(digger->generator, padded_box)) {
        return NULL;
    }
    struct area *stairs_down = digger_dig_area(digger,
                                               length,
                                               width,
                                               left_offset,
                                               entrance_type,
                                               area_type_stairs_down);
    digger_move_forward(digger, length);
    return stairs_down;
}


struct area *
digger_dig_stairs_up(struct digger *digger,
                     int distance,
                     enum wall_type entrance_type)
{
    int const length = distance;
    int const width = 1;
    int const left_offset = 0;
    struct box padded_box = box_for_area(digger,
                                         length,
                                         width,
                                         left_offset,
                                         digger->generator->padding);
    if (generator_is_box_excavated(digger->generator, padded_box)) {
        return NULL;
    }
    struct area *stairs_up = digger_dig_area(digger,
                                             length,
                                             width,
                                             left_offset,
                                             entrance_type,
                                             area_type_stairs_up);
    digger_move_forward(digger, length);
    return stairs_up;
}


void
digger_free(struct digger *digger)
{
    free_or_die(digger);
}


void
digger_move(struct digger *digger, int steps, enum direction direction)
{
    digger->point = point_move(digger->point, steps, direction);
}


void
digger_move_backward(struct digger *digger, int steps)
{
    digger_move(digger, steps, direction_opposite(digger->direction));
}


void
digger_move_forward(struct digger *digger, int steps)
{
    digger_move(digger, steps, digger->direction);
}


void
digger_move_left(struct digger *digger, int steps)
{
    digger_move(digger, steps, direction_90_degrees_left(digger->direction));
}


void
digger_move_right(struct digger *digger, int steps)
{
    digger_move(digger, steps, direction_90_degrees_right(digger->direction));
}


void
digger_spin_180_degrees(struct digger *digger)
{
    digger->direction = direction_opposite(digger->direction);
}


void
digger_spin_90_degrees_left(struct digger *digger)
{
    digger->direction = direction_90_degrees_left(digger->direction);
}


void
digger_spin_90_degrees_right(struct digger *digger)
{
    digger->direction = direction_90_degrees_right(digger->direction);
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
