#include "ability_scores.h"

#include <base/base.h>
#include <mechanics/mechanics.h>


static int
compare_ability_scores(void const *first, void const *second);


struct ability_scores *
ability_scores_alloc_method_1(struct rnd *rnd)
{
    struct ability_scores *scores = calloc_or_die(1, sizeof(struct ability_scores));
    size_t count = sizeof scores->values / sizeof scores->values[0];
    struct dice dice = dice_make(4, 6);
    for (size_t i = 0; i < count; ++i) {
        scores->values[i] = dice_roll_and_drop_lowest(dice, rnd);
    }
    qsort(scores->values, count, sizeof scores->values[0],
          compare_ability_scores);
    return scores;
}


struct ability_scores *
ability_scores_alloc_method_2(struct rnd *rnd)
{
    int characteristics[12];
    size_t count = sizeof characteristics / sizeof characteristics[0];
    for (size_t i = 0; i < count; ++i) {
        characteristics[i] = roll("3d6", rnd);
    }
    qsort(characteristics, count, sizeof characteristics[0],
          compare_ability_scores);

    struct ability_scores *scores = calloc_or_die(1, sizeof(struct ability_scores));
    memcpy(scores->values, characteristics, sizeof scores->values);
    return scores;
}


void
ability_scores_free(struct ability_scores *scores)
{
    free_or_die(scores);
}


static int
compare_ability_scores(void const *first, void const *second)
{
    return *((int const *) second) - *((int const *) first);
}
