#include "exit.h"

#include <assert.h>
#include <stddef.h>
#include <base/base.h>

#include "generator.h"
#include "tile.h"


int
exits(struct generator *generator,
      struct box box,
      struct exit *exits,
      int exits_count)
{
    int total = 0;
    int count = 0;
    int available = exits_count;
    
    count = exits_north(generator, box, exits, available);
    available -= (count > available) ? available : count;
    total += count;
    
    count = exits_south(generator, box, exits, available);
    available -= (count > available) ? available : count;
    total += count;
    
    count = exits_east(generator, box, exits, available);
    available -= (count > available) ? available : count;
    total += count;
    
    count = exits_west(generator, box, exits, available);
    available -= (count > available) ? available : count;
    total += count;
    
    return total;
}


int
exits_east(struct generator *generator,
           struct box box,
           struct exit *exits,
           int exits_count)
{
    assert(1 == box.size.height);
    int count = 0;
    struct point end = box_end_point(box);
    for (int j = 0; j < box.size.length; ++j) {
        struct point point = point_make(end.x, box.origin.y + j, box.origin.z);
        struct tile *outside_tile = generator_tile_at(generator, point);
        struct tile *inside_tile = generator_tile_at(generator, point_west(point));
        if (   tile_is_escavated(outside_tile)
            && tile_is_escavated(inside_tile)
            && tile_has_west_exit(outside_tile))
        {
            int index = count;
            ++count;
            if (exits && index < exits_count) {
                exits[index].direction = direction_east;
                exits[index].point = inside_tile->point;
                exits[index].type = outside_tile->walls.west;
            }
        }
    }
    return count;
}


int
exits_in_direction(struct generator *generator,
                   struct box box,
                   enum direction direction,
                   struct exit *exits,
                   int exits_count)
{
    switch (direction) {
        case direction_north:
            return exits_north(generator, box, exits, exits_count);
        case direction_south:
            return exits_south(generator, box, exits, exits_count);
        case direction_east:
            return exits_east(generator, box, exits, exits_count);
        case direction_west:
            return exits_west(generator, box, exits, exits_count);
        default:
            unreachable("Unrecognized direction %i", direction);
            return 0;
    }
}


int
exits_north(struct generator *generator,
            struct box box,
            struct exit *exits,
            int exits_count)
{
    assert(1 == box.size.height);
    int count = 0;
    struct point end = box_end_point(box);
    for (int i = 0; i < box.size.width; ++i) {
        struct point point = point_make(box.origin.x + i, end.y, box.origin.z);
        struct tile *outside_tile = generator_tile_at(generator, point);
        struct tile *inside_tile = generator_tile_at(generator, point_south(point));
        if (   tile_is_escavated(outside_tile)
            && tile_is_escavated(inside_tile)
            && tile_has_south_exit(outside_tile))
        {
            int index = count;
            ++count;
            if (exits && index < exits_count) {
                exits[index].direction = direction_north;
                exits[index].point = inside_tile->point;
                exits[index].type = outside_tile->walls.south;
            }
        }
    }
    return count;
}


int
exits_south(struct generator *generator,
            struct box box,
            struct exit *exits,
            int exits_count)
{
    assert(1 == box.size.height);
    int count = 0;
    for (int i = 0; i < box.size.width; ++i) {
        struct point point = point_make(box.origin.x + i, box.origin.y, box.origin.z);
        struct tile *inside_tile = generator_tile_at(generator, point);
        struct tile *outside_tile = generator_tile_at(generator, point_south(point));
        if (   tile_is_escavated(outside_tile)
            && tile_is_escavated(inside_tile)
            && tile_has_south_exit(inside_tile))
        {
            int index = count;
            ++count;
            if (exits && index < exits_count) {
                exits[index].direction = direction_south;
                exits[index].point = inside_tile->point;
                exits[index].type = inside_tile->walls.south;
            }
        }
    }
    return count;
}


int
exits_west(struct generator *generator,
           struct box box,
           struct exit *exits,
           int exits_count)
{
    assert(1 == box.size.height);
    int count = 0;
    for (int j = 0; j < box.size.length; ++j) {
        struct point point = point_make(box.origin.x, box.origin.y + j, box.origin.z);
        struct tile *inside_tile = generator_tile_at(generator, point);
        struct tile *outside_tile = generator_tile_at(generator, point_west(point));
        if (   tile_is_escavated(outside_tile)
            && tile_is_escavated(inside_tile)
            && tile_has_west_exit(inside_tile))
        {
            int index = count;
            ++count;
            if (exits && index < exits_count) {
                exits[index].direction = direction_west;
                exits[index].point = inside_tile->point;
                exits[index].type = inside_tile->walls.west;
            }
        }
    }
    return count;
}


int
possible_exits(struct generator *generator,
               struct box box,
               struct exit *exits,
               int exits_count)
{
    int total = 0;
    int count = 0;
    int available = exits_count;
    
    count = possible_exits_north(generator, box, exits, available);
    available -= (count > available) ? available : count;
    total += count;
    
    count = possible_exits_south(generator, box, exits, available);
    available -= (count > available) ? available : count;
    total += count;
    
    count = possible_exits_east(generator, box, exits, available);
    available -= (count > available) ? available : count;
    total += count;
    
    count = possible_exits_west(generator, box, exits, available);
    available -= (count > available) ? available : count;
    total += count;
    
    return total;
}


int
possible_exits_east(struct generator *generator,
                    struct box box,
                    struct exit *exits,
                    int exits_count)
{
    int count = 0;
    struct point end = box_end_point(box);
    for (int j = 0; j < box.size.length; ++j) {
        struct point point = point_make(end.x, box.origin.y + j, box.origin.z);
        struct tile *outside_tile = generator_tile_at(generator, point);
        struct tile *inside_tile = generator_tile_at(generator, point_west(point));
        if (   tile_is_unescavated(outside_tile)
            && tile_is_escavated(inside_tile))
        {
            int index = count;
            ++count;
            if (exits && index < exits_count) {
                exits[index].direction = direction_east;
                exits[index].point = inside_tile->point;
                exits[index].type = inside_tile->walls.west;
            }
        }
    }
    return count;
}


int
possible_exits_in_direction(struct generator *generator,
                            struct box box,
                            enum direction direction,
                            struct exit *exits,
                            int exits_count)
{
    switch (direction) {
        case direction_north:
            return possible_exits_north(generator, box, exits, exits_count);
        case direction_south:
            return possible_exits_south(generator, box, exits, exits_count);
        case direction_east:
            return possible_exits_east(generator, box, exits, exits_count);
        case direction_west:
            return possible_exits_west(generator, box, exits, exits_count);
        default:
            unreachable("Unrecognized direction %i", direction);
            return 0;
    }
}


int
possible_exits_north(struct generator *generator,
                     struct box box,
                     struct exit *exits,
                     int exits_count)
{
    assert(1 == box.size.height);
    int count = 0;
    struct point end = box_end_point(box);
    for (int i = 0; i < box.size.width; ++i) {
        struct point point = point_make(box.origin.x + i, end.y, box.origin.z);
        struct tile *outside_tile = generator_tile_at(generator, point);
        struct tile *inside_tile = generator_tile_at(generator, point_south(point));
        if (   tile_is_unescavated(outside_tile)
            && tile_is_escavated(inside_tile))
        {
            int index = count;
            ++count;
            if (exits && index < exits_count) {
                exits[index].direction = direction_north;
                exits[index].point = inside_tile->point;
                exits[index].type = outside_tile->walls.south;
            }
        }
    }
    return count;
}


int
possible_exits_south(struct generator *generator,
                     struct box box,
                     struct exit *exits,
                     int exits_count)
{
    assert(1 == box.size.height);
    int count = 0;
    for (int i = 0; i < box.size.width; ++i) {
        struct point point = point_make(box.origin.x + i, box.origin.y, box.origin.z);
        struct tile *inside_tile = generator_tile_at(generator, point);
        struct tile *outside_tile = generator_tile_at(generator, point_south(point));
        if (   tile_is_unescavated(outside_tile)
            && tile_is_escavated(inside_tile))
        {
            int index = count;
            ++count;
            if (exits && index < exits_count) {
                exits[index].direction = direction_south;
                exits[index].point = inside_tile->point;
                exits[index].type = inside_tile->walls.south;
            }
        }
    }
    return count;
}


int
possible_exits_west(struct generator *generator,
                    struct box box,
                    struct exit *exits,
                    int exits_count)
{
    assert(1 == box.size.height);
    int count = 0;
    for (int j = 0; j < box.size.length; ++j) {
        struct point point = point_make(box.origin.x, box.origin.y + j, box.origin.z);
        struct tile *inside_tile = generator_tile_at(generator, point);
        struct tile *outside_tile = generator_tile_at(generator, point_west(point));
        if (   tile_is_unescavated(outside_tile)
            && tile_is_escavated(inside_tile))
        {
            int index = count;
            ++count;
            if (exits && index < exits_count) {
                exits[index].direction = direction_west;
                exits[index].point = inside_tile->point;
                exits[index].type = inside_tile->walls.west;
            }
        }
    }
    return count;
}
