#include "Areas.h"

#include <stdlib.h>
#include "Area.h"
#include "heap.h"


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
    areas->areas = REALLOC_OR_DIE(areas->areas, areas->capacity * sizeof(struct Area *));
  }
  areas->areas[areas->count] = area;
  ++areas->count;
}


size_t areasCount(struct Areas const *areas)
{
  return areas->count;
}


struct Areas *createAreas(void)
{
  struct Areas *areas = CALLOC_OR_DIE(1, sizeof(struct Areas));
  areas->areas = CALLOC_OR_DIE(0, sizeof(struct Area *));
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
