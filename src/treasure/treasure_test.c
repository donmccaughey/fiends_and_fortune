#include <assert.h>
#include <background/background.h>
#include <base/base.h>
#include <treasure/treasure.h>


void
treasure_test(void);


static void
treasure_initialize_test(void)
{
    struct treasure treasure;
    treasure_initialize(&treasure);

    assert(NULL == treasure.type);
    assert(0 == treasure.coins.cp);
    assert(0 == treasure.coins.sp);
    assert(0 == treasure.coins.ep);
    assert(0 == treasure.coins.gp);
    assert(0 == treasure.coins.pp);
    assert(NULL == treasure.gems);
    assert(0 == treasure.gems_count);
    assert(NULL == treasure.jewelry);
    assert(0 == treasure.jewelry_count);
    assert(NULL == treasure.maps);
    assert(0 == treasure.maps_count);
    assert(NULL == treasure.magic_items);
    assert(0 == treasure.magic_items_count);

    treasure_finalize(&treasure);
}


static void
treasure_generate_magic_items_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_min();
    struct treasure treasure;
    treasure_initialize(&treasure);

    treasure_generate_magic_items(&treasure, rnd, 1, ANY_MAGIC_ITEM);

    assert(NULL == treasure.type);
    assert(0 == treasure.coins.cp);
    assert(0 == treasure.coins.sp);
    assert(0 == treasure.coins.ep);
    assert(0 == treasure.coins.gp);
    assert(0 == treasure.coins.pp);
    assert(NULL == treasure.gems);
    assert(0 == treasure.gems_count);
    assert(NULL == treasure.jewelry);
    assert(0 == treasure.jewelry_count);
    assert(NULL == treasure.maps);
    assert(0 == treasure.maps_count);

    assert(NULL != treasure.magic_items);
    assert(1 == treasure.magic_items_count);

    assert(magic_item_type_potion == treasure.magic_items[0].type);
    assert(str_eq("animal control potion", treasure.magic_items[0].true_description));

    rnd_free(rnd);
    rnd = rnd_alloc_fake_median();

    treasure_generate_magic_items(&treasure, rnd, 1, ANY_MAGIC_ITEM);

    assert(NULL != treasure.magic_items);
    assert(2 == treasure.magic_items_count);

    assert(magic_item_type_potion == treasure.magic_items[0].type);
    assert(str_eq("animal control potion", treasure.magic_items[0].true_description));
    assert(magic_item_type_misc == treasure.magic_items[1].type);
    assert(str_eq("cloak of protection +2", treasure.magic_items[1].true_description));

    treasure_finalize(&treasure);
    rnd_free(rnd);
}


void
treasure_test(void)
{
    treasure_initialize_test();
    treasure_generate_magic_items_test();
}
