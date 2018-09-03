#ifndef FNF_CHARACTER_CHARACTERISTICS_SETS_H_INCLUDED
#define FNF_CHARACTER_CHARACTERISTICS_SETS_H_INCLUDED


#include <character/characteristics.h>


struct rnd;


struct characteristics_sets {
    struct characteristics values[12];
};


struct characteristics_sets *
characteristics_sets_alloc_method_4(struct rnd *rnd);

void
characteristics_sets_free(struct characteristics_sets *sets);

void
characteristics_sets_sort(struct characteristics_sets *sets);


#endif
