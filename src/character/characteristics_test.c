#include <assert.h>
#include <base/base.h>
#include <character/character.h>


void
characteristics_test(void);


static void
characteristics_alloc_invalid_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    int *characteristics = characteristics_alloc(rnd,
                                                 characteristic_generation_method_invalid,
                                                 characteristic_flag_none);

    assert(6 == characteristics[0]);
    assert(15 == characteristics[1]);
    assert(6 == characteristics[2]);
    assert(15 == characteristics[3]);
    assert(6 == characteristics[4]);
    assert(15 == characteristics[5]);

    characteristics_free(characteristics);
    rnd_free(rnd);
}


static void
characteristics_alloc_simple_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    int *characteristics = characteristics_alloc(rnd,
                                                 characteristic_generation_method_simple,
                                                 characteristic_flag_none);

    assert(6 == characteristics[0]);
    assert(15 == characteristics[1]);
    assert(6 == characteristics[2]);
    assert(15 == characteristics[3]);
    assert(6 == characteristics[4]);
    assert(15 == characteristics[5]);

    characteristics_free(characteristics);
    rnd_free(rnd);
}


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
characteristics_alloc_method1_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    int *characteristics = characteristics_alloc(rnd,
                                                 characteristic_generation_method_1,
                                                 characteristic_flag_none);

    assert(15 == characteristics[0]);
    assert(15 == characteristics[1]);
    assert(13 == characteristics[2]);
    assert(13 == characteristics[3]);
    assert(9 == characteristics[4]);
    assert(9 == characteristics[5]);

    characteristics_free(characteristics);
    rnd_free(rnd);
}


static void
characteristics_alloc_method2_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    int *characteristics = characteristics_alloc(rnd,
                                                 characteristic_generation_method_2,
                                                 characteristic_flag_none);

    assert(15 == characteristics[0]);
    assert(15 == characteristics[1]);
    assert(15 == characteristics[2]);
    assert(15 == characteristics[3]);
    assert(15 == characteristics[4]);
    assert(15 == characteristics[5]);

    characteristics_free(characteristics);
    rnd_free(rnd);
}


static void
characteristics_alloc_method3_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    int *characteristics = characteristics_alloc(rnd,
                                                 characteristic_generation_method_3,
                                                 characteristic_flag_none);

    assert(15 == characteristics[0]);
    assert(15 == characteristics[1]);
    assert(15 == characteristics[2]);
    assert(15 == characteristics[3]);
    assert(15 == characteristics[4]);
    assert(15 == characteristics[5]);

    characteristics_free(characteristics);
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
characteristics_alloc_method4_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    int *characteristics = characteristics_alloc(rnd,
                                                 characteristic_generation_method_4,
                                                 characteristic_flag_none);

    assert(6 == characteristics[0]);
    assert(15 == characteristics[1]);
    assert(6 == characteristics[2]);
    assert(15 == characteristics[3]);
    assert(6 == characteristics[4]);
    assert(15 == characteristics[5]);

    assert(6 == characteristics[6]);
    assert(15 == characteristics[7]);
    assert(6 == characteristics[8]);
    assert(15 == characteristics[9]);
    assert(6 == characteristics[10]);
    assert(15 == characteristics[11]);

    assert(6 == characteristics[12]);
    assert(15 == characteristics[13]);
    assert(6 == characteristics[14]);
    assert(15 == characteristics[15]);
    assert(6 == characteristics[16]);
    assert(15 == characteristics[17]);

    assert(6 == characteristics[18]);
    assert(15 == characteristics[19]);
    assert(6 == characteristics[20]);
    assert(15 == characteristics[21]);
    assert(6 == characteristics[22]);
    assert(15 == characteristics[23]);

    assert(6 == characteristics[24]);
    assert(15 == characteristics[25]);
    assert(6 == characteristics[26]);
    assert(15 == characteristics[27]);
    assert(6 == characteristics[28]);
    assert(15 == characteristics[29]);

    assert(6 == characteristics[30]);
    assert(15 == characteristics[31]);
    assert(6 == characteristics[32]);
    assert(15 == characteristics[33]);
    assert(6 == characteristics[34]);
    assert(15 == characteristics[35]);

    assert(6 == characteristics[36]);
    assert(15 == characteristics[37]);
    assert(6 == characteristics[38]);
    assert(15 == characteristics[39]);
    assert(6 == characteristics[40]);
    assert(15 == characteristics[41]);

    assert(6 == characteristics[42]);
    assert(15 == characteristics[43]);
    assert(6 == characteristics[44]);
    assert(15 == characteristics[45]);
    assert(6 == characteristics[46]);
    assert(15 == characteristics[47]);

    assert(6 == characteristics[48]);
    assert(15 == characteristics[49]);
    assert(6 == characteristics[50]);
    assert(15 == characteristics[51]);
    assert(6 == characteristics[52]);
    assert(15 == characteristics[53]);

    assert(6 == characteristics[54]);
    assert(15 == characteristics[55]);
    assert(6 == characteristics[56]);
    assert(15 == characteristics[57]);
    assert(6 == characteristics[58]);
    assert(15 == characteristics[59]);

    assert(6 == characteristics[60]);
    assert(15 == characteristics[61]);
    assert(6 == characteristics[62]);
    assert(15 == characteristics[63]);
    assert(6 == characteristics[64]);
    assert(15 == characteristics[65]);

    assert(6 == characteristics[66]);
    assert(15 == characteristics[67]);
    assert(6 == characteristics[68]);
    assert(15 == characteristics[69]);
    assert(6 == characteristics[70]);
    assert(15 == characteristics[71]);

    characteristics_free(characteristics);
    rnd_free(rnd);
}


static void
characteristics_alloc_method_general_NPC_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    int *characteristics = characteristics_alloc(rnd,
                                                 characteristic_generation_method_general_NPC,
                                                 characteristic_flag_none);

    assert(8 == characteristics[0]);
    assert(13 == characteristics[1]);
    assert(8 == characteristics[2]);
    assert(13 == characteristics[3]);
    assert(8 == characteristics[4]);
    assert(13 == characteristics[5]);

    characteristics_free(characteristics);
    rnd_free(rnd);
}


static void
characteristics_struct_alloc_method_general_NPC_test(void)
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
    int *characteristics = characteristics_alloc(rnd,
                                                 characteristic_generation_method_special_NPC,
                                                 characteristic_flag_none);

    assert(6 == characteristics[0]);
    assert(15 == characteristics[1]);
    assert(6 == characteristics[2]);
    assert(15 == characteristics[3]);
    assert(6 == characteristics[4]);
    assert(15 == characteristics[5]);

    characteristics_free(characteristics);
    rnd_free(rnd);
}


static void
characteristics_struct_alloc_method_special_NPC_test(void)
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
characteristics_alloc_method_special_NPC_STRENGTH_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    int *characteristics = characteristics_alloc(rnd,
                                                 characteristic_generation_method_special_NPC,
                                                 characteristic_flag_strength);

    assert(9 == characteristics[0]);
    assert(15 == characteristics[1]);
    assert(6 == characteristics[2]);
    assert(15 == characteristics[3]);
    assert(6 == characteristics[4]);
    assert(15 == characteristics[5]);

    characteristics_free(characteristics);
    rnd_free(rnd);
}


static void
characteristics_struct_alloc_method_special_NPC_STRENGTH_test(void)
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
characteristics_alloc_method_special_NPC_DEXTERITY_CONSTITUTION_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    int *characteristics = characteristics_alloc(rnd,
                                                 characteristic_generation_method_special_NPC,
                                                 characteristic_flag_dexterity | characteristic_flag_constitution);

    assert(6 == characteristics[0]);
    assert(15 == characteristics[1]);
    assert(6 == characteristics[2]);
    assert(17 == characteristics[3]);
    assert(9 == characteristics[4]);
    assert(15 == characteristics[5]);

    characteristics_free(characteristics);
    rnd_free(rnd);
}


static void
characteristics_struct_alloc_method_special_NPC_DEXTERITY_CONSTITUTION_test(void)
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


void
characteristics_test(void)
{
    characteristics_alloc_invalid_test();
    characteristics_alloc_simple_test();
    characteristics_alloc_simple_method_test();
    characteristics_alloc_method1_test();
    characteristics_alloc_method2_test();
    characteristics_alloc_method3_test();
    characteristics_alloc_method_3_test();
    characteristics_alloc_method4_test();
    characteristics_alloc_method_general_NPC_test();
    characteristics_struct_alloc_method_general_NPC_test();
    characteristics_alloc_method_special_NPC_test();
    characteristics_struct_alloc_method_special_NPC_test();
    characteristics_alloc_method_special_NPC_STRENGTH_test();
    characteristics_struct_alloc_method_special_NPC_STRENGTH_test();
    characteristics_alloc_method_special_NPC_DEXTERITY_CONSTITUTION_test();
    characteristics_struct_alloc_method_special_NPC_DEXTERITY_CONSTITUTION_test();
}
