#include "ability_sets.h"

#include <base/base.h>
#include <mechanics/mechanics.h>


struct ability_sets *
ability_sets_alloc_method_4(struct rnd *rnd)
{
    struct ability_sets *sets = xcalloc(1, sizeof(struct ability_sets));
    size_t count = sizeof sets->values / sizeof sets->values[0];
    for (size_t i = 0; i < count; ++i) {
        sets->values[i].strength = roll("3d6", rnd);
        sets->values[i].intelligence = roll("3d6", rnd);
        sets->values[i].wisdom = roll("3d6", rnd);
        sets->values[i].dexterity = roll("3d6", rnd);
        sets->values[i].constitution = roll("3d6", rnd);
        sets->values[i].charisma = roll("3d6", rnd);
    }
    ability_sets_sort(sets);
    return sets;
}


void
ability_sets_free(struct ability_sets *sets)
{
    free(sets);
}


void
ability_sets_sort(struct ability_sets *sets)
{
    size_t size = sizeof sets->values[0];
    size_t count = sizeof sets->values / size;
    compare_fn *compare = (compare_fn *)abilities_compare;
    insertion_sort(&sets->values, count, size, compare);
}
