#include "Area.h"

#include <stdlib.h>

#include "earmark.h"
#include "heap.h"
#include "Tiles.h"


struct Area *createArea(char const *description, struct Tiles *parentTiles, enum AreaType type)
{
  struct Area *area = em_malloc(sizeof(struct Area));
  area->description = STRDUP_OR_DIE(description);
  area->tiles = createEmptyTilesWithParent(parentTiles);
  area->type = type;
  return area;
}


void destroyArea(struct Area *area)
{
  free(area->description);
  destroyTiles(area->tiles);
  free(area);
}
