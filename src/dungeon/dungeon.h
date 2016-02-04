#ifndef FNF_DUNGEON_DUNGEON_H_INCLUDED
#define FNF_DUNGEON_DUNGEON_H_INCLUDED


struct area;
struct areas;
struct rnd;
struct tiles;


struct dungeon {
    struct area **areas;
    int areas_count;
    struct tiles *tiles;
};


struct dungeon *
dungeon_alloc(void);

void
dungeon_free(struct dungeon *dungeon);

void
dungeon_generate(struct dungeon *dungeon, struct rnd *rnd);

void
dungeon_generate_small(struct dungeon *dungeon);

void
dungeon_add_area(struct dungeon *dungeon, struct area *area);


#endif
