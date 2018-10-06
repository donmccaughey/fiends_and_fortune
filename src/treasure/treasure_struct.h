#ifndef FNF_TREASURE_STRUCT_H_INCLUDED
#define FNF_TREASURE_STRUCT_H_INCLUDED


#include <treasure/coins.h>
#include <treasure/magic_item.h>


struct cJSON;
struct gem;
struct jewelry;
struct magic_item;
struct ptr_array;
struct rnd;
struct treasure_map;
struct treasure_type;


struct treasure {
    struct treasure_type *type;
    struct coins coins;
    struct gem *gems;
    int gems_count;
    struct jewelry *jewelry;
    int jewelry_count;
    struct treasure_map *maps;
    int maps_count;
    struct magic_item *magic_items;
    int magic_items_count;
};


char *
treasure_alloc_description(struct treasure *treasure);

struct ptr_array *
treasure_alloc_details(struct treasure *treasure);

struct cJSON *
treasure_create_json_object(struct treasure *treasure);

void
treasure_finalize(struct treasure *treasure);

void
treasure_generate_magic_items(struct treasure *treasure,
                              struct rnd *rnd,
                              int count,
                              possible_magic_items_t possible_magic_items);

void
treasure_generate_maps(struct treasure *treasure, struct rnd *rnd, int count);

void
treasure_initialize(struct treasure *treasure);

void
treasure_initialize_from_json_object(struct treasure *treasure,
                                     struct cJSON *json_object);

int
treasure_value_in_cp(struct treasure *treasure);


#endif
