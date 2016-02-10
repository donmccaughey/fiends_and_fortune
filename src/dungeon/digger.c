#include "digger.h"

#include <assert.h>

#include "common/alloc_or_die.h"
#include "common/dice.h"
#include "common/fail.h"
#include "common/rnd.h"

#include "area.h"
#include "area_type.h"
#include "dungeon.h"
#include "dungeon_generator.h"
#include "tile.h"


static void
location_of_door(struct rnd *rnd, bool *left, bool *right, bool *ahead);

static void
space_beyond_door(struct digger *digger, bool is_straight_ahead);


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
            size = size_make(-length - padding, +width + (2 * padding), 1);
            break;
        default:
            fail("Unrecognized direction %i", digger->direction);
            origin = point_make(0, 0, 0);
            size = size_make(0, 0, 0);
            break;
    }
    return box_normalize(box_make(origin, size));
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
    if (dungeon_is_box_excavated(digger->generator->dungeon, box_to_dig)) {
        return NULL;
    }
    
    struct box box_for_level = dungeon_box_for_level(digger->generator->dungeon,
                                                     digger->point.z);
    struct box new_box_for_level = box_for_level = box_make_from_boxes(box_for_level,
                                                                       box_to_dig);
    if (   new_box_for_level.size.width > digger->generator->max_size.width
        || new_box_for_level.size.length > digger->generator->max_size.length)
    {
        return NULL;
    }
    
    struct area *area = dungeon_add_area(digger->generator->dungeon,
                                         area_type,
                                         orientation_from_direction(digger->direction),
                                         box_to_dig,
                                         tile_type_empty);
    
    struct tile *entrance_tile;
    switch (digger->direction) {
        case direction_north:
            entrance_tile = dungeon_tile_at(digger->generator->dungeon,
                                            digger->point);
            entrance_tile->walls.south = entrance_type;
            break;
        case direction_south:
            entrance_tile = dungeon_tile_at(digger->generator->dungeon,
                                            point_north(digger->point));
            entrance_tile->walls.south = entrance_type;
            break;
        case direction_east:
            entrance_tile = dungeon_tile_at(digger->generator->dungeon,
                                            digger->point);
            entrance_tile->walls.west = entrance_type;
            break;
        case direction_west:
            entrance_tile = dungeon_tile_at(digger->generator->dungeon,
                                            point_east(digger->point));
            entrance_tile->walls.west = entrance_type;
            break;
        default:
            fail("Unrecognized direction %i", digger->direction);
            break;
    }
    
    digger->point = point_move(digger->point, length, digger->direction);
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
    if (dungeon_is_box_excavated(digger->generator->dungeon, padded_box)) {
        return NULL;
    }
    return digger_dig_area(digger,
                           length,
                           width,
                           left_offset,
                           entrance_type,
                           area_type_chamber);
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
    if (dungeon_is_box_excavated(digger->generator->dungeon, padded_box)) {
        return NULL;
    }
    return digger_dig_area(digger,
                           length,
                           width,
                           left_offset,
                           wall_type_none,
                           area_type_intersection);
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
    if (dungeon_is_box_excavated(digger->generator->dungeon, padded_box)) {
        return NULL;
    }
    return digger_dig_area(digger,
                           length,
                           width,
                           left_offset,
                           entrance_type,
                           area_type_passage);
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
    if (dungeon_is_box_excavated(digger->generator->dungeon, padded_box)) {
        return NULL;
    }
    return digger_dig_area(digger,
                           length,
                           width,
                           left_offset,
                           entrance_type,
                           area_type_room);
}


void
digger_generate_chamber(struct digger *digger,
                        enum wall_type entrance_type)
{
    int score = roll("1d20", digger->generator->rnd);
    if (score <= 2) {
        // square 20x20
        digger_dig_chamber(digger, 2, 2, 0, entrance_type);
    } else if (score <= 4) {
        // square 20x20
        digger_dig_chamber(digger, 2, 2, 1, entrance_type);
    } else if (score <= 6) {
        // square 30x30
        int left_offset = rnd_next_uniform_value(digger->generator->rnd, 3);
        digger_dig_chamber(digger, 3, 3, left_offset, entrance_type);
    } else if (score <= 8) {
        // square 40x40
        int left_offset = rnd_next_uniform_value(digger->generator->rnd, 4);
        digger_dig_chamber(digger, 4, 4, left_offset, entrance_type);
    } else if (score <= 10) {
        // rectangular 20x30
        int left_offset = rnd_next_uniform_value(digger->generator->rnd, 3);
        digger_dig_chamber(digger, 2, 3, left_offset, entrance_type);
    } else if (score <= 13) {
        // rectangular 20x30
        int left_offset = rnd_next_uniform_value(digger->generator->rnd, 2);
        digger_dig_chamber(digger, 3, 2, left_offset, entrance_type);
    } else if (score <= 15) {
        // rectangular 30x50
        int orientation = rnd_next_uniform_value(digger->generator->rnd, 2);
        if (orientation) {
            int left_offset = rnd_next_uniform_value(digger->generator->rnd, 3);
            digger_dig_chamber(digger, 5, 3, left_offset, entrance_type);
        } else {
            int left_offset = rnd_next_uniform_value(digger->generator->rnd, 5);
            digger_dig_chamber(digger, 3, 5, left_offset, entrance_type);
        }
    } else if (score <= 17) {
        // rectangular 40x60
        int orientation = rnd_next_uniform_value(digger->generator->rnd, 2);
        if (orientation) {
            int left_offset = rnd_next_uniform_value(digger->generator->rnd, 4);
            digger_dig_chamber(digger, 6, 4, left_offset, entrance_type);
        } else {
            int left_offset = rnd_next_uniform_value(digger->generator->rnd, 6);
            digger_dig_chamber(digger, 4, 6, left_offset, entrance_type);
        }
    } else {
        // unusual shape and size
    }
    // TODO: exits
    dungeon_generator_delete_digger(digger->generator, digger);
}


void
digger_generate_door(struct digger *digger)
{
    struct digger *door_digger = dungeon_generator_copy_digger(digger->generator,
                                                               digger);
    bool door_left = false;
    bool door_right = false;
    bool door_ahead = false;
    
    location_of_door(digger->generator->rnd,
                     &door_left, &door_right, &door_ahead);
    if (!door_ahead) {
        // repeat periodic check
        int score = roll("1d20", digger->generator->rnd);
        if (score >= 3 && score <= 5) {
            location_of_door(digger->generator->rnd,
                             &door_left, &door_right, &door_ahead);
        } else {
            digger_dig_passage(digger, 3, wall_type_none);
        }
    }
    
    if (door_left) {
        struct digger *left_digger = dungeon_generator_copy_digger(digger->generator,
                                                                   door_digger);
        digger_turn_90_degrees_left(left_digger);
        space_beyond_door(left_digger, false);
    }
    
    if (door_right) {
        struct digger *right_digger = dungeon_generator_copy_digger(digger->generator,
                                                                    door_digger);
        digger_turn_90_degrees_right(right_digger);
        space_beyond_door(right_digger, false);
    }
    
    if (door_ahead) {
        struct digger *ahead_digger = dungeon_generator_copy_digger(digger->generator,
                                                                    door_digger);
        space_beyond_door(ahead_digger, true);
    }
    
    dungeon_generator_delete_digger(digger->generator, door_digger);
}


void
digger_generate_room(struct digger *digger,
                     enum wall_type entrance_type)
{
    int score = roll("1d20", digger->generator->rnd);
    if (score <= 2) {
        // square 10x10
        digger_dig_room(digger, 1, 1, 0, entrance_type);
    } else if (score <= 4) {
        // square 20x20
        int left_offset = rnd_next_uniform_value(digger->generator->rnd, 2);
        digger_dig_room(digger, 2, 2, left_offset, entrance_type);
    } else if (score <= 6) {
        // square 30x30
        int left_offset = rnd_next_uniform_value(digger->generator->rnd, 3);
        digger_dig_room(digger, 3, 3, left_offset, entrance_type);
    } else if (score <= 8) {
        // square 40x40
        int left_offset = rnd_next_uniform_value(digger->generator->rnd, 4);
        digger_dig_room(digger, 4, 4, left_offset, entrance_type);
    } else if (score <= 10) {
        // rectangular 10x20
        int orientation = rnd_next_uniform_value(digger->generator->rnd, 2);
        if (orientation) {
            int left_offset = rnd_next_uniform_value(digger->generator->rnd, 2);
            digger_dig_room(digger, 1, 2, left_offset, entrance_type);
        } else {
            digger_dig_room(digger, 2, 1, 0, entrance_type);
        }
    } else if (score <= 13) {
        // rectangular 20x30
        int orientation = rnd_next_uniform_value(digger->generator->rnd, 2);
        if (orientation) {
            int left_offset = rnd_next_uniform_value(digger->generator->rnd, 2);
            digger_dig_room(digger, 3, 2, left_offset, entrance_type);
        } else {
            int left_offset = rnd_next_uniform_value(digger->generator->rnd, 3);
            digger_dig_room(digger, 2, 3, left_offset, entrance_type);
        }
    } else if (score <= 15) {
        // rectangular 20x40
        int orientation = rnd_next_uniform_value(digger->generator->rnd, 2);
        if (orientation) {
            int left_offset = rnd_next_uniform_value(digger->generator->rnd, 2);
            digger_dig_room(digger, 4, 2, left_offset, entrance_type);
        } else {
            int left_offset = rnd_next_uniform_value(digger->generator->rnd, 4);
            digger_dig_room(digger, 2, 4, left_offset, entrance_type);
        }
    } else if (score <= 17) {
        // rectangular 30x40
        int orientation = rnd_next_uniform_value(digger->generator->rnd, 2);
        if (orientation) {
            int left_offset = rnd_next_uniform_value(digger->generator->rnd, 3);
            digger_dig_room(digger, 4, 3, left_offset, entrance_type);
        } else {
            int left_offset = rnd_next_uniform_value(digger->generator->rnd, 4);
            digger_dig_room(digger, 3, 4, left_offset, entrance_type);
        }
    } else {
        // unusual shape and size
    }
    // TODO: exits
    dungeon_generator_delete_digger(digger->generator, digger);
}


void
digger_generate_side_passage(struct digger *digger)
{
    int score = roll("1d20", digger->generator->rnd);
    if (score <= 2) {
        // left 90 degrees
        if (!digger_dig_intersection(digger)) return;
        
        struct digger *side_digger = dungeon_generator_copy_digger(digger->generator,
                                                                   digger);
        digger_turn_90_degrees_left(side_digger);
        digger_dig_passage(side_digger, 3, wall_type_none);
        
        digger_dig_passage(digger, 3, wall_type_none);
    } else if (score <= 4) {
        // right 90 degrees
        if (!digger_dig_intersection(digger)) return;
        
        struct digger *side_digger = dungeon_generator_copy_digger(digger->generator,
                                                                   digger);
        digger_turn_90_degrees_right(side_digger);
        digger_dig_passage(side_digger, 3, wall_type_none);
        
        digger_dig_passage(digger, 3, wall_type_none);
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
        struct area *intersection = digger_dig_intersection(digger);
        if (!intersection) return;
        int exit_count = 0;
        
        struct digger *left_digger = dungeon_generator_copy_digger(digger->generator,
                                                                   digger);
        digger_turn_90_degrees_left(left_digger);
        struct area *left_passage = digger_dig_passage(left_digger, 3, wall_type_none);
        if (left_passage) {
            ++exit_count;
        } else {
            dungeon_generator_delete_digger(digger->generator, left_digger);
        }
        
        struct digger *right_digger = dungeon_generator_copy_digger(digger->generator,
                                                                    digger);
        digger_turn_90_degrees_right(right_digger);
        struct area *right_passage = digger_dig_passage(right_digger, 3, wall_type_none);
        if (right_passage) {
            ++exit_count;
        } else {
            dungeon_generator_delete_digger(digger->generator, right_digger);
        }
        
        if (0 == exit_count || 1 == exit_count) {
            // TODO: remove intersection if zero exits
            intersection->type = area_type_passage;
            // TODO: merge intersection area with adjacent passage
        } else {
            dungeon_generator_delete_digger(digger->generator, digger);
        }
    } else if (score <= 15) {
        // passage Y's
    } else if (score < 19) {
        // four way intersection
        struct area *intersection = digger_dig_intersection(digger);
        if (!intersection) return;
        int exit_count = 0;
        
        struct digger *left_digger = dungeon_generator_copy_digger(digger->generator,
                                                                   digger);
        digger_turn_90_degrees_left(left_digger);
        struct area *left_passage = digger_dig_passage(left_digger, 3, wall_type_none);
        if (left_passage) {
            ++exit_count;
        } else {
            dungeon_generator_delete_digger(digger->generator, left_digger);
        }
        
        struct digger *right_digger = dungeon_generator_copy_digger(digger->generator,
                                                                    digger);
        digger_turn_90_degrees_right(right_digger);
        struct area *right_passage = digger_dig_passage(right_digger, 3, wall_type_none);
        if (right_passage) {
            ++exit_count;
        } else {
            dungeon_generator_delete_digger(digger->generator, right_digger);
        }
        
        struct area *forward_passage = digger_dig_passage(digger, 3, wall_type_none);
        if (forward_passage) {
            ++exit_count;
        } else {
            dungeon_generator_delete_digger(digger->generator, digger);
        }
        
        if (0 == exit_count || 1 == exit_count) {
            // TODO: remove intersection if zero exits and add forward digger
            intersection->type = area_type_passage;
            // TODO: merge intersection area with adjacent passage
        }
    } else {
        // passage X's
    }
}


void
digger_generate_turn(struct digger *digger)
{
    int score = roll("1d20", digger->generator->rnd);
    if (score <= 8) {
        digger_turn_90_degrees_left(digger);
        digger_dig_passage(digger, 3, wall_type_none);
    } else if (score == 9) {
        // left 45 degrees ahead
    } else if (score == 10) {
        // left 45 degrees behind
    } else if (score <= 18) {
        digger_turn_90_degrees_right(digger);
        digger_dig_passage(digger, 3, wall_type_none);
    } else if (score == 19) {
        // right 45 degrees ahead
    } else {
        // right 45 degrees behind
    }
}


void
digger_move(struct digger *digger, int steps, enum direction direction)
{
    digger->point = point_move(digger->point, steps, direction);
}


void
digger_periodic_check(struct digger *digger)
{
    int score = roll("1d20", digger->generator->rnd);
    if (score <= 2) {
        digger_dig_passage(digger, 6, wall_type_none);
    } else if (score <= 5) {
        digger_generate_door(digger);
    } else if (score <= 10) {
        digger_generate_side_passage(digger);
    } else if (score <= 13) {
        digger_generate_turn(digger);
    } else if (score <= 16) {
        digger_generate_chamber(digger, wall_type_none);
    } else if (score == 17) {
        // stairs
    } else if (score == 18) {
        // dead end
        dungeon_generator_delete_digger(digger->generator, digger);
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


static void
location_of_door(struct rnd *rnd, bool *left, bool *right, bool *ahead)
{
    int score = roll("1d20", rnd);
    if (score <= 6) {
        *left = true;
    } else if (score <= 12) {
        *right = true;
    } else {
        *ahead = true;
    }
}


static void
space_beyond_door(struct digger *digger, bool is_straight_ahead)
{
    int score = roll("1d20", digger->generator->rnd);
    if (score <= 4) {
        // parallel passage or 10x10 room
        if (is_straight_ahead) {
            digger_dig_room(digger, 1, 1, 0, wall_type_door);
        } else {
            if (digger->generator->padding) {
                int const distance = digger->generator->padding;
                digger_dig_passage(digger, distance, wall_type_door);
                digger_dig_intersection(digger);
            } else {
                digger_dig_passage(digger, 1, wall_type_door);
            }
            
            struct digger *left_digger = dungeon_generator_copy_digger(digger->generator, digger);
            digger_turn_90_degrees_left(left_digger);
            digger_dig_passage(left_digger, 3, wall_type_none);
            
            digger_turn_90_degrees_right(digger);
            digger_dig_passage(digger, 3, wall_type_none);
        }
    } else if (score <= 8) {
        // passage straight ahead
        digger_dig_passage(digger, 6, wall_type_door);
    } else if (score == 9) {
        // passage 45 degrees ahead/behind
    } else if (score == 10) {
        // passage 45 degrees behind/ahead
    } else if (score <= 18) {
        if (is_straight_ahead || !digger->generator->padding) {
            digger_generate_room(digger, wall_type_door);
        } else if (digger->generator->padding) {
            int const distance = digger->generator->padding;
            digger_dig_passage(digger, distance, wall_type_door);
            digger_generate_room(digger, wall_type_none);
        }
    } else {
        if (is_straight_ahead || !digger->generator->padding) {
            digger_generate_chamber(digger, wall_type_door);
        } else {
            int const distance = digger->generator->padding;
            digger_dig_passage(digger, distance, wall_type_door);
            digger_generate_chamber(digger, wall_type_none);
        }
    }
}
