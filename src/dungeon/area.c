#include "area.h"

#include <base/base.h>

#include "astr.h"
#include "xstring.h"


static void
realloc_append_level_description(char **s, int level);


static char *
alloc_stairs_description(int level, enum area_type area_type)
{
    char const *direction = area_type_stairs_down == area_type ? "down" : "up";
    char *description = astr_f("stairs %s to ", direction);
    realloc_append_level_description(&description, level);
    return description;
}


struct area *
area_alloc(enum area_type area_type,
           enum direction direction,
           struct box box)
{
    struct area *area = xcalloc(1, sizeof(struct area));
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
            description = astr_f("%u' x %u' chamber", width, length);
            break;
        case area_type_intersection:
            description = xstrdup("intersection");
            break;
        case area_type_passage: {
            enum orientation orientation = orientation_from_direction(area->direction);
            if (orientation_east_to_west == orientation) swap(&width, &length);
            description = astr_f("%u' passage %s", length,
                                              orientation_name(orientation));
            break;
        }
        case area_type_room:
            description = astr_f("%u' x %u' room", width, length);
            break;
        case area_type_stairs_down:
            description = alloc_stairs_description(level + 1, area_type_stairs_down);
            break;
        case area_type_stairs_up:
            description = alloc_stairs_description(level - 1, area_type_stairs_up);
            break;
        default:
            description = astr_f("%u' x %u' area", width, length);
            break;
    }
    if (!area->features) return description;
    
    if (area->features & area_features_chimney_up) {
        description = astr_cat_f(description, ", chimney up to ");
        realloc_append_level_description(&description, level - 1);
    }
    
    if (area->features & area_features_chimney_down) {
        description = astr_cat_f(description, ", chimney down to ");
        realloc_append_level_description(&description, level + 1);
    }
    
    if (area->features & area_features_chute_entrance) {
        description = astr_cat_f(description, ", chute down to ");
        realloc_append_level_description(&description, level + 1);
    }
    
    if (area->features & area_features_chute_exit) {
        description = astr_cat_f(description, ", chute down from ");
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
    free(area);
}


bool
area_is_chamber_or_room(struct area const *area)
{
    if (area_type_chamber == area->type) return true;
    if (area_type_room == area->type) return true;
    return false;
}


bool
area_is_interesting(struct area const *area)
{
    return area_is_chamber_or_room(area)
        || area_is_level_transition(area);
}


bool
area_is_level_transition(struct area const *area)
{
    if (area_type_stairs_down == area->type) return true;
    if (area_type_stairs_up == area->type) return true;
    if (area->features) return true;
    return false;
}


static void
realloc_append_level_description(char **s, int level)
{
    if (level <= 0) {
        *s = astr_cat_f(*s, "surface");
    } else {
        *s = astr_cat_f(*s, "level %i", level);
    }
}
