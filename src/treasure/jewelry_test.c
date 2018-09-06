#include <assert.h>
#include <background/background.h>
#include <base/base.h>
#include <treasure/treasure.h>


void
jewelry_test(void);


static void
jewelry_initialize_test(void)
{
    struct jewelry jewelry;
    jewelry_initialize(&jewelry);

    assert(false == jewelry.has_gems);
    assert(jewelry_form_anklet == jewelry.form);
    assert(jewelry_material_fake == jewelry.material);
    assert(0 == jewelry.workmanship_bonus);
    assert(0 == jewelry.exceptional_stone_bonus);
    assert(0 == jewelry.value_in_cp);
    assert(NULL == jewelry.true_description);

    jewelry_finalize(&jewelry);
}


static void
jewelry_generate_with_fake_min_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_min();
    struct jewelry jewelry;
    jewelry_initialize(&jewelry);
    jewelry_generate(&jewelry, rnd);

    assert(false == jewelry.has_gems);
    assert(jewelry_form_anklet == jewelry.form);
    assert(jewelry_material_ivory == jewelry.material);
    assert(12 == jewelry.workmanship_bonus);
    assert(0 == jewelry.exceptional_stone_bonus);
    assert(gp_to_cp(2000) == jewelry.value_in_cp);
    assert(str_eq("ivory anklet (workmanship +12: 2000 gp)", jewelry.true_description));

    jewelry_finalize(&jewelry);
    rnd_free(rnd);
}


static void
jewelry_generate_with_fake_max_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_max();
    struct jewelry jewelry;
    jewelry_initialize(&jewelry);
    jewelry_generate(&jewelry, rnd);

    assert(true == jewelry.has_gems);
    assert(jewelry_form_tiara == jewelry.form);
    assert(jewelry_material_platinum_with_gems == jewelry.material);
    assert(0 == jewelry.workmanship_bonus);
    assert(0 == jewelry.exceptional_stone_bonus);
    assert(gp_to_cp(12000) == jewelry.value_in_cp);
    assert(str_eq("platinum tiara with gems (12000 gp)", jewelry.true_description));

    jewelry_finalize(&jewelry);
    rnd_free(rnd);
}


static void
jewelry_generate_with_fake_median_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_median();
    struct jewelry jewelry;
    jewelry_initialize(&jewelry);
    jewelry_generate(&jewelry, rnd);

    assert(true == jewelry.has_gems);
    assert(jewelry_form_goblet == jewelry.form);
    assert(jewelry_material_silver_with_gems == jewelry.material);
    assert(0 == jewelry.workmanship_bonus);
    assert(0 == jewelry.exceptional_stone_bonus);
    assert(gp_to_cp(4000) == jewelry.value_in_cp);
    assert(str_eq("silver goblet with gems (4000 gp)", jewelry.true_description));

    jewelry_finalize(&jewelry);
    rnd_free(rnd);
}


static void
jewelry_generate_for_exceptional_stone_bonus_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_fixed(64);
    struct jewelry jewelry;
    jewelry_initialize(&jewelry);
    jewelry_generate(&jewelry, rnd);

    assert(true == jewelry.has_gems);
    assert(jewelry_form_medallion == jewelry.form);
    assert(jewelry_material_silver_with_gems == jewelry.material);
    assert(0 == jewelry.workmanship_bonus);
    assert(1 == jewelry.exceptional_stone_bonus);
    assert(gp_to_cp(10000) == jewelry.value_in_cp);
    assert(str_eq("silver medallion with gems (exceptional stone +1: 10000 gp)", jewelry.true_description));

    jewelry_finalize(&jewelry);
    rnd_free(rnd);
}


static void
jewelry_generate_for_exceptional_stone_bonus_multiple_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_fixed(72);
    struct jewelry jewelry;
    jewelry_initialize(&jewelry);
    jewelry_generate(&jewelry, rnd);

    assert(true == jewelry.has_gems);
    assert(jewelry_form_necklace == jewelry.form);
    assert(jewelry_material_gold_with_gems == jewelry.material);
    assert(0 == jewelry.workmanship_bonus);
    assert(128 == jewelry.exceptional_stone_bonus);
    assert(gp_to_cp(642000) == jewelry.value_in_cp);
    assert(str_eq("gold necklace with gems (exceptional stone +128: 642000 gp)", jewelry.true_description));

    jewelry_finalize(&jewelry);
    rnd_free(rnd);
}


void
jewelry_test(void)
{
    jewelry_initialize_test();
    jewelry_generate_with_fake_min_test();
    jewelry_generate_with_fake_max_test();
    jewelry_generate_with_fake_median_test();
    jewelry_generate_for_exceptional_stone_bonus_test();
    jewelry_generate_for_exceptional_stone_bonus_multiple_test();
}
