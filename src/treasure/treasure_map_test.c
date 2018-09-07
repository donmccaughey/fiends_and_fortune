#include <assert.h>
#include <background/background.h>
#include <base/base.h>
#include <treasure/treasure.h>


void
treasure_map_test(void);


static void
treasure_map_initialize_test(void)
{
    struct treasure_map map;
    treasure_map_initialize(&map);

    assert( ! map.is_false);
    assert(0 == map.treasure.coins.cp);
    assert(0 == map.treasure.coins.sp);
    assert(0 == map.treasure.coins.ep);
    assert(0 == map.treasure.coins.gp);
    assert(0 == map.treasure.coins.pp);
    assert(0 == map.treasure.gems_count);
    assert(0 == map.treasure.jewelry_count);
    assert(0 == map.treasure.maps_count);
    assert(0 == map.treasure.magic_items_count);
    assert(NULL == map.true_description);

    treasure_map_finalize(&map);
}


static void
treasure_map_generate_with_fake_min_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_min();
    struct treasure_map map;
    treasure_map_initialize(&map);
    treasure_map_generate(&map, rnd);

    assert(map.is_false);
    assert(0 == map.treasure.coins.cp);
    assert(0 == map.treasure.coins.sp);
    assert(0 == map.treasure.coins.ep);
    assert(0 == map.treasure.coins.gp);
    assert(0 == map.treasure.coins.pp);
    assert(0 == map.treasure.gems_count);
    assert(0 == map.treasure.jewelry_count);
    assert(0 == map.treasure.maps_count);
    assert(0 == map.treasure.magic_items_count);
    assert(str_eq("false map to treasure of (no treasure) in nearby labyrinth to the north", map.true_description));

    treasure_map_finalize(&map);
    rnd_free(rnd);
}


static void
treasure_map_generate_with_fake_max_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_max();
    struct treasure_map map;
    treasure_map_initialize(&map);
    treasure_map_generate(&map, rnd);

    assert( ! map.is_false);
    assert(0 == map.treasure.coins.cp);
    assert(0 == map.treasure.coins.sp);
    assert(0 == map.treasure.coins.ep);
    assert(0 == map.treasure.coins.gp);
    assert(0 == map.treasure.coins.pp);
    assert(0 == map.treasure.gems_count);
    assert(0 == map.treasure.jewelry_count);
    assert(0 == map.treasure.maps_count);
    assert(17 == map.treasure.magic_items_count);
    assert(str_eq("map to combined hoard of 17 magic items 500 miles to the northwest, secreted in a town", map.true_description));

    treasure_map_finalize(&map);
    rnd_free(rnd);
}


static void
treasure_map_generate_with_fake_median_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_median();
    struct treasure_map map;
    treasure_map_initialize(&map);
    treasure_map_generate(&map, rnd);

    assert( ! map.is_false);
    assert(0 == map.treasure.coins.cp);
    assert(0 == map.treasure.coins.sp);
    assert(0 == map.treasure.coins.ep);
    assert(0 == map.treasure.coins.gp);
    assert(1500 == map.treasure.coins.pp);
    assert(0 == map.treasure.gems_count);
    assert(0 == map.treasure.jewelry_count);
    assert(0 == map.treasure.maps_count);
    assert(0 == map.treasure.magic_items_count);
    assert(str_eq("map to monetary treasure of 1500 platinum 7 miles to the south, guarded in a lair", map.true_description));

    treasure_map_finalize(&map);
    rnd_free(rnd);
}


void
treasure_map_test(void)
{
    treasure_map_initialize_test();
    treasure_map_generate_with_fake_min_test();
    treasure_map_generate_with_fake_max_test();
    treasure_map_generate_with_fake_median_test();
}

