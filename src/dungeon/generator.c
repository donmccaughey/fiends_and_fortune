#include "generator.h"

#include <assert.h>

#include "common/alloc_or_die.h"
#include "common/dice.h"
#include "common/fail.h"
#include "common/str.h"

#include "area.h"
#include "areas.h"
#include "digger.h"
#include "direction.h"
#include "dungeon.h"
#include "point.h"
#include "range.h"
#include "reverse_range.h"
#include "tile.h"
#include "tiles.h"


static struct digger *
add_digger(struct generator *generator,
           struct point point,
           enum direction direction,
           dig_fn *dig);

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

static struct digger *
dup_digger(struct generator *generator, struct digger *digger);

static struct point
intersection(struct dungeon *dungeon,
             struct point from_point,
             enum direction direction);

static struct point
passage(struct dungeon *dungeon,
        struct point from_point,
        uint32_t distance,
        enum direction direction);

static void
periodic_check(struct generator *generator, struct digger *digger);

static void
remove_digger(struct generator *generator, struct digger *digger);

static void
side_passages(struct generator *generator, struct digger *digger);

static void
turns(struct generator *generator, struct digger *digger);


static struct digger *
add_digger(struct generator *generator,
           struct point point,
           enum direction direction,
           dig_fn *dig)
{
    int index = generator->diggers_count;
    ++generator->diggers_count;
    generator->diggers = reallocarray_or_die(generator->diggers,
                                             generator->diggers_count,
                                             sizeof(struct digger *));
    generator->diggers[index] = digger_alloc(point, direction, dig);
    return generator->diggers[index];
}


static void
add_new_empty_tile_to_dungeon_at(struct dungeon *dungeon,
                                 struct area *area,
                                 int32_t x,
                                 int32_t y,
                                 int32_t z)
{
    //assert(NULL == tiles_find_tile_at(dungeon->tiles, point_make(x, y, z)));
    
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
        case area_type_intersection:
            description = strdup_or_die("intersection");
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
    
    return point_move(from_point, length, direction);
}


static struct point
chamber(struct dungeon *dungeon,
        struct point from_point,
        uint32_t length,
        uint32_t width,
        enum direction direction,
        uint32_t left_offset)
{
    return area(dungeon, from_point, length, width, direction, left_offset, area_type_chamber);
}


static struct digger *
dup_digger(struct generator *generator, struct digger *digger)
{
    return add_digger(generator, digger->point, digger->direction, digger->dig);
}


struct generator *
generator_alloc(struct dungeon *dungeon, struct rnd *rnd)
{
    struct generator *generator = calloc_or_die(1, sizeof(struct generator));
    
    generator->dungeon = dungeon;
    generator->rnd = rnd;
    generator->diggers = calloc_or_die(1, sizeof(struct digger *));
    
    return generator;
}


void
generator_free(struct generator *generator)
{
    for (int i = 0; i < generator->diggers_count; ++i) {
        digger_free(generator->diggers[i]);
    }
    free_or_die(generator->diggers);
    free_or_die(generator);
}


void
generator_generate(struct generator *generator)
{
    int const max_interation_count = 5;
    
    struct point point = point_make(0, 0, 1);
    point = passage(generator->dungeon, point, 2, North);
    add_digger(generator, point, North, periodic_check);
    while (generator->diggers_count && generator->iteration_count < max_interation_count) {
        struct digger **diggers = arraydup_or_die(generator->diggers,
                                                  generator->diggers_count,
                                                  sizeof(struct digger *));
        int count = generator->diggers_count;
        for (int i = 0; i < count; ++i) {
            diggers[i]->dig(generator, diggers[i]);
        }
        free_or_die(diggers);
        ++generator->iteration_count;
    }
}


void
generator_generate_small(struct generator *generator)
{
    struct point point = point_make(0, 0, 1);
    
    point = passage(generator->dungeon, point, 2, North);
    
    point = chamber(generator->dungeon, point, 5, 3, North, 1);
    
    /* chamber exits */
    struct point point_in_chamber = point_move(point, 2, South);
    struct point north_west_exit = point_move(point_in_chamber, 2, West);
    struct point south_west_exit = point_move(north_west_exit, 2, South);
    struct point north_east_exit = point_move(point_in_chamber, 2, East);
    struct point south_east_exit = point_move(north_east_exit, 2, South);
    
    point = passage(generator->dungeon, point, 7, North);
    point = passage(generator->dungeon, point, 8, East);
    point = passage(generator->dungeon, point, 4, South);
    
    point = chamber(generator->dungeon, point, 3, 4, South, 0);
    point = point_move(point, 3, West);
    
    point = passage(generator->dungeon, point, 2, South);
    passage(generator->dungeon, point, 4, West);
    
    /* from entry chamber, north west exit */
    point = passage(generator->dungeon, north_west_exit, 1, West);
    point = passage(generator->dungeon, point, 7, North);
    
    chamber(generator->dungeon, point, 3, 2, North, 1);
    
    /* from entry chamber, south west exit */
    point = passage(generator->dungeon, south_west_exit, 1, West);
    point = passage(generator->dungeon, point, 2, South);
    point = passage(generator->dungeon, point, 2, West);
    point = passage(generator->dungeon, point, 3, North);
    
    point = chamber(generator->dungeon, point, 2, 2, North, 1);
    
    point = point_move(point, 1, West);
    point = passage(generator->dungeon, point, 3, North);
    
    chamber(generator->dungeon, point, 2, 3, North, 1);
    
    /* from entry chamber, south east exit */
    point = passage(generator->dungeon, south_east_exit, 1, East);
    
    chamber(generator->dungeon, point, 6, 4, East, 0);
    
    struct tile *tile = tiles_find_tile_at(generator->dungeon->tiles, point_make(5, 2, 1));
    tiles_remove_tile(generator->dungeon->tiles, tile);
    tile_free(tile);
}


static struct point
intersection(struct dungeon *dungeon,
             struct point from_point,
             enum direction direction)
{
    return area(dungeon, from_point, 1, 1, direction, 0, area_type_intersection);
}


static struct point
passage(struct dungeon *dungeon, struct point from_point, uint32_t distance, enum direction direction)
{
    return area(dungeon, from_point, distance, 1, direction, 0, area_type_passage);
}


static void
periodic_check(struct generator *generator, struct digger *digger)
{
    int score = roll("1d20", generator->rnd);
    if (score <= 2) {
        digger->point = passage(generator->dungeon, digger->point, 6, digger->direction);
    } else if (score <= 5) {
        // door
    } else if (score <= 10) {
        // side passage
        side_passages(generator, digger);
    } else if (score <= 13) {
        // passage turns
        turns(generator, digger);
    } else if (score <= 16) {
        // chamber
    } else if (score == 17) {
        // stairs
    } else if (score == 18) {
        // dead end
        remove_digger(generator, digger);
    } else if (score == 19) {
        // trick/trap
    } else {
        // wandering monster
    }
}


static void
remove_digger(struct generator *generator, struct digger *digger)
{
    int index = -1;
    for (int i = 0; i < generator->diggers_count; ++i) {
        if (digger == generator->diggers[i]) {
            index = i;
            break;
        }
    }
    assert(index >= 0 && index < generator->diggers_count);
    digger_free(digger);
    
    int next_index = index + 1;
    int last_index = generator->diggers_count - 1;
    if (next_index < last_index) {
        struct digger **vacant = &generator->diggers[index];
        struct digger **next = &generator->diggers[next_index];
        size_t size = (last_index - index) * sizeof(struct digger *);
        memmove(vacant, next, size);
    }
    --generator->diggers_count;
    generator->diggers = reallocarray_or_die(generator->diggers,
                                             generator->diggers_count,
                                             sizeof(struct digger *));
}


static void
side_passages(struct generator *generator, struct digger *digger)
{
    int score = roll("1d20", generator->rnd);
    if (score <= 2) {
        // left 90 degrees
        digger->point = intersection(generator->dungeon, digger->point, digger->direction);
        
        struct digger *side_digger = dup_digger(generator, digger);
        digger_turn_90_degrees_left(side_digger);
        side_digger->point = passage(generator->dungeon, side_digger->point, 3, side_digger->direction);
        
        digger->point = passage(generator->dungeon, digger->point, 3, digger->direction);
    } else if (score <= 4) {
        // right 90 degrees
        digger->point = intersection(generator->dungeon, digger->point, digger->direction);
        
        struct digger *side_digger = dup_digger(generator, digger);
        digger_turn_90_degrees_right(side_digger);
        side_digger->point = passage(generator->dungeon, side_digger->point, 3, side_digger->direction);
        
        digger->point = passage(generator->dungeon, digger->point, 3, digger->direction);
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
        digger->point = intersection(generator->dungeon, digger->point, digger->direction);
        
        struct digger *left_digger = dup_digger(generator, digger);
        digger_turn_90_degrees_left(left_digger);
        left_digger->point = passage(generator->dungeon, left_digger->point, 3, left_digger->direction);
        
        struct digger *right_digger = dup_digger(generator, digger);
        digger_turn_90_degrees_right(right_digger);
        right_digger->point = passage(generator->dungeon, right_digger->point, 3, right_digger->direction);
        
        remove_digger(generator, digger);
    } else if (score <= 15) {
        // passage Y's
    } else if (score < 19) {
        // four way intersection
        digger->point = intersection(generator->dungeon, digger->point, digger->direction);
        
        struct digger *left_digger = dup_digger(generator, digger);
        digger_turn_90_degrees_left(left_digger);
        left_digger->point = passage(generator->dungeon, left_digger->point, 3, left_digger->direction);
        
        struct digger *right_digger = dup_digger(generator, digger);
        digger_turn_90_degrees_right(right_digger);
        right_digger->point = passage(generator->dungeon, right_digger->point, 3, right_digger->direction);
        
        digger->point = passage(generator->dungeon, digger->point, 3, digger->direction);
    } else {
        // passage X's
    }
}


static void
turns(struct generator *generator, struct digger *digger)
{
    int score = roll("1d20", generator->rnd);
    if (score <= 8) {
        // left 90 degrees
        digger_turn_90_degrees_left(digger);
        digger->point = passage(generator->dungeon, digger->point, 3, digger->direction);
    } else if (score == 9) {
        // left 45 degrees ahead
    } else if (score == 10) {
        // left 45 degrees behind
    } else if (score <= 18) {
        // right 90 degrees
        digger_turn_90_degrees_right(digger);
        digger->point = passage(generator->dungeon, digger->point, 3, digger->direction);
    } else if (score == 19) {
        // right 45 degrees ahead
    } else {
        // right 45 degrees behind
    }
}
