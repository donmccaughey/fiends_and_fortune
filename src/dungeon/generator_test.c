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
    assert(1 == generator->diggers_count);
    
    generator_delete_digger(generator, digger1);
    assert(0 == generator->diggers_count);

    digger1 = generator_add_digger(generator,
                                   point_make(1, 1, 1),
                                   direction_north);
    assert(1 == generator->diggers_count);

    struct digger *digger2 = generator_add_digger(generator,
                                                  point_make(2, 2, 2),
                                                  direction_north);
    assert(2 == generator->diggers_count);
    
    struct digger *digger3 = generator_add_digger(generator,
                                                  point_make(3, 3, 3),
                                                  direction_north);
    assert(3 == generator->diggers_count);
    
    generator_delete_digger(generator, digger2);
    assert(2 == generator->diggers_count);
    assert(digger1 == generator->diggers[0]);
    assert(digger3 == generator->diggers[1]);
    
    generator_delete_digger(generator, digger1);
    assert(1 == generator->diggers_count);
    assert(digger3 == generator->diggers[0]);
    
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


void generator_test(void)
{
    generator_add_digger_test();
}