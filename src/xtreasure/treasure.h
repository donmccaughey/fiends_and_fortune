#ifndef FNF_TREASURE_TREASURE_H_INCLUDED
#define FNF_TREASURE_TREASURE_H_INCLUDED


#include "coins.h"
#include "MagicItem.h"


struct gem;
struct jewelry;
struct MagicItem;
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
    struct MagicItem *magic_items;
    int magic_items_count;
};


char *
treasure_alloc_description(struct treasure *treasure);

void
treasure_finalize(struct treasure *treasure);

void
treasure_generate_magic_items(struct treasure *treasure,
                              struct rnd *rnd,
                              int count,
                              PossibleMagicItems possible_magic_items);

void
treasure_generate_maps(struct treasure *treasure, struct rnd *rnd, int count);

void
treasure_initialize(struct treasure *treasure);

int
treasure_value_in_cp(struct treasure *treasure);


#endif
