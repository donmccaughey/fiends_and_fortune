#include "periodic_check.h"

#include <stddef.h>

#include "common/dice.h"
#include "common/int.h"
#include "common/rnd.h"

#include "area.h"
#include "digger.h"
#include "exit.h"
#include "generator.h"
#include "wall_type.h"


static struct digger *
exit_location(struct digger *digger, struct area *chamber_or_room);

static struct digger *
exit_location_in_direction(struct digger *digger,
                           struct area *chamber_or_room,
                           enum direction direction);

static void
location_of_door(struct rnd *rnd, bool *left, bool *right, bool *ahead);

static void
number_of_exits(struct rnd *rnd,
                struct area *chamber_or_room,
                int *doors,
                int *passages,
                bool *check_for_secret_doors);

static bool
side_passages(struct digger *digger);

static bool
space_beyond_door(struct digger *digger, bool is_straight_ahead);

static bool
stairs(struct digger *digger);

static bool
stairs_down_one_level(struct digger *digger);

static bool
stairs_up_one_level(struct digger *digger);

static bool
turns(struct digger *digger);


static bool
chambers(struct digger *digger, enum wall_type entrance_type)
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
        length = 2;
        width = 3;
    } else if (score <= 15) {
        // rectangular 30x50
        length = 3;
        width = 5;
    } else if (score <= 17) {
        // rectangular 40x60
        length = 4;
        width = 6;
    } else {
        // unusual shape and size
        return false;
    }
    if (length != width) {
        int orientation = rnd_next_uniform_value(digger->generator->rnd, 2);
        if (orientation) swap(&length, &width);
    }
    
    int left_offsets[max(length, width)];
    fill_shuffled(digger->generator->rnd, left_offsets, width);
    for (int i = 0; i < width; ++i) {
        chamber = digger_dig_chamber(digger, length, width, left_offsets[i], entrance_type);
        if (chamber) break;
    }
    if (!chamber && length != width) {
        swap(&length, &width);
        fill_shuffled(digger->generator->rnd, left_offsets, width);
        for (int i = 0; i < width; ++i) {
            chamber = digger_dig_chamber(digger, length, width, left_offsets[i], entrance_type);
            if (chamber) break;
        }
    }
    if (!chamber) return false;
    // TODO: try smaller chambers?
    
    bool check_for_secret_doors = false;
    int door_count = 0;
    int passage_count = 0;
    number_of_exits(digger->generator->rnd,
                    chamber,
                    &door_count,
                    &passage_count,
                    &check_for_secret_doors);
    
    for (int i = 0; i < door_count; ++i) {
        // TODO: don't fail if no possible exit
        struct digger *exit_digger = exit_location(digger, chamber);
        if (!exit_digger) return false;
        if (!space_beyond_door(exit_digger, false)) return false;
    }
    
    for (int i = 0; i < passage_count; ++i) {
        // TODO: don't fail if no possible exit
        struct digger *exit_digger = exit_location(digger, chamber);
        if (!exit_digger) return false;
        // TODO: check exit direction
        if (!digger_dig_passage(exit_digger, 3, wall_type_none)) return false;
    }
    
    generator_delete_digger(digger->generator, digger);
    return true;
}


static bool
doors(struct digger *digger)
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
            // TODO: ensure a second location is chosen?
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


static struct digger *
exit_location(struct digger *digger, struct area *chamber_or_room)
{
    enum direction *directions;
    int score = roll("1d20", digger->generator->rnd);
    if (score <= 7) {
        // opposite wall
        directions = (enum direction[]){
            digger->direction,
            direction_opposite(digger->direction),
            direction_90_degrees_right(digger->direction),
            direction_90_degrees_left(digger->direction),
        };
    } else if (score <= 12) {
        // left wall
        directions = (enum direction[]){
            direction_90_degrees_left(digger->direction),
            direction_90_degrees_right(digger->direction),
            digger->direction,
            direction_opposite(digger->direction),
        };
    } else if (score <= 17) {
        // right wall
        directions = (enum direction[]){
            direction_90_degrees_right(digger->direction),
            direction_90_degrees_left(digger->direction),
            digger->direction,
            direction_opposite(digger->direction),
        };
    } else {
        // same wall
        directions = (enum direction[]){
            direction_opposite(digger->direction),
            digger->direction,
            direction_90_degrees_left(digger->direction),
            direction_90_degrees_right(digger->direction),
        };
    }
    for (int i = 0; i < 4; ++i) {
        struct digger *exit_digger = exit_location_in_direction(digger,
                                                                chamber_or_room,
                                                                directions[i]);
        if (exit_digger) return exit_digger;
    }
    return NULL;
}


static struct digger *
exit_location_in_direction(struct digger *digger,
                           struct area *chamber_or_room,
                           enum direction direction)
{
    int count = max(chamber_or_room->box.size.width, chamber_or_room->box.size.length);
    struct exit exits[count];
    count = possible_exits_in_direction(digger->generator,
                                        chamber_or_room->box,
                                        direction,
                                        exits,
                                        count);
    if (!count) return NULL;
    
    int index = rnd_next_uniform_value(digger->generator->rnd, count);
    struct digger *exit_digger = generator_add_digger(digger->generator,
                                                      exits[index].point,
                                                      direction);
    digger_move_forward(exit_digger, 1);
    return exit_digger;
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
number_of_exits(struct rnd *rnd,
                struct area *chamber_or_room,
                int *door_count,
                int *passage_count,
                bool *check_for_secret_doors)
{
    *door_count = 0;
    *passage_count = 0;
    int *default_exit_count;
    int *other_exit_count;
    if (area_type_room == chamber_or_room->type) {
        default_exit_count = door_count;
        other_exit_count = passage_count;
    } else {
        default_exit_count = passage_count;
        other_exit_count = door_count;
    }
    *check_for_secret_doors = false;
    int area = box_area(chamber_or_room->box);
    int score = roll("1d20", rnd);
    if (score <= 3) {
        *default_exit_count = (area <= 6) ? 1 : 2;
    } else if (score <= 6) {
        *default_exit_count = (area <= 6) ? 2 : 3;
    } else if (score <= 9) {
        *default_exit_count = (area <= 6) ? 3 : 4;
    } else if (score <= 12) {
        if (area <= 12) {
            *default_exit_count = 0;
            *check_for_secret_doors = true;
        } else {
            *default_exit_count = 1;
        }
    } else if (score <= 15) {
        if (area <= 16) {
            *default_exit_count = 0;
            *check_for_secret_doors = true;
        } else {
            *default_exit_count = 1;
        }
    } else if (score <= 18) {
        *default_exit_count = roll("1d4", rnd);
    } else {
        *other_exit_count = 1;
    }
}


bool
periodic_check(struct digger *digger)
{
    int score = roll("1d20", digger->generator->rnd);
    if (score <= 2) {
        return digger_dig_passage(digger, 6, wall_type_none);
    } else if (score <= 5) {
        return doors(digger);
    } else if (score <= 10) {
        return side_passages(digger);
    } else if (score <= 13) {
        return turns(digger);
    } else if (score <= 16) {
        return chambers(digger, wall_type_none);
    } else if (score == 17) {
        return stairs(digger);
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


static bool
rooms(struct digger *digger, enum wall_type entrance_type)
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
        length = 1;
        width = 2;
    } else if (score <= 13) {
        // rectangular 20x30
        length = 2;
        width = 3;
    } else if (score <= 15) {
        // rectangular 20x40
        length = 2;
        width = 4;
    } else if (score <= 17) {
        // rectangular 30x40
        length = 3;
        width = 4;
    } else {
        // unusual shape and size
        return false;
    }
    if (length != width) {
        int orientation = rnd_next_uniform_value(digger->generator->rnd, 2);
        if (orientation) swap(&length, &width);
    }
    
    int left_offsets[max(length, width)];
    fill_shuffled(digger->generator->rnd, left_offsets, width);
    for (int i = 0; i < width; ++i) {
        room = digger_dig_room(digger, length, width, left_offsets[i], entrance_type);
        if (room) break;
    }
    if (!room && length != width) {
        swap(&length, &width);
        fill_shuffled(digger->generator->rnd, left_offsets, width);
        for (int i = 0; i < width; ++i) {
            room = digger_dig_room(digger, length, width, left_offsets[i], entrance_type);
            if (room) break;
        }
    }
    if (!room) return false;
    // TODO: try smaller rooms?
    
    bool check_for_secret_doors = false;
    int door_count = 0;
    int passage_count = 0;
    number_of_exits(digger->generator->rnd,
                    room,
                    &door_count,
                    &passage_count,
                    &check_for_secret_doors);
    
    for (int i = 0; i < door_count; ++i) {
        // TODO: don't fail if no possible exit
        struct digger *exit_digger = exit_location(digger, room);
        if (!exit_digger) return false;
        if (!space_beyond_door(exit_digger, false)) return false;
    }
    
    for (int i = 0; i < passage_count; ++i) {
        // TODO: don't fail if no possible exit
        struct digger *exit_digger = exit_location(digger, room);
        if (!exit_digger) return false;
        // TODO: check exit direction
        if (!digger_dig_passage(exit_digger, 3, wall_type_none)) return false;
    }
    
    generator_delete_digger(digger->generator, digger);
    return true;
}


static bool
side_passages(struct digger *digger)
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
            if (!rooms(digger, wall_type_door)) return false;
        } else if (digger->generator->padding) {
            int const distance = digger->generator->padding;
            if (!digger_dig_passage(digger, distance, wall_type_door)) {
                return true;
            }
            if (!rooms(digger, wall_type_none)) return false;
        }
        return true;
    } else {
        if (is_straight_ahead || !digger->generator->padding) {
            if (!chambers(digger, wall_type_door)) return false;
        } else {
            int const distance = digger->generator->padding;
            if (!digger_dig_passage(digger, distance, wall_type_door)) {
                return false;
            }
            if (!chambers(digger, wall_type_none)) return false;
        }
        return true;
    }
}


static bool
stairs(struct digger *digger)
{
    int score = roll("1d20", digger->generator->rnd);
    if (score <= 5) {
        // down 1 level
        // 1 in 20 has a door which closes egress for the day
        return stairs_down_one_level(digger);
    } else if (score == 6) {
        // down 2 levels
        // 2 in 20 has a door which closes egress for the day
        if (!stairs_down_one_level(digger)) return false;
        // TODO: randomly determine space between staircases
        return stairs_down_one_level(digger);
    } else if (score == 7) {
        // down 3 levels
        // 3 in 20 has a door which closes egress for the day
        if (!stairs_down_one_level(digger)) return false;
        // TODO: randomly determine space between staircases
        if (!stairs_down_one_level(digger)) return false;
        // TODO: randomly determine space between staircases
        return stairs_down_one_level(digger);
    } else if (score == 8) {
        // up 1 level
        return stairs_up_one_level(digger);
    } else if (score == 9) {
        // up dead end (1 in 6 chance to chute down 2 levels)
        if (!stairs_up_one_level(digger)) return false;
        generator_delete_digger(digger->generator, digger);
        return true;
    } else if (score == 10) {
        // down dead end (1 in 6 chance to chute down 1 level)
        if (!stairs_down_one_level(digger)) return false;
        generator_delete_digger(digger->generator, digger);
        return true;
    } else if (score == 11) {
        // chimney up 1 level, passage continues, check again in 30’
        return false;
    } else if (score == 12) {
        // chimney up 2 levels, passage continues, check again in 30’
        return false;
    } else if (score == 13) {
        // chimney down 2 levels, passage continues, check again in 30’
        return false;
    } else if (score <= 16) {
        // trap door down 1 level, passage continues, check again in 30’
        return false;
    } else if (score == 17) {
        // trap door down 2 levels, passage continues, check again in 30’
        return false;
    } else {
        // up 1 then down 2 (total down 1), chamber at end
        if (digger->point.z == generator_min_level(digger->generator)) {
            return false;
        }
        if (!stairs_up_one_level(digger)) return false;
        // TODO: randomly determine space between staircases
        if (!stairs_down_one_level(digger)) return false;
        // TODO: randomly determine space between staircases
        if (!stairs_down_one_level(digger)) return false;
        return chambers(digger, wall_type_none);
    }
}


static bool
stairs_down_one_level(struct digger *digger)
{
    int next_level = digger->point.z + 1;
    if (next_level > generator_max_level(digger->generator)) return false;
    
    if (!digger_dig_stairs_down(digger, 2, wall_type_none)) return false;
    
    digger_descend(digger, 1);
    digger_move_backward(digger, 1);
    digger_spin_180_degrees(digger);
    if (!digger_dig_stairs_up(digger, 2, wall_type_none)) return false;
    digger_spin_180_degrees(digger);
    digger_move_forward(digger, 3);
    
    if (!digger_dig_passage(digger, 1, wall_type_none)) return false;
    return true;
}


static bool
stairs_up_one_level(struct digger *digger)
{
    if (!digger_dig_stairs_up(digger, 2, wall_type_none)) return false;
    if (digger->point.z == generator_min_level(digger->generator)) return true;
    
    digger_ascend(digger, 1);
    digger_move_backward(digger, 1);
    digger_spin_180_degrees(digger);
    if (!digger_dig_stairs_down(digger, 2, wall_type_none)) return false;
    digger_spin_180_degrees(digger);
    digger_move_forward(digger, 3);
    
    if (!digger_dig_passage(digger, 1, wall_type_none)) return false;
    return true;
}


static bool
turns(struct digger *digger)
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
