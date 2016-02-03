#include "areas.h"

#include <assert.h>

#include "common/alloc_or_die.h"

#include "area.h"


void
areas_add_area(struct areas *areas, struct area *area)
{
    int index = areas->count;
    ++areas->count;
    areas->members = reallocarray_or_die(areas->members, areas->count, sizeof(struct area *));
    areas->members[index] = area;
}


struct areas *
areas_alloc(void)
{
    struct areas *areas = calloc_or_die(1, sizeof(struct areas));
    areas->members = calloc_or_die(1, sizeof(struct area *));
    return areas;
}


void
areas_free(struct areas *areas)
{
    for (size_t i = 0; i < areas->count; ++i) {
        area_free(areas->members[i]);
    }
    free_or_die(areas->members);
    free_or_die(areas);
}
