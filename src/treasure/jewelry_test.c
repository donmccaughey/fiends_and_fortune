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


void
jewelry_test(void)
{
    jewelry_initialize_test();
    jewelry_generate_with_fake_min_test();
    jewelry_generate_with_fake_max_test();
    jewelry_generate_with_fake_median_test();
}
