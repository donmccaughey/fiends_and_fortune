#include "Area.h"

#include <stdlib.h>

#include "alloc_or_die.h"
#include "Tiles.h"


struct Area *createArea(char const *description, struct Tiles *parentTiles, enum AreaType type)
{
  struct Area *area = malloc_or_die(sizeof(struct Area));
  area->description = strdup_or_die(description);
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
