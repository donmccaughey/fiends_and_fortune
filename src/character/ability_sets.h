#ifndef FNF_CHARACTER_ABILITY_SETS_H_INCLUDED
#define FNF_CHARACTER_ABILITY_SETS_H_INCLUDED


#include <character/abilities.h>


struct rnd;


struct ability_sets {
    struct abilities values[12];
};


struct ability_sets *
ability_sets_alloc_method_4(struct rnd *rnd);

void
ability_sets_free(struct ability_sets *sets);

void
ability_sets_sort(struct ability_sets *sets);


#endif
