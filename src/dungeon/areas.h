#ifndef FNF_DUNGEON_AREAS_H_INCLUDED
#define FNF_DUNGEON_AREAS_H_INCLUDED


struct area;


struct areas {
    struct area **areas;
    int capacity;
    int count;
};


void
areas_add_area(struct areas *areas, struct area *area);

struct area *
areas_area_at_index(struct areas *areas, int index);

int
areas_count(struct areas const *areas);

struct areas *
areas_alloc(void);

void
areas_free(struct areas *areas);


#endif
