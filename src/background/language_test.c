#include <assert.h>
#include <base/base.h>
#include <background/background.h>


void
language_test(void);


static void
language_determine_no_exclude_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_fixed(8);

    char const *language = language_determine(rnd, NULL, 0);
    assert(str_eq("Copper Dragon", language));

    rnd_free(rnd);
}


static void
language_determine_with_exclude_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_fixed(8);
    char const *exclude[] = { "Brownie", "Centaur" };
    size_t exclude_count = 2;

    char const *language = language_determine(rnd, exclude, exclude_count);
    assert(str_eq("Green Dragon", language));

    rnd_free(rnd);
}


void
language_test(void)
{
    language_determine_no_exclude_test();
    language_determine_with_exclude_test();
}
