#include <assert.h>
#include <background/background.h>
#include <base/base.h>
#include <treasure/treasure.h>

#include "astr.h"


void
treasure_type_test(void);


static void
treasure_type_by_letter_A_test(void)
{
    struct treasure_type *treasure_type = treasure_type_by_letter('A');

    assert(NULL != treasure_type);

    char *name = treasure_type_alloc_name(treasure_type);
    assert(astr_eq("A", name));
    free(name);

    char *description = treasure_type_alloc_description(treasure_type, false);
    char const *expected = "    A     |  1-6:25%  |  1-6:30%  |  1-6:35%  |  1-10:40% |  1-4:25%  |  4-40:60% |  3-30:50% | any 3: 30%\n";
    assert(astr_eq(expected, description));
    free(description);

    description = treasure_type_alloc_description(treasure_type, true);
    expected =
            "          |  1,000's  |  1,000's  |  1,000's  |  1,000's  |   100's   |           |           |    Maps   \n"
            "Treasure  |    of     |    of     |    of     |    of     |    of     |           |           |     or    \n"
            "  Type    |  Copper   |  Silver   | Electrum  |   Gold    | Platinum  |   Gems    |  Jewelry  |   Magic   \n"
            "----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------\n"
            "    A     |  1-6:25%  |  1-6:30%  |  1-6:35%  |  1-10:40% |  1-4:25%  |  4-40:60% |  3-30:50% | any 3: 30%\n";
    assert(astr_eq(expected, description));
    free(description);
}


static void
treasure_type_generate_for_A_with_fake_min_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_min();
    struct treasure treasure;
    treasure_initialize(&treasure);
    struct treasure_type *treasure_type = treasure_type_by_letter('A');

    treasure_type_generate(treasure_type, rnd, &treasure);

    assert(1000 == treasure.coins.cp);
    assert(1000 == treasure.coins.sp);
    assert(1000 == treasure.coins.ep);
    assert(1000 == treasure.coins.gp);
    assert(100 == treasure.coins.pp);
    assert(4 == treasure.gems_count);
    assert(3 == treasure.jewelry_count);
    assert(0 == treasure.magic_items_count);
    assert(3 == treasure.maps_count);

    treasure_finalize(&treasure);
    rnd_free(rnd);
}


static void
treasure_type_generate_for_A_with_fake_max_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_max();
    struct treasure treasure;
    treasure_initialize(&treasure);
    struct treasure_type *treasure_type = treasure_type_by_letter('A');

    treasure_type_generate(treasure_type, rnd, &treasure);

    assert(0 == treasure.coins.cp);
    assert(0 == treasure.coins.sp);
    assert(0 == treasure.coins.ep);
    assert(0 == treasure.coins.gp);
    assert(0 == treasure.coins.pp);
    assert(0 == treasure.gems_count);
    assert(0 == treasure.jewelry_count);
    assert(0 == treasure.magic_items_count);
    assert(0 == treasure.maps_count);

    treasure_finalize(&treasure);
    rnd_free(rnd);
}


static void
treasure_type_generate_for_A_with_fake_fixed_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_fixed(25);
    struct treasure treasure;
    treasure_initialize(&treasure);
    struct treasure_type *treasure_type = treasure_type_by_letter('A');

    treasure_type_generate(treasure_type, rnd, &treasure);

    assert(0 == treasure.coins.cp);
    assert(2000 == treasure.coins.sp);
    assert(2000 == treasure.coins.ep);
    assert(6000 == treasure.coins.gp);
    assert(0 == treasure.coins.pp);
    assert(24 == treasure.gems_count);
    assert(18 == treasure.jewelry_count);
    assert(3 == treasure.magic_items_count);
    assert(0 == treasure.maps_count);

    treasure_finalize(&treasure);
    rnd_free(rnd);
}


void
treasure_type_test(void)
{
    treasure_type_by_letter_A_test();
    treasure_type_generate_for_A_with_fake_min_test();
    treasure_type_generate_for_A_with_fake_max_test();
    treasure_type_generate_for_A_with_fake_fixed_test();
}
