#ifndef FNF_DUNGEON_AREAS_H_INCLUDED
#define FNF_DUNGEON_AREAS_H_INCLUDED

#include <stddef.h>


struct area;
struct areas;


/// The areas struct takes ownership of the given area.
void
areas_append_area(struct areas *areas, struct area *area);

struct area *
areas_area_at_index(struct areas *areas, size_t index);

size_t
areas_count(struct areas const *areas);

struct areas *
areas_alloc(void);

void
areas_free(struct areas *areas);


#endif
