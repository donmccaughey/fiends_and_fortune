#include "areas.h"

#include <assert.h>

#include "common/alloc_or_die.h"

#include "area.h"


void
areas_add_area(struct areas *areas, struct area *area)
{
    if (areas->capacity == areas->count) {
        if (areas->capacity) {
            areas->capacity *= 2;
        } else {
            areas->capacity = 256;
        }
        areas->areas = reallocarray_or_die(areas->areas,
                                           areas->capacity,
                                           sizeof(struct area *));
    }
    areas->areas[areas->count] = area;
    ++areas->count;
}


struct area *
areas_area_at_index(struct areas *areas, size_t index)
{
    assert(index < areas->count);
    return areas->areas[index];
}


size_t
areas_count(struct areas const *areas)
{
    return areas->count;
}


struct areas *
areas_alloc(void)
{
    struct areas *areas = calloc_or_die(1, sizeof(struct areas));
    areas->areas = calloc_or_die(0, sizeof(struct area *));
    return areas;
}


void
areas_free(struct areas *areas)
{
    for (size_t i = 0; i < areas->count; ++i) {
        area_free(areas->areas[i]);
    }
    free_or_die(areas->areas);
    free_or_die(areas);
}
