#ifndef DUNGEONS_AREAS_H_INCLUDED
#define DUNGEONS_AREAS_H_INCLUDED

#include <stddef.h>
#include "Range.h"


struct Area;
struct Areas;


/// The Areas struct takes ownership of the given area.
void addAreaToAreas(struct Areas *areas, struct Area *area);

size_t areasCount(struct Areas const *areas);

struct Areas *createAreas(void);

void destroyAreas(struct Areas *areas);


#endif /* #ifndef DUNGEONS_AREAS_H_INCLUDED */
