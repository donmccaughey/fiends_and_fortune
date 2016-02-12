#include "digger.h"

#include <assert.h>

#include "common/alloc_or_die.h"
#include "common/dice.h"
#include "common/fail.h"
#include "common/rnd.h"

#include "area.h"
#include "generator.h"
#include "tile.h"


static bool
exit_location(struct digger *digger, int length, int width, int left_offset);

static void
location_of_door(struct rnd *rnd, bool *left, bool *right, bool *ahead);

static int
number_of_exits(struct rnd *rnd,
                struct area *chamber_or_room,
                bool *check_for_secret_doors);

static bool
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
    if (generator_is_box_excavated(digger->generator, box_to_dig)) {
        return NULL;
    }
    
    struct box box_for_level = generator_box_for_level(digger->generator,
                                                       digger->point.z);
    struct box new_box_for_level = box_for_level = box_make_from_boxes(box_for_level,
                                                                       box_to_dig);
    if (   new_box_for_level.size.width > digger->generator->max_size.width
        || new_box_for_level.size.length > digger->generator->max_size.length)
    {
        return NULL;
    }
    
    struct area *area = generator_add_area(digger->generator,
                                           area_type,
                                           orientation_from_direction(digger->direction),
                                           box_to_dig,
                                           tile_type_empty);
    
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
    digger_move_forward(digger, length);
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
    digger_move_forward(digger, length);
    return room;
}


bool
digger_chambers(struct digger *digger, enum wall_type entrance_type)
{
    int length = 0;
    int width = 0;
    struct area *chamber = NULL;
    int score = roll("1d20", digger->generator->rnd);
    if (score <= 2) {
        // square 20x20
        length = 2;
        width = 2;
    } else if (score <= 4) {
        // square 20x20
        length = 2;
        width = 2;
    } else if (score <= 6) {
        // square 30x30
        length = 3;
        width = 3;
    } else if (score <= 8) {
        // square 40x40
        length = 4;
        width = 4;
    } else if (score <= 10) {
        // rectangular 20x30
        length = 2;
        width = 3;
    } else if (score <= 13) {
        // rectangular 20x30
        length = 3;
        width = 2;
    } else if (score <= 15) {
        // rectangular 30x50
        int orientation = rnd_next_uniform_value(digger->generator->rnd, 2);
        if (orientation) {
            length = 5;
            width = 3;
        } else {
            length = 3;
            width = 5;
        }
    } else if (score <= 17) {
        // rectangular 40x60
        int orientation = rnd_next_uniform_value(digger->generator->rnd, 2);
        if (orientation) {
            length = 6;
            width = 4;
        } else {
            length = 4;
            width = 6;
        }
    } else {
        // unusual shape and size
        return false;
    }
    int left_offset = rnd_next_uniform_value(digger->generator->rnd, width);
    chamber = digger_dig_chamber(digger,
                                 length,
                                 width,
                                 left_offset,
                                 entrance_type);
    if (!chamber) return false;
    
    bool check_for_secret_doors = false;
    int exit_count = number_of_exits(digger->generator->rnd,
                                     chamber,
                                     &check_for_secret_doors);
    
    for (int i = 0; i < exit_count; ++i) {
        if (!exit_location(digger, length, width, left_offset)) return false;
    }
    
    generator_delete_digger(digger->generator, digger);
    return true;
}


bool
digger_doors(struct digger *digger)
{
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
        }
    }
    
    if (door_left) {
        struct digger *left_digger = generator_copy_digger(digger->generator,
                                                           digger);
        digger_turn_90_degrees_left(left_digger);
        if (!space_beyond_door(left_digger, false)) return false;
    }
    
    if (door_right) {
        struct digger *right_digger = generator_copy_digger(digger->generator,
                                                            digger);
        digger_turn_90_degrees_right(right_digger);
        if (!space_beyond_door(right_digger, false)) return false;
    }
    
    if (door_ahead) {
        if (!space_beyond_door(digger, true)) return false;
    } else {
        if (!digger_dig_passage(digger, 3, wall_type_none)) return false;
    }
    
    return true;
}


bool
digger_rooms(struct digger *digger, enum wall_type entrance_type)
{
    int length = 0;
    int width = 0;
    struct area *room = NULL;
    int score = roll("1d20", digger->generator->rnd);
    if (score <= 2) {
        // square 10x10
        length = 1;
        width = 1;
    } else if (score <= 4) {
        // square 20x20
        length = 2;
        width = 2;
    } else if (score <= 6) {
        // square 30x30
        length = 3;
        width = 3;
    } else if (score <= 8) {
        // square 40x40
        length = 4;
        width = 4;
    } else if (score <= 10) {
        // rectangular 10x20
        int orientation = rnd_next_uniform_value(digger->generator->rnd, 2);
        if (orientation) {
            length = 1;
            width = 2;
        } else {
            length = 2;
            width = 1;
        }
    } else if (score <= 13) {
        // rectangular 20x30
        int orientation = rnd_next_uniform_value(digger->generator->rnd, 2);
        if (orientation) {
            length = 3;
            width = 2;
        } else {
            length = 2;
            width = 3;
        }
    } else if (score <= 15) {
        // rectangular 20x40
        int orientation = rnd_next_uniform_value(digger->generator->rnd, 2);
        if (orientation) {
            length = 4;
            width = 2;
        } else {
            length = 2;
            width = 4;
        }
    } else if (score <= 17) {
        // rectangular 30x40
        int orientation = rnd_next_uniform_value(digger->generator->rnd, 2);
        if (orientation) {
            length = 4;
            width = 3;
        } else {
            length = 3;
            width = 4;
        }
    } else {
        // unusual shape and size
        return false;
    }
    int left_offset = rnd_next_uniform_value(digger->generator->rnd, width);
    room = digger_dig_room(digger, length, width, left_offset, entrance_type);
    if (!room) return false;
    
    bool check_for_secret_doors = false;
    int exit_count = number_of_exits(digger->generator->rnd,
                                     room,
                                     &check_for_secret_doors);
    
    for (int i = 0; i < exit_count; ++i) {
        if (!exit_location(digger, length, width, left_offset)) return false;
    }
    
    generator_delete_digger(digger->generator, digger);
    return true;
}


bool
digger_side_passages(struct digger *digger)
{
    int score = roll("1d20", digger->generator->rnd);
    if (score <= 2) {
        // left 90 degrees
        if (!digger_dig_intersection(digger)) return false;
        
        struct digger *side_digger = generator_copy_digger(digger->generator,
                                                           digger);
        digger_turn_90_degrees_left(side_digger);
        if (!digger_dig_passage(side_digger, 3, wall_type_none)) return false;
        
        if (!digger_dig_passage(digger, 3, wall_type_none)) return false;
        return true;
    } else if (score <= 4) {
        // right 90 degrees
        if (!digger_dig_intersection(digger)) return false;
        
        struct digger *side_digger = generator_copy_digger(digger->generator,
                                                           digger);
        digger_turn_90_degrees_right(side_digger);
        if (!digger_dig_passage(side_digger, 3, wall_type_none)) return false;
        
        if (!digger_dig_passage(digger, 3, wall_type_none)) return false;
        return true;
    } else if (score == 5) {
        // left 45 degrees ahead
        return true;
    } else if (score == 6) {
        // right 45 degrees ahead
        return true;
    } else if (score == 7) {
        // left 45 degrees behind
        return true;
    } else if (score == 8) {
        // right 45 degrees behind
        return true;
    } else if (score == 9) {
        // left curve 45 degrees ahead
        return true;
    } else if (score == 10) {
        // right curve 45 degrees ahead
        return true;
    } else if (score <= 13) {
        // passage T's
        struct area *intersection = digger_dig_intersection(digger);
        if (!intersection) return false;
        
        struct digger *left_digger = generator_copy_digger(digger->generator,
                                                           digger);
        digger_turn_90_degrees_left(left_digger);
        if (!digger_dig_passage(left_digger, 3, wall_type_none)) return false;
        
        struct digger *right_digger = generator_copy_digger(digger->generator,
                                                            digger);
        digger_turn_90_degrees_right(right_digger);
        if (!digger_dig_passage(right_digger, 3, wall_type_none)) return false;
        
        generator_delete_digger(digger->generator, digger);
        return true;
    } else if (score <= 15) {
        // passage Y's
        return true;
    } else if (score < 19) {
        // four way intersection
        struct area *intersection = digger_dig_intersection(digger);
        if (!intersection) return false;
        
        struct digger *left_digger = generator_copy_digger(digger->generator,
                                                           digger);
        digger_turn_90_degrees_left(left_digger);
        if (!digger_dig_passage(left_digger, 3, wall_type_none)) return false;
        
        struct digger *right_digger = generator_copy_digger(digger->generator,
                                                            digger);
        digger_turn_90_degrees_right(right_digger);
        if (!digger_dig_passage(right_digger, 3, wall_type_none)) return false;
        
        if (!digger_dig_passage(digger, 3, wall_type_none)) return false;
        return true;
    } else {
        // passage X's
        return true;
    }
}


bool
digger_turns(struct digger *digger)
{
    int score = roll("1d20", digger->generator->rnd);
    if (score <= 8) {
        digger_turn_90_degrees_left(digger);
        return digger_dig_passage(digger, 3, wall_type_none);
    } else if (score == 9) {
        // left 45 degrees ahead
        return true;
    } else if (score == 10) {
        // left 45 degrees behind
        return true;
    } else if (score <= 18) {
        digger_turn_90_degrees_right(digger);
        return digger_dig_passage(digger, 3, wall_type_none);
    } else if (score == 19) {
        // right 45 degrees ahead
        return true;
    } else {
        // right 45 degrees behind
        return true;
    }
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


bool
digger_periodic_check(struct digger *digger)
{
    int score = roll("1d20", digger->generator->rnd);
    if (score <= 2) {
        return digger_dig_passage(digger, 6, wall_type_none);
    } else if (score <= 5) {
        return digger_doors(digger);
    } else if (score <= 10) {
        return digger_side_passages(digger);
    } else if (score <= 13) {
        return digger_turns(digger);
    } else if (score <= 16) {
        return digger_chambers(digger, wall_type_none);
    } else if (score == 17) {
        // stairs
        return true;
    } else if (score == 18) {
        // dead end
        generator_delete_digger(digger->generator, digger);
        return true;
    } else if (score == 19) {
        // trick/trap
        return true;
    } else {
        // wandering monster
        return true;
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


static bool
exit_location(struct digger *digger, int length, int width, int left_offset)
{
    int score = roll("1d20", digger->generator->rnd);
    if (score <= 7) {
        // opposite wall
        struct digger *exit_digger = generator_copy_digger(digger->generator, digger);
        digger_move_left(exit_digger, left_offset);
        int exit_offset = rnd_next_uniform_value(digger->generator->rnd,
                                                 width);
        digger_move_right(exit_digger, exit_offset);
        if (!space_beyond_door(exit_digger, false)) return false;
    } else if (score <= 12) {
        // left wall
        struct digger *exit_digger = generator_copy_digger(digger->generator, digger);
        digger_move_left(exit_digger, left_offset);
        digger_turn_90_degrees_left(exit_digger);
        int exit_offset = rnd_next_uniform_value(digger->generator->rnd,
                                                 length);
        digger_move_left(exit_digger, exit_offset);
        if (!space_beyond_door(exit_digger, false)) return false;
    } else if (score <= 17) {
        // right wall
        struct digger *exit_digger = generator_copy_digger(digger->generator, digger);
        digger_move_right(exit_digger, width - left_offset - 1);
        digger_turn_90_degrees_right(exit_digger);
        int exit_offset = rnd_next_uniform_value(digger->generator->rnd,
                                                 length);
        digger_move_right(exit_digger, exit_offset);
        if (!space_beyond_door(exit_digger, false)) return false;
    } else {
        // same wall
        return false;
    }
    return true;
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


static int
number_of_exits(struct rnd *rnd,
                struct area *chamber_or_room,
                bool *check_for_secret_doors)
{
    int exit_count = 0;
    *check_for_secret_doors = false;
    int area = box_area(chamber_or_room->box);
    int score = roll("1d20", rnd);
    if (score <= 3) {
        exit_count = (area <= 6) ? 1 : 2;
    } else if (score <= 6) {
        exit_count = (area <= 6) ? 2 : 3;
    } else if (score <= 9) {
        exit_count = (area <= 6) ? 3 : 4;
    } else if (score <= 12) {
        if (area <= 12) {
            exit_count = 0;
            *check_for_secret_doors = true;
        } else {
            exit_count = 1;
        }
    } else if (score <= 15) {
        if (area <= 16) {
            exit_count = 0;
            *check_for_secret_doors = true;
        } else {
            exit_count = 1;
        }
    } else if (score <= 18) {
        exit_count = roll("1d4", rnd);
    } else {
        exit_count = 1;
    }
    return exit_count;
}


static bool
space_beyond_door(struct digger *digger, bool is_straight_ahead)
{
    int score = roll("1d20", digger->generator->rnd);
    if (score <= 4) {
        // parallel passage or 10x10 room
        if (is_straight_ahead) {
            if (!digger_dig_room(digger, 1, 1, 0, wall_type_door)) return true;
        } else {
            if (digger->generator->padding) {
                int const distance = digger->generator->padding;
                if (!digger_dig_passage(digger, distance, wall_type_door)) {
                    return false;
                }
                if (!digger_dig_intersection(digger)) return false;
            } else {
                if (!digger_dig_passage(digger, 1, wall_type_door)) {
                    return false;
                }
            }
            
            struct digger *left_digger = generator_copy_digger(digger->generator,
                                                               digger);
            digger_turn_90_degrees_left(left_digger);
            if (!digger_dig_passage(left_digger, 3, wall_type_none)) {
                return false;
            }
            
            digger_turn_90_degrees_right(digger);
            if (!digger_dig_passage(digger, 3, wall_type_none)) return false;
        }
        return true;
    } else if (score <= 8) {
        // passage straight ahead
        if (!digger_dig_passage(digger, 3, wall_type_door)) return false;
        return true;
    } else if (score == 9) {
        // passage 45 degrees ahead/behind
        return true;
    } else if (score == 10) {
        // passage 45 degrees behind/ahead
        return true;
    } else if (score <= 18) {
        if (is_straight_ahead || !digger->generator->padding) {
            if (!digger_rooms(digger, wall_type_door)) return false;
        } else if (digger->generator->padding) {
            int const distance = digger->generator->padding;
            if (!digger_dig_passage(digger, distance, wall_type_door)) {
                return true;
            }
            if (!digger_rooms(digger, wall_type_none)) return false;
        }
        return true;
    } else {
        if (is_straight_ahead || !digger->generator->padding) {
            if (!digger_chambers(digger, wall_type_door)) return false;
        } else {
            int const distance = digger->generator->padding;
            if (!digger_dig_passage(digger, distance, wall_type_door)) {
                return false;
            }
            if (!digger_chambers(digger, wall_type_none)) return false;
        }
        return true;
    }
}
