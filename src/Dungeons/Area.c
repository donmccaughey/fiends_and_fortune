#include "Area.h"

#include <stdlib.h>

#include "common/alloc_or_die.h"

#include "tiles.h"


struct Area *createArea(char const *description, struct tiles *parentTiles, enum AreaType type)
{
    struct Area *area = malloc_or_die(sizeof(struct Area));
    area->description = strdup_or_die(description);
    area->tiles = tiles_alloc_with_parent(parentTiles);
    area->type = type;
    return area;
}


void destroyArea(struct Area *area)
{
    free_or_die(area->description);
    tiles_free(area->tiles);
    free_or_die(area);
}
