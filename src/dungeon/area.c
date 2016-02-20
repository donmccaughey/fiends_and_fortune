#include "area.h"

#include "common/alloc_or_die.h"
#include "common/int.h"
#include "common/str.h"

#include "orientation.h"
#include "tile.h"


static void
realloc_append_level_description(char **s, int level);


static char *
alloc_stairs_description(int level, enum area_type area_type)
{
    char const *direction = area_type_stairs_down == area_type ? "down" : "up";
    char *description = str_alloc_formatted("stairs %s to ", direction);
    realloc_append_level_description(&description, level);
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
    char *description;
    switch (area->type) {
        case area_type_chamber:
            description = str_alloc_formatted("%u' x %u' chamber", width, length);
            break;
        case area_type_intersection:
            description = strdup_or_die("intersection");
            break;
        case area_type_passage: {
            enum orientation orientation = orientation_from_direction(area->direction);
            if (orientation_east_to_west == orientation) swap(&width, &length);
            description = str_alloc_formatted("%u' passage %s", length,
                                              orientation_name(orientation));
            break;
        }
        case area_type_room:
            description = str_alloc_formatted("%u' x %u' room", width, length);
            break;
        case area_type_stairs_down:
            description = alloc_stairs_description(level + 1, area_type_stairs_down);
            break;
        case area_type_stairs_up:
            description = alloc_stairs_description(level - 1, area_type_stairs_up);
            break;
        default:
            description = str_alloc_formatted("%u' x %u' area", width, length);
            break;
    }
    if (!area->features) return description;
    
    if (area->features & area_features_chimney_up) {
        str_realloc_append_formatted(&description, ", chimney up to ");
        realloc_append_level_description(&description, level - 1);
    }
    
    if (area->features & area_features_chimney_down) {
        str_realloc_append_formatted(&description, ", chimney down to ");
        realloc_append_level_description(&description, level + 1);
    }
    
    if (area->features & area_features_chute_entrance) {
        str_realloc_append_formatted(&description, ", chute down to ");
        realloc_append_level_description(&description, level + 1);
    }
    
    if (area->features & area_features_chute_exit) {
        str_realloc_append_formatted(&description, ", chute down from ");
        realloc_append_level_description(&description, level - 1);
    }
    
    return description;
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
    if (area->features) return true;
    return false;
}


static void
realloc_append_level_description(char **s, int level)
{
    if (level <= 0) {
        str_realloc_append_formatted(s, "surface");
    } else {
        str_realloc_append_formatted(s, "level %i", level);
    }
}
