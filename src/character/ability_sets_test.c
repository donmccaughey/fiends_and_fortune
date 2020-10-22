#include <assert.h>
#include <base/base.h>
#include <character/character.h>


void
ability_sets_test(void);


static void
ability_sets_alloc_method_4_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);
    struct ability_sets *sets = ability_sets_alloc_method_4(rnd);

    for (int i = 0; i < 12; ++i) {
        assert(6 == sets->values[i].strength);
        assert(15 == sets->values[i].intelligence);
        assert(6 == sets->values[i].wisdom);
        assert(15 == sets->values[i].dexterity);
        assert(6 == sets->values[i].constitution);
        assert(15 == sets->values[i].charisma);
    }

    ability_sets_free(sets);
    rnd_free(rnd);
}


static void
ability_sets_alloc_method_4_random_test(void)
{
    struct rnd *rnd = rnd_alloc_lcg(100200300);
    struct ability_sets *sets = ability_sets_alloc_method_4(rnd);

    assert(18 == sets->values[0].strength);
    assert(15 == sets->values[0].intelligence);
    assert(12 == sets->values[0].wisdom);
    assert(14 == sets->values[0].dexterity);
    assert(11 == sets->values[0].constitution);
    assert(8 == sets->values[0].charisma);

    assert(11 == sets->values[1].strength);
    assert(8 == sets->values[1].intelligence);
    assert(11 == sets->values[1].wisdom);
    assert(17 == sets->values[1].dexterity);
    assert(18 == sets->values[1].constitution);
    assert(13 == sets->values[1].charisma);

    assert(9 == sets->values[2].strength);
    assert(12 == sets->values[2].intelligence);
    assert(13 == sets->values[2].wisdom);
    assert(11 == sets->values[2].dexterity);
    assert(13 == sets->values[2].constitution);
    assert(10 == sets->values[2].charisma);

    assert(11 == sets->values[3].strength);
    assert(11 == sets->values[3].intelligence);
    assert(7 == sets->values[3].wisdom);
    assert(10 == sets->values[3].dexterity);
    assert(10 == sets->values[3].constitution);
    assert(17 == sets->values[3].charisma);

    assert(10 == sets->values[4].strength);
    assert(12 == sets->values[4].intelligence);
    assert(14 == sets->values[4].wisdom);
    assert(8 == sets->values[4].dexterity);
    assert(12 == sets->values[4].constitution);
    assert(8 == sets->values[4].charisma);

    assert(13 == sets->values[5].strength);
    assert(13 == sets->values[5].intelligence);
    assert(6 == sets->values[5].wisdom);
    assert(11 == sets->values[5].dexterity);
    assert(9 == sets->values[5].constitution);
    assert(10 == sets->values[5].charisma);

    assert(11 == sets->values[6].strength);
    assert(10 == sets->values[6].intelligence);
    assert(8 == sets->values[6].wisdom);
    assert(7 == sets->values[6].dexterity);
    assert(12 == sets->values[6].constitution);
    assert(14 == sets->values[6].charisma);

    assert(11 == sets->values[7].strength);
    assert(9 == sets->values[7].intelligence);
    assert(8 == sets->values[7].wisdom);
    assert(12 == sets->values[7].dexterity);
    assert(12 == sets->values[7].constitution);
    assert(10 == sets->values[7].charisma);

    assert(13 == sets->values[8].strength);
    assert(12 == sets->values[8].intelligence);
    assert(4 == sets->values[8].wisdom);
    assert(8 == sets->values[8].dexterity);
    assert(13 == sets->values[8].constitution);
    assert(10 == sets->values[8].charisma);

    assert(16 == sets->values[9].strength);
    assert(9 == sets->values[9].intelligence);
    assert(9 == sets->values[9].wisdom);
    assert(11 == sets->values[9].dexterity);
    assert(10 == sets->values[9].constitution);
    assert(5 == sets->values[9].charisma);

    assert(6 == sets->values[10].strength);
    assert(7 == sets->values[10].intelligence);
    assert(12 == sets->values[10].wisdom);
    assert(13 == sets->values[10].dexterity);
    assert(10 == sets->values[10].constitution);
    assert(9 == sets->values[10].charisma);

    assert(7 == sets->values[11].strength);
    assert(9 == sets->values[11].intelligence);
    assert(11 == sets->values[11].wisdom);
    assert(9 == sets->values[11].dexterity);
    assert(8 == sets->values[11].constitution);
    assert(12 == sets->values[11].charisma);

    ability_sets_free(sets);
    rnd_free(rnd);
}


static void
ability_sets_sort_test(void)
{
    struct ability_sets sets;
    for (int i = 0; i < 12; ++i) {
        sets.values[i].strength = i;
        sets.values[i].intelligence = i;
        sets.values[i].wisdom = i;
        sets.values[i].dexterity = i;
        sets.values[i].constitution = i;
        sets.values[i].charisma = i;
    }

    ability_sets_sort(&sets);

    assert(11 == sets.values[0].strength);
    assert(11 == sets.values[0].intelligence);
    assert(11 == sets.values[0].wisdom);
    assert(11 == sets.values[0].dexterity);
    assert(11 == sets.values[0].constitution);
    assert(11 == sets.values[0].charisma);

    assert(10 == sets.values[1].strength);
    assert(10 == sets.values[1].intelligence);
    assert(10 == sets.values[1].wisdom);
    assert(10 == sets.values[1].dexterity);
    assert(10 == sets.values[1].constitution);
    assert(10 == sets.values[1].charisma);

    assert(9 == sets.values[2].strength);
    assert(9 == sets.values[2].intelligence);
    assert(9 == sets.values[2].wisdom);
    assert(9 == sets.values[2].dexterity);
    assert(9 == sets.values[2].constitution);
    assert(9 == sets.values[2].charisma);

    assert(8 == sets.values[3].strength);
    assert(8 == sets.values[3].intelligence);
    assert(8 == sets.values[3].wisdom);
    assert(8 == sets.values[3].dexterity);
    assert(8 == sets.values[3].constitution);
    assert(8 == sets.values[3].charisma);

    assert(7 == sets.values[4].strength);
    assert(7 == sets.values[4].intelligence);
    assert(7 == sets.values[4].wisdom);
    assert(7 == sets.values[4].dexterity);
    assert(7 == sets.values[4].constitution);
    assert(7 == sets.values[4].charisma);

    assert(6 == sets.values[5].strength);
    assert(6 == sets.values[5].intelligence);
    assert(6 == sets.values[5].wisdom);
    assert(6 == sets.values[5].dexterity);
    assert(6 == sets.values[5].constitution);
    assert(6 == sets.values[5].charisma);

    assert(5 == sets.values[6].strength);
    assert(5 == sets.values[6].intelligence);
    assert(5 == sets.values[6].wisdom);
    assert(5 == sets.values[6].dexterity);
    assert(5 == sets.values[6].constitution);
    assert(5 == sets.values[6].charisma);

    assert(4 == sets.values[7].strength);
    assert(4 == sets.values[7].intelligence);
    assert(4 == sets.values[7].wisdom);
    assert(4 == sets.values[7].dexterity);
    assert(4 == sets.values[7].constitution);
    assert(4 == sets.values[7].charisma);

    assert(3 == sets.values[8].strength);
    assert(3 == sets.values[8].intelligence);
    assert(3 == sets.values[8].wisdom);
    assert(3 == sets.values[8].dexterity);
    assert(3 == sets.values[8].constitution);
    assert(3 == sets.values[8].charisma);

    assert(2 == sets.values[9].strength);
    assert(2 == sets.values[9].intelligence);
    assert(2 == sets.values[9].wisdom);
    assert(2 == sets.values[9].dexterity);
    assert(2 == sets.values[9].constitution);
    assert(2 == sets.values[9].charisma);

    assert(1 == sets.values[10].strength);
    assert(1 == sets.values[10].intelligence);
    assert(1 == sets.values[10].wisdom);
    assert(1 == sets.values[10].dexterity);
    assert(1 == sets.values[10].constitution);
    assert(1 == sets.values[10].charisma);

    assert(0 == sets.values[11].strength);
    assert(0 == sets.values[11].intelligence);
    assert(0 == sets.values[11].wisdom);
    assert(0 == sets.values[11].dexterity);
    assert(0 == sets.values[11].constitution);
    assert(0 == sets.values[11].charisma);
}


void
ability_sets_test(void)
{
    ability_sets_alloc_method_4_test();
    ability_sets_alloc_method_4_random_test();
    ability_sets_sort_test();
}
