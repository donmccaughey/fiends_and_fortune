#include <assert.h>
#include <base/base.h>
#include <character/character.h>


void
ability_scores_test(void);


static void
ability_scores_alloc_method_1_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct ability_scores *scores = ability_scores_alloc_method_1(rnd);

    assert(15 == scores->values[0]);
    assert(15 == scores->values[1]);
    assert(13 == scores->values[2]);
    assert(13 == scores->values[3]);
    assert( 9 == scores->values[4]);
    assert( 9 == scores->values[5]);

    ability_scores_free(scores);
    rnd_free(rnd);
}


static void
ability_scores_alloc_method_2_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct ability_scores *scores = ability_scores_alloc_method_2(rnd);

    assert(15 == scores->values[0]);
    assert(15 == scores->values[1]);
    assert(15 == scores->values[2]);
    assert(15 == scores->values[3]);
    assert(15 == scores->values[4]);
    assert(15 == scores->values[5]);

    ability_scores_free(scores);
    rnd_free(rnd);
}


void
ability_scores_test(void)
{
    ability_scores_alloc_method_1_test();
    ability_scores_alloc_method_2_test();
}
