#include "Areas.h"

#include <assert.h>
#include <stdlib.h>

#include "Area.h"
#include "earmark.h"


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
    areas->areas = em_realloc(areas->areas, areas->capacity * sizeof(struct Area *));
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
  struct Areas *areas = em_calloc(1, sizeof(struct Areas));
  areas->areas = em_calloc(0, sizeof(struct Area *));
  return areas;
}


void destroyAreas(struct Areas *areas)
{
  for (size_t i = 0; i < areas->count; ++i) {
    destroyArea(areas->areas[i]);
  }
  free(areas->areas);
  free(areas);
}
