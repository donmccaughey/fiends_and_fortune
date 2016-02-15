#include "area.h"

#include "common/alloc_or_die.h"
#include "common/int.h"
#include "common/str.h"

#include "orientation.h"
#include "tile.h"


static char *
alloc_level_description(int level)
{
    if (level <= 0) {
        return strdup_or_die("surface");
    } else {
        return str_alloc_formatted("level %i", level);
    }
}

static char *
alloc_stairs_description(int level, enum area_type area_type)
{
    char *level_description = alloc_level_description(level);
    char const *direction = area_type_stairs_down == area_type ? "down" : "up";
    
    char *description = str_alloc_formatted("stairs %s to %s",
                                            direction, level_description);
    
    free_or_die(level_description);
    return description;
}


struct area *
area_alloc(enum area_type area_type,
           enum direction direction,
           struct box box,
           enum tile_type tile_type)
{
    struct area *area = calloc_or_die(1, sizeof(struct area));
    area->box = box;
    area->direction = direction;
    area->type = area_type;
    return area;
}


char *
area_alloc_description(struct area const *area)
{
    int width = area->box.size.width * 10;
    int length = area->box.size.length * 10;
    int level = area->box.origin.z;
    switch (area->type) {
        case area_type_chamber:
            return str_alloc_formatted("%u' x %u' chamber", width, length);
        case area_type_intersection:
            return strdup_or_die("intersection");
        case area_type_passage: {
            enum orientation orientation = orientation_from_direction(area->direction);
            if (orientation_east_to_west == orientation) swap(&width, &length);
            return str_alloc_formatted("%u' passage %s", length,
                                       orientation_name(orientation));
        }
        case area_type_room:
            return str_alloc_formatted("%u' x %u' room", width, length);
        case area_type_stairs_down:
            return alloc_stairs_description(level + 1, area_type_stairs_down);
        case area_type_stairs_up:
            return alloc_stairs_description(level - 1, area_type_stairs_up);
        default:
            return str_alloc_formatted("%u' x %u' area", width, length);
    }
}


struct point
area_center_point(struct area const *area)
{
    int x = area->box.origin.x + ((area->box.size.width - 1) / 2);
    int y = area->box.origin.y + ((area->box.size.length - 1) / 2);
    int z = area->box.origin.z + ((area->box.size.height - 1) / 2);
    return point_make(x, y, z);
}


void
area_free(struct area *area)
{
    free_or_die(area);
}


bool
area_is_interesting(struct area const *area)
{
    if (area_type_chamber ==  area->type) return true;
    if (area_type_room ==  area->type) return true;
    if (area_type_stairs_down ==  area->type) return true;
    if (area_type_stairs_up ==  area->type) return true;
    return false;
}
