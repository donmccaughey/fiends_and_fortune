#ifndef FNF_TREASURE_TREASURE_MAP_H_INCLUDED
#define FNF_TREASURE_TREASURE_MAP_H_INCLUDED


#include <stdbool.h>

#include <treasure/treasure_struct.h>


struct rnd;


struct treasure_map {
    bool is_false;
    struct treasure treasure;
    char *true_description;
};


void
treasure_map_finalize(struct treasure_map *treasure_map);

void
treasure_map_generate(struct treasure_map *treasure_map, struct rnd *rnd);

void
treasure_map_initialize(struct treasure_map *treasure_map);


#endif
