#include "Areas.h"

#include <assert.h>
#include <stdlib.h>

#include "common/alloc_or_die.h"

#include "Area.h"


struct Areas {
    struct Area **areas;
    size_t capacity;
    size_t count;
};


void addAreaToAreas(struct Areas *areas, struct Area *area)
{
    if (areas->capacity == areas->count) {
        if (areas->capacity) {
            areas->capacity *= 2;
        } else {
            areas->capacity = 256;
        }
        areas->areas = reallocarray_or_die(areas->areas,
                                           areas->capacity,
                                           sizeof(struct Area *));
    }
    areas->areas[areas->count] = area;
    ++areas->count;
}


struct Area *areaInAreasAtIndex(struct Areas *areas, size_t index)
{
    assert(index < areas->count);
    return areas->areas[index];
}


size_t areasCount(struct Areas const *areas)
{
    return areas->count;
}


struct Areas *createAreas(void)
{
    struct Areas *areas = calloc_or_die(1, sizeof(struct Areas));
    areas->areas = calloc_or_die(0, sizeof(struct Area *));
    return areas;
}


void destroyAreas(struct Areas *areas)
{
    for (size_t i = 0; i < areas->count; ++i) {
        destroyArea(areas->areas[i]);
    }
    free_or_die(areas->areas);
    free_or_die(areas);
}
