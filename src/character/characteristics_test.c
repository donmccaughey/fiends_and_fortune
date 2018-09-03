#include <assert.h>
#include <base/base.h>
#include <character/character.h>


void
characteristics_test(void);


static void
characteristics_alloc_simple_method_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct characteristics *characteristics = characteristics_alloc_simple_method(rnd);

    assert(6 == characteristics->strength);
    assert(15 == characteristics->intelligence);
    assert(6 == characteristics->wisdom);
    assert(15 == characteristics->dexterity);
    assert(6 == characteristics->constitution);
    assert(15 == characteristics->charisma);

    characteristics_struct_free(characteristics);
    rnd_free(rnd);
}


static void
characteristics_alloc_method_3_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct characteristics *characteristics = characteristics_alloc_method_3(rnd);

    assert(15 == characteristics->strength);
    assert(15 == characteristics->intelligence);
    assert(15 == characteristics->wisdom);
    assert(15 == characteristics->dexterity);
    assert(15 == characteristics->constitution);
    assert(15 == characteristics->charisma);

    characteristics_struct_free(characteristics);
    rnd_free(rnd);
}


static void
characteristics_alloc_method_general_NPC_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct characteristics *characteristics = characteristics_alloc_general_NPC(rnd);

    assert(8 == characteristics->strength);
    assert(13 == characteristics->intelligence);
    assert(8 == characteristics->wisdom);
    assert(13 == characteristics->dexterity);
    assert(8 == characteristics->constitution);
    assert(13 == characteristics->charisma);

    characteristics_struct_free(characteristics);
    rnd_free(rnd);
}


static void
characteristics_alloc_method_special_NPC_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct characteristics *characteristics = characteristics_alloc_special_NPC(rnd,
                                                                                characteristic_flag_none);

    assert(6 == characteristics->strength);
    assert(15 == characteristics->intelligence);
    assert(6 == characteristics->wisdom);
    assert(15 == characteristics->dexterity);
    assert(6 == characteristics->constitution);
    assert(15 == characteristics->charisma);

    characteristics_struct_free(characteristics);
    rnd_free(rnd);
}


static void
characteristics_alloc_method_special_NPC_with_strength_flag_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct characteristics *characteristics = characteristics_alloc_special_NPC(rnd,
                                                                                characteristic_flag_strength);

    assert(9 == characteristics->strength);
    assert(15 == characteristics->intelligence);
    assert(6 == characteristics->wisdom);
    assert(15 == characteristics->dexterity);
    assert(6 == characteristics->constitution);
    assert(15 == characteristics->charisma);

    characteristics_struct_free(characteristics);
    rnd_free(rnd);
}


static void
characteristics_alloc_method_special_NPC_with_dexterity_and_constitution_flags_test(
        void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct characteristics *characteristics = characteristics_alloc_special_NPC(rnd,
                                                                                characteristic_flag_dexterity | characteristic_flag_constitution);

    assert(6 == characteristics->strength);
    assert(15 == characteristics->intelligence);
    assert(6 == characteristics->wisdom);
    assert(17 == characteristics->dexterity);
    assert(9 == characteristics->constitution);
    assert(15 == characteristics->charisma);

    characteristics_struct_free(characteristics);
    rnd_free(rnd);
}


static void
characteristics_compare_test(void)
{
    struct characteristics first = {
            .strength=10,
            .intelligence=10,
            .wisdom=10,
            .dexterity=10,
            .constitution=10,
            .charisma=10,
    };
    struct characteristics second = {
            .strength=10,
            .intelligence=10,
            .wisdom=10,
            .dexterity=10,
            .constitution=10,
            .charisma=10,
    };

    assert(0 == characteristics_compare(&first, &second));
    assert(0 == characteristics_compare(&second, &first));

    first.strength = 11;

    assert(characteristics_compare(&first, &second) < 0);
    assert(characteristics_compare(&second, &first) > 0);

    first.strength = 10;
    first.charisma = 11;

    assert(characteristics_compare(&first, &second) < 0);
    assert(characteristics_compare(&second, &first) > 0);

    first.charisma = 10;
    first.intelligence = 9;
    first.constitution = 11;

    assert(0 == characteristics_compare(&first, &second));
    assert(0 == characteristics_compare(&second, &first));
}


static void
characteristics_total_test(void)
{
    struct characteristics characteristics = {
            .strength=10,
            .intelligence=10,
            .wisdom=10,
            .dexterity=10,
            .constitution=10,
            .charisma=10,
    };

    assert(60 == characteristics_total(&characteristics));

    characteristics.strength = 18;
    characteristics.intelligence = 17;
    characteristics.wisdom = 16;
    characteristics.dexterity = 15;
    characteristics.constitution = 14;
    characteristics.charisma = 13;

    assert(93 == characteristics_total(&characteristics));
}


void
characteristics_test(void)
{
    characteristics_alloc_simple_method_test();
    characteristics_alloc_method_3_test();
    characteristics_alloc_method_general_NPC_test();
    characteristics_alloc_method_special_NPC_test();
    characteristics_alloc_method_special_NPC_with_strength_flag_test();
    characteristics_alloc_method_special_NPC_with_dexterity_and_constitution_flags_test();
    characteristics_compare_test();
    characteristics_total_test();
}
