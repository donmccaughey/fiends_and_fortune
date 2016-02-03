#include <assert.h>
#include <stddef.h>

#include "common/rnd.h"

#include "dungeon/digger.h"
#include "dungeon/dungeon.h"
#include "dungeon/dungeon_generator.h"


void
dungeon_generator_test(void);


static void
dungeon_generator_take_on_digger_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    
    struct dungeon_generator *generator = dungeon_generator_alloc(dungeon, global_rnd);
    assert(0 == generator->diggers_count);
    
    struct digger *digger1 = digger_alloc(point_make(1, 1, 1), North);
    struct digger *digger2 = digger_alloc(point_make(2, 2, 2), North);
    struct digger *digger3 = digger_alloc(point_make(3, 3, 3), North);
    
    dungeon_generator_take_on_digger(generator, digger1);
    assert(1 == generator->diggers_count);
    assert(generator == digger1->generator);
    
    dungeon_generator_give_up_digger(generator, digger1);
    assert(0 == generator->diggers_count);
    assert(NULL == digger1->generator);
    
    dungeon_generator_take_on_digger(generator, digger1);
    assert(1 == generator->diggers_count);
    assert(generator == digger1->generator);
    
    dungeon_generator_take_on_digger(generator, digger2);
    assert(2 == generator->diggers_count);
    assert(generator == digger2->generator);
    
    dungeon_generator_take_on_digger(generator, digger3);
    assert(3 == generator->diggers_count);
    assert(generator == digger3->generator);
    
    dungeon_generator_give_up_digger(generator, digger2);
    assert(2 == generator->diggers_count);
    assert(NULL == digger2->generator);
    digger_free(digger2);
    
    assert(digger1 == generator->diggers[0]);
    assert(digger3 == generator->diggers[1]);
    
    dungeon_generator_give_up_digger(generator, digger1);
    assert(1 == generator->diggers_count);
    assert(NULL == digger1->generator);
    digger_free(digger1);
    
    assert(digger3 == generator->diggers[0]);
    
    dungeon_generator_free(generator);
    dungeon_free(dungeon);
}


void dungeon_generator_test(void)
{
    dungeon_generator_take_on_digger_test();
}
