#include <assert.h>
#include <base/base.h>
#include <character/character.h>


void
abilities_test(void);


static void
abilities_alloc_simple_method_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct abilities *abilities = abilities_alloc_simple_method(rnd);

    assert(6 == abilities->strength);
    assert(15 == abilities->intelligence);
    assert(6 == abilities->wisdom);
    assert(15 == abilities->dexterity);
    assert(6 == abilities->constitution);
    assert(15 == abilities->charisma);

    abilities_free(abilities);
    rnd_free(rnd);
}


static void
abilities_alloc_method_3_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct abilities *abilities = abilities_alloc_method_3(rnd);

    assert(15 == abilities->strength);
    assert(15 == abilities->intelligence);
    assert(15 == abilities->wisdom);
    assert(15 == abilities->dexterity);
    assert(15 == abilities->constitution);
    assert(15 == abilities->charisma);

    abilities_free(abilities);
    rnd_free(rnd);
}


static void
abilities_alloc_method_general_NPC_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct abilities *abilities = abilities_alloc_general_NPC(rnd);

    assert(8 == abilities->strength);
    assert(13 == abilities->intelligence);
    assert(8 == abilities->wisdom);
    assert(13 == abilities->dexterity);
    assert(8 == abilities->constitution);
    assert(13 == abilities->charisma);

    abilities_free(abilities);
    rnd_free(rnd);
}


static void
abilities_alloc_method_special_NPC_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct abilities *abilities = abilities_alloc_special_NPC(rnd,
                                                              ability_flag_none);

    assert(6 == abilities->strength);
    assert(15 == abilities->intelligence);
    assert(6 == abilities->wisdom);
    assert(15 == abilities->dexterity);
    assert(6 == abilities->constitution);
    assert(15 == abilities->charisma);

    abilities_free(abilities);
    rnd_free(rnd);
}


static void
abilities_alloc_method_special_NPC_with_strength_flag_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct abilities *abilities = abilities_alloc_special_NPC(rnd,
                                                              ability_flag_strength);

    assert(9 == abilities->strength);
    assert(15 == abilities->intelligence);
    assert(6 == abilities->wisdom);
    assert(15 == abilities->dexterity);
    assert(6 == abilities->constitution);
    assert(15 == abilities->charisma);

    abilities_free(abilities);
    rnd_free(rnd);
}


static void
abilities_alloc_method_special_NPC_with_dexterity_and_constitution_flags_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct abilities *abilities = abilities_alloc_special_NPC(rnd,
                                                              ability_flag_dexterity |
                                                              ability_flag_constitution);

    assert(6 == abilities->strength);
    assert(15 == abilities->intelligence);
    assert(6 == abilities->wisdom);
    assert(17 == abilities->dexterity);
    assert(9 == abilities->constitution);
    assert(15 == abilities->charisma);

    abilities_free(abilities);
    rnd_free(rnd);
}


static void
abilities_compare_test(void)
{
    struct abilities first = {
            .strength=10,
            .intelligence=10,
            .wisdom=10,
            .dexterity=10,
            .constitution=10,
            .charisma=10,
    };
    struct abilities second = {
            .strength=10,
            .intelligence=10,
            .wisdom=10,
            .dexterity=10,
            .constitution=10,
            .charisma=10,
    };

    assert(0 == abilities_compare(&first, &second));
    assert(0 == abilities_compare(&second, &first));

    first.strength = 11;

    assert(abilities_compare(&first, &second) < 0);
    assert(abilities_compare(&second, &first) > 0);

    first.strength = 10;
    first.charisma = 11;

    assert(abilities_compare(&first, &second) < 0);
    assert(abilities_compare(&second, &first) > 0);

    first.charisma = 10;
    first.intelligence = 9;
    first.constitution = 11;

    assert(0 == abilities_compare(&first, &second));
    assert(0 == abilities_compare(&second, &first));
}


static void
abilities_total_test(void)
{
    struct abilities abilities = {
            .strength=10,
            .intelligence=10,
            .wisdom=10,
            .dexterity=10,
            .constitution=10,
            .charisma=10,
    };

    assert(60 == abilities_total(&abilities));

    abilities.strength = 18;
    abilities.intelligence = 17;
    abilities.wisdom = 16;
    abilities.dexterity = 15;
    abilities.constitution = 14;
    abilities.charisma = 13;

    assert(93 == abilities_total(&abilities));
}


void
abilities_test(void)
{
    abilities_alloc_simple_method_test();
    abilities_alloc_method_3_test();
    abilities_alloc_method_general_NPC_test();
    abilities_alloc_method_special_NPC_test();
    abilities_alloc_method_special_NPC_with_strength_flag_test();
    abilities_alloc_method_special_NPC_with_dexterity_and_constitution_flags_test();
    abilities_compare_test();
    abilities_total_test();
}
