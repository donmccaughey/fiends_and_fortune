#include "area.h"

#include "common/alloc_or_die.h"

#include "tiles.h"


struct area *
area_alloc(char const *description,
           struct tiles *parent_tiles,
           enum area_type type)
{
    struct area *area = malloc_or_die(sizeof(struct area));
    area->description = strdup_or_die(description);
    area->tiles = tiles_alloc_with_parent(parent_tiles);
    area->type = type;
    return area;
}


void
area_free(struct area *area)
{
    free_or_die(area->description);
    tiles_free(area->tiles);
    free_or_die(area);
}
