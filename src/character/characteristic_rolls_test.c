#include <assert.h>
#include <base/base.h>
#include <character/character.h>


void
characteristic_rolls_test(void);


static void
characteristic_rolls_alloc_invalid_method_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct characteristic_rolls *rolls = characteristic_rolls_alloc(rnd,
                                                                    characteristic_generation_method_invalid,
                                                                    characteristic_flag_none);

    assert(characteristic_rolls_type_none == rolls->type);
    assert(0 == rolls->scores[0]);
    assert(0 == rolls->scores[1]);
    assert(0 == rolls->scores[2]);
    assert(0 == rolls->scores[3]);
    assert(0 == rolls->scores[4]);
    assert(0 == rolls->scores[5]);

    characteristic_rolls_free(rolls);
    rnd_free(rnd);
}


static void
characteristic_rolls_alloc_simple_method_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct characteristic_rolls *rolls = characteristic_rolls_alloc(rnd,
                                                                    characteristic_generation_method_simple,
                                                                    characteristic_flag_none);

    assert(characteristic_rolls_type_six_characteristics == rolls->type);
    assert(6 == rolls->characteristics.strength);
    assert(15 == rolls->characteristics.intelligence);
    assert(6 == rolls->characteristics.wisdom);
    assert(15 == rolls->characteristics.dexterity);
    assert(6 == rolls->characteristics.constitution);
    assert(15 == rolls->characteristics.charisma);

    characteristic_rolls_free(rolls);
    rnd_free(rnd);
}


static void
characteristic_rolls_alloc_method_1_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct characteristic_rolls *rolls = characteristic_rolls_alloc(rnd,
                                                                    characteristic_generation_method_1,
                                                                    characteristic_flag_none);

    assert(characteristic_rolls_type_six_scores == rolls->type);
    assert(15 == rolls->scores[0]);
    assert(15 == rolls->scores[1]);
    assert(13 == rolls->scores[2]);
    assert(13 == rolls->scores[3]);
    assert(9 == rolls->scores[4]);
    assert(9 == rolls->scores[5]);

    characteristic_rolls_free(rolls);
    rnd_free(rnd);
}


static void
characteristic_rolls_alloc_method_2_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct characteristic_rolls *rolls = characteristic_rolls_alloc(rnd,
                                                                    characteristic_generation_method_2,
                                                                    characteristic_flag_none);

    assert(characteristic_rolls_type_six_scores == rolls->type);
    assert(15 == rolls->scores[0]);
    assert(15 == rolls->scores[1]);
    assert(15 == rolls->scores[2]);
    assert(15 == rolls->scores[3]);
    assert(15 == rolls->scores[4]);
    assert(15 == rolls->scores[5]);

    characteristic_rolls_free(rolls);
    rnd_free(rnd);
}


static void
characteristic_rolls_alloc_method_3_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct characteristic_rolls *rolls = characteristic_rolls_alloc(rnd,
                                                                    characteristic_generation_method_3,
                                                                    characteristic_flag_none);

    assert(characteristic_rolls_type_six_characteristics == rolls->type);
    assert(15 == rolls->characteristics.strength);
    assert(15 == rolls->characteristics.intelligence);
    assert(15 == rolls->characteristics.wisdom);
    assert(15 == rolls->characteristics.dexterity);
    assert(15 == rolls->characteristics.constitution);
    assert(15 == rolls->characteristics.charisma);

    characteristic_rolls_free(rolls);
    rnd_free(rnd);
}


static void
characteristic_rolls_alloc_method_4_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct characteristic_rolls *rolls = characteristic_rolls_alloc(rnd,
                                                                    characteristic_generation_method_4,
                                                                    characteristic_flag_none);

    assert(characteristic_rolls_type_twelve_sets_of_six_characteristics == rolls->type);
    assert(6 == rolls->characteristics_sets[0].strength);
    assert(15 == rolls->characteristics_sets[0].intelligence);
    assert(6 == rolls->characteristics_sets[0].wisdom);
    assert(15 == rolls->characteristics_sets[0].dexterity);
    assert(6 == rolls->characteristics_sets[0].constitution);
    assert(15 == rolls->characteristics_sets[0].charisma);

    assert(6 == rolls->characteristics_sets[1].strength);
    assert(15 == rolls->characteristics_sets[1].intelligence);
    assert(6 == rolls->characteristics_sets[1].wisdom);
    assert(15 == rolls->characteristics_sets[1].dexterity);
    assert(6 == rolls->characteristics_sets[1].constitution);
    assert(15 == rolls->characteristics_sets[1].charisma);

    assert(6 == rolls->characteristics_sets[2].strength);
    assert(15 == rolls->characteristics_sets[2].intelligence);
    assert(6 == rolls->characteristics_sets[2].wisdom);
    assert(15 == rolls->characteristics_sets[2].dexterity);
    assert(6 == rolls->characteristics_sets[2].constitution);
    assert(15 == rolls->characteristics_sets[2].charisma);

    assert(6 == rolls->characteristics_sets[3].strength);
    assert(15 == rolls->characteristics_sets[3].intelligence);
    assert(6 == rolls->characteristics_sets[3].wisdom);
    assert(15 == rolls->characteristics_sets[3].dexterity);
    assert(6 == rolls->characteristics_sets[3].constitution);
    assert(15 == rolls->characteristics_sets[3].charisma);

    assert(6 == rolls->characteristics_sets[4].strength);
    assert(15 == rolls->characteristics_sets[4].intelligence);
    assert(6 == rolls->characteristics_sets[4].wisdom);
    assert(15 == rolls->characteristics_sets[4].dexterity);
    assert(6 == rolls->characteristics_sets[4].constitution);
    assert(15 == rolls->characteristics_sets[4].charisma);

    assert(6 == rolls->characteristics_sets[5].strength);
    assert(15 == rolls->characteristics_sets[5].intelligence);
    assert(6 == rolls->characteristics_sets[5].wisdom);
    assert(15 == rolls->characteristics_sets[5].dexterity);
    assert(6 == rolls->characteristics_sets[5].constitution);
    assert(15 == rolls->characteristics_sets[5].charisma);

    assert(6 == rolls->characteristics_sets[6].strength);
    assert(15 == rolls->characteristics_sets[6].intelligence);
    assert(6 == rolls->characteristics_sets[6].wisdom);
    assert(15 == rolls->characteristics_sets[6].dexterity);
    assert(6 == rolls->characteristics_sets[6].constitution);
    assert(15 == rolls->characteristics_sets[6].charisma);

    assert(6 == rolls->characteristics_sets[7].strength);
    assert(15 == rolls->characteristics_sets[7].intelligence);
    assert(6 == rolls->characteristics_sets[7].wisdom);
    assert(15 == rolls->characteristics_sets[7].dexterity);
    assert(6 == rolls->characteristics_sets[7].constitution);
    assert(15 == rolls->characteristics_sets[7].charisma);

    assert(6 == rolls->characteristics_sets[8].strength);
    assert(15 == rolls->characteristics_sets[8].intelligence);
    assert(6 == rolls->characteristics_sets[8].wisdom);
    assert(15 == rolls->characteristics_sets[8].dexterity);
    assert(6 == rolls->characteristics_sets[8].constitution);
    assert(15 == rolls->characteristics_sets[8].charisma);

    assert(6 == rolls->characteristics_sets[9].strength);
    assert(15 == rolls->characteristics_sets[9].intelligence);
    assert(6 == rolls->characteristics_sets[9].wisdom);
    assert(15 == rolls->characteristics_sets[9].dexterity);
    assert(6 == rolls->characteristics_sets[9].constitution);
    assert(15 == rolls->characteristics_sets[9].charisma);

    assert(6 == rolls->characteristics_sets[10].strength);
    assert(15 == rolls->characteristics_sets[10].intelligence);
    assert(6 == rolls->characteristics_sets[10].wisdom);
    assert(15 == rolls->characteristics_sets[10].dexterity);
    assert(6 == rolls->characteristics_sets[10].constitution);
    assert(15 == rolls->characteristics_sets[10].charisma);

    assert(6 == rolls->characteristics_sets[11].strength);
    assert(15 == rolls->characteristics_sets[11].intelligence);
    assert(6 == rolls->characteristics_sets[11].wisdom);
    assert(15 == rolls->characteristics_sets[11].dexterity);
    assert(6 == rolls->characteristics_sets[11].constitution);
    assert(15 == rolls->characteristics_sets[11].charisma);

    characteristic_rolls_free(rolls);
    rnd_free(rnd);
}


static void
characteristic_rolls_alloc_general_NPC_method_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct characteristic_rolls *rolls = characteristic_rolls_alloc(rnd,
                                                                    characteristic_generation_method_general_NPC,
                                                                    characteristic_flag_none);

    assert(characteristic_rolls_type_six_characteristics == rolls->type);
    assert(8 == rolls->characteristics.strength);
    assert(13 == rolls->characteristics.intelligence);
    assert(8 == rolls->characteristics.wisdom);
    assert(13 == rolls->characteristics.dexterity);
    assert(8 == rolls->characteristics.constitution);
    assert(13 == rolls->characteristics.charisma);

    characteristic_rolls_free(rolls);
    rnd_free(rnd);
}


static void
characteristic_rolls_alloc_special_NPC_method_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct characteristic_rolls *rolls = characteristic_rolls_alloc(rnd,
                                                                    characteristic_generation_method_special_NPC,
                                                                    characteristic_flag_none);

    assert(characteristic_rolls_type_six_characteristics == rolls->type);
    assert(6 == rolls->characteristics.strength);
    assert(15 == rolls->characteristics.intelligence);
    assert(6 == rolls->characteristics.wisdom);
    assert(15 == rolls->characteristics.dexterity);
    assert(6 == rolls->characteristics.constitution);
    assert(15 == rolls->characteristics.charisma);

    characteristic_rolls_free(rolls);
    rnd_free(rnd);
}


static void
characteristic_rolls_alloc_special_NPC_method_strength_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct characteristic_rolls *rolls = characteristic_rolls_alloc(rnd,
                                                                    characteristic_generation_method_special_NPC,
                                                                    characteristic_flag_strength);

    assert(characteristic_rolls_type_six_characteristics == rolls->type);
    assert(9 == rolls->characteristics.strength);
    assert(15 == rolls->characteristics.intelligence);
    assert(6 == rolls->characteristics.wisdom);
    assert(15 == rolls->characteristics.dexterity);
    assert(6 == rolls->characteristics.constitution);
    assert(15 == rolls->characteristics.charisma);

    characteristic_rolls_free(rolls);
    rnd_free(rnd);
}


static void
characteristic_rolls_alloc_special_NPC_method_dexterity_constitution_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct characteristic_rolls *rolls = characteristic_rolls_alloc(rnd,
                                                                    characteristic_generation_method_special_NPC,
                                                                    characteristic_flag_dexterity | characteristic_flag_constitution);

    assert(characteristic_rolls_type_six_characteristics == rolls->type);
    assert(6 == rolls->characteristics.strength);
    assert(15 == rolls->characteristics.intelligence);
    assert(6 == rolls->characteristics.wisdom);
    assert(17 == rolls->characteristics.dexterity);
    assert(9 == rolls->characteristics.constitution);
    assert(15 == rolls->characteristics.charisma);

    characteristic_rolls_free(rolls);
    rnd_free(rnd);
}


void
characteristic_rolls_test(void)
{
    characteristic_rolls_alloc_invalid_method_test();
    characteristic_rolls_alloc_simple_method_test();
    characteristic_rolls_alloc_method_1_test();
    characteristic_rolls_alloc_method_2_test();
    characteristic_rolls_alloc_method_3_test();
    characteristic_rolls_alloc_method_4_test();
    characteristic_rolls_alloc_general_NPC_method_test();
    characteristic_rolls_alloc_special_NPC_method_test();
    characteristic_rolls_alloc_special_NPC_method_strength_test();
    characteristic_rolls_alloc_special_NPC_method_dexterity_constitution_test();
}
