#ifndef FNF_DUNGEON_AREAS_H_INCLUDED
#define FNF_DUNGEON_AREAS_H_INCLUDED


struct area;


struct areas {
    struct area **members;
    int count;
};


struct areas *
areas_alloc(void);

void
areas_free(struct areas *areas);

void
areas_add_area(struct areas *areas, struct area *area);


#endif
