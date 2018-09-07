#include <assert.h>
#include <background/background.h>
#include <base/base.h>
#include <treasure/treasure.h>


void
magic_item_test(void);


static void
magic_item_initialize_test(void)
{
    struct magic_item magic_item;
    magic_item_initialize(&magic_item);

    assert(0 == magic_item.experience_points);
    assert(NULL == magic_item.true_description);
    assert(NULL == magic_item.true_details);
    assert(0 == magic_item.true_value_in_cp);
    assert(magic_item_type_unknown == magic_item.type);

    magic_item_finalize(&magic_item);
}


static void
magic_item_generate_for_any_with_fake_min_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_min();
    struct magic_item magic_item;
    magic_item_initialize(&magic_item);
    magic_item_generate(&magic_item, rnd, ANY_MAGIC_ITEM);

    assert(250 == magic_item.experience_points);
    assert(str_eq("animal control potion", magic_item.true_description));
    assert(NULL == magic_item.true_details);
    assert(gp_to_cp(400) == magic_item.true_value_in_cp);
    assert(magic_item_type_potion == magic_item.type);

    magic_item_finalize(&magic_item);
    rnd_free(rnd);
}


static void
magic_item_generate_for_any_with_fake_max_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_max();
    struct magic_item magic_item;
    magic_item_initialize(&magic_item);
    magic_item_generate(&magic_item, rnd, ANY_MAGIC_ITEM);

    assert(1500 == magic_item.experience_points);
    assert(str_eq("trident (military fork) +3", magic_item.true_description));
    assert(NULL == magic_item.true_details);
    assert(gp_to_cp(12500) == magic_item.true_value_in_cp);
    assert(magic_item_type_misc_weapon == magic_item.type);

    magic_item_finalize(&magic_item);
    rnd_free(rnd);
}


static void
magic_item_generate_for_any_with_fake_median_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_median();
    struct magic_item magic_item;
    magic_item_initialize(&magic_item);
    magic_item_generate(&magic_item, rnd, ANY_MAGIC_ITEM);

    assert(2000 == magic_item.experience_points);
    assert(str_eq("cloak of protection +2", magic_item.true_description));
    assert(NULL == magic_item.true_details);
    assert(gp_to_cp(20000) == magic_item.true_value_in_cp);
    assert(magic_item_type_misc == magic_item.type);

    magic_item_finalize(&magic_item);
    rnd_free(rnd);
}


static void
magic_item_generate_for_sword_with_fake_min_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_min();
    struct magic_item magic_item;
    magic_item_initialize(&magic_item);
    magic_item_generate(&magic_item, rnd, SWORD);

    assert(400 == magic_item.experience_points);
    assert(str_eq("longsword +1", magic_item.true_description));
    assert(NULL == magic_item.true_details);
    assert(gp_to_cp(2000) == magic_item.true_value_in_cp);
    assert(magic_item_type_sword == magic_item.type);

    magic_item_finalize(&magic_item);
    rnd_free(rnd);
}


static void
magic_item_generate_for_sword_with_fake_max_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_max();
    struct magic_item magic_item;
    magic_item_initialize(&magic_item);
    magic_item_generate(&magic_item, rnd, SWORD);

    assert(900 == magic_item.experience_points);
    assert(str_eq("two-handed sword, cursed berserking (unusual)", magic_item.true_description));
    assert(NULL != magic_item.true_details);

    assert(str_eq("intelligence 17 (speech and telepathy)", magic_item.true_details[0]));
    assert(str_eq("neutral good alignment", magic_item.true_details[1]));
    assert(str_eq("X-ray vision, 4\" range - 8 times/day, 1 turn per use", magic_item.true_details[2]));
    assert(str_eq("special purpose: slay non-human monsters", magic_item.true_details[3]));
    assert(str_eq("special purpose power: +2 on all saving throws, -1 on each die of damage sustained", magic_item.true_details[4]));
    assert(str_eq("speaks Xorn, Troll, Titan, Sylph, Sprite, Shedu, Satyr, Salamander, Pixie, Orcish, neutral good", magic_item.true_details[5]));
    assert(str_eq("personality strength 45 (ego 28)", magic_item.true_details[6]));
    assert(NULL == magic_item.true_details[7]);

    assert(0 == magic_item.true_value_in_cp);
    assert(magic_item_type_sword == magic_item.type);

    magic_item_finalize(&magic_item);
    rnd_free(rnd);
}


static void
magic_item_generate_for_unusual_sword_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_fixed(82);
    struct magic_item magic_item;
    magic_item_initialize(&magic_item);
    magic_item_generate(&magic_item, rnd, SWORD);

    assert(5000 == magic_item.experience_points);
    assert(str_eq("broadsword of life stealing (unusual)", magic_item.true_description));
    assert(NULL != magic_item.true_details);

    assert(str_eq("intelligence 12 (semi-empathy)", magic_item.true_details[0]));
    assert(str_eq("neutral good alignment", magic_item.true_details[1]));
    assert(str_eq("detect invisible objects in a 1\" radius", magic_item.true_details[2]));
    assert(str_eq("personality strength 17 (ego 5)", magic_item.true_details[3]));
    assert(NULL == magic_item.true_details[4]);

    assert(gp_to_cp(25000) == magic_item.true_value_in_cp);
    assert(magic_item_type_sword == magic_item.type);

    magic_item_finalize(&magic_item);
    rnd_free(rnd);
}


void
magic_item_test(void)
{
    magic_item_initialize_test();
    magic_item_generate_for_any_with_fake_min_test();
    magic_item_generate_for_any_with_fake_max_test();
    magic_item_generate_for_any_with_fake_median_test();

    magic_item_generate_for_sword_with_fake_min_test();
    magic_item_generate_for_sword_with_fake_max_test();
    magic_item_generate_for_unusual_sword_test();
}
