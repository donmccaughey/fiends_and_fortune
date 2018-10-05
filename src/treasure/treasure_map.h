#ifndef FNF_TREASURE_TREASURE_MAP_H_INCLUDED
#define FNF_TREASURE_TREASURE_MAP_H_INCLUDED


#include <stdbool.h>

#include <treasure/treasure_struct.h>


struct cJSON;
struct rnd;


struct treasure_map {
    bool is_false;
    struct treasure treasure;
    char *true_description;
};


struct cJSON *
treasure_map_create_json_object(struct treasure_map *treasure_map);

void
treasure_map_finalize(struct treasure_map *treasure_map);

void
treasure_map_generate(struct treasure_map *treasure_map, struct rnd *rnd);

void
treasure_map_initialize(struct treasure_map *treasure_map);

void
treasure_map_initialize_from_json_object(struct treasure_map *treasure_map,
                                         struct cJSON *json_object);

#endif
