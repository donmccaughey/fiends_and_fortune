#include <assert.h>
#include <stddef.h>
#include <base/base.h>
#include <dungeon/dungeon.h>


void
generator_test(void);


static void
generator_add_digger_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);
    assert(0 == generator->diggers_count);
    
    struct digger *digger1 = generator_add_digger(generator,
                                                  point_make(1, 1, 1),
                                                  direction_north);
    assert(digger1);
    assert(1 == generator->diggers_count);
    assert(digger1 == generator->diggers[0]);

    struct digger *digger2 = generator_add_digger(generator,
                                                  point_make(2, 2, 2),
                                                  direction_south);
    assert(digger2);
    assert(2 == generator->diggers_count);
    assert(digger1 == generator->diggers[0]);
    assert(digger2 == generator->diggers[1]);

    struct digger *digger3 = generator_add_digger(generator,
                                                  point_make(3, 3, 3),
                                                  direction_east);
    assert(digger3);
    assert(3 == generator->diggers_count);
    assert(digger1 == generator->diggers[0]);
    assert(digger2 == generator->diggers[1]);
    assert(digger2 == generator->diggers[1]);
    assert(digger3 == generator->diggers[2]);

    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
generator_copy_digger_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();

    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);

    struct digger *digger1 = generator_add_digger(generator,
                                                  point_make(1, 1, 1),
                                                  direction_north);

    struct digger *digger2 = generator_add_digger(generator,
                                                  point_make(2, 2, 2),
                                                  direction_south);

    struct digger *digger3 = generator_copy_digger(generator, digger2);
    assert(digger3);
    assert(digger3 != digger2);
    assert(3 == generator->diggers_count);
    assert(direction_north == generator->diggers[0]->direction);
    assert(direction_south == generator->diggers[1]->direction);
    assert(direction_south == generator->diggers[2]->direction);

    struct digger *digger4 = generator_copy_digger(generator, digger1);
    assert(digger4);
    assert(digger4 != digger1);
    assert(4 == generator->diggers_count);
    assert(direction_north == generator->diggers[0]->direction);
    assert(direction_south == generator->diggers[1]->direction);
    assert(direction_south == generator->diggers[2]->direction);
    assert(direction_north == generator->diggers[3]->direction);

    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
generator_delete_digger_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();

    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);
    assert(0 == generator->diggers_count);

    struct digger *digger1 = generator_add_digger(generator,
                                                  point_make(1, 1, 1),
                                                  direction_north);
    struct digger *digger2 = generator_add_digger(generator,
                                                  point_make(2, 2, 2),
                                                  direction_south);
    struct digger *digger3 = generator_add_digger(generator,
                                                  point_make(3, 3, 3),
                                                  direction_east);

    generator_delete_digger(generator, digger2);
    assert(2 == generator->diggers_count);
    assert(digger1 == generator->diggers[0]);
    assert(digger3 == generator->diggers[1]);

    generator_delete_digger(generator, digger1);
    assert(1 == generator->diggers_count);
    assert(digger3 == generator->diggers[0]);

    generator_delete_digger(generator, digger3);
    assert(0 == generator->diggers_count);

    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


void generator_test(void)
{
    generator_add_digger_test();
    generator_copy_digger_test();
    generator_delete_digger_test();
}
