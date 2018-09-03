#include <assert.h>
#include <base/base.h>
#include <treasure/treasure.h>


void
coins_test(void);


static void
coins_alloc_description_test(void)
{
    struct coins coins = {
            .pp=0,
            .gp=0,
            .ep=0,
            .sp=0,
            .cp=0,
    };
    char *description;

    description = coins_alloc_description(coins);
    assert(str_eq("", description));
    free_or_die(description);

    coins.pp = 1;
    description = coins_alloc_description(coins);
    assert(str_eq("1 platinum", description));
    free_or_die(description);

    coins.gp = 22;
    description = coins_alloc_description(coins);
    assert(str_eq("22 gold, 1 platinum", description));
    free_or_die(description);

    coins.ep = 333;
    description = coins_alloc_description(coins);
    assert(str_eq("333 electrum, 22 gold, 1 platinum", description));
    free_or_die(description);

    coins.sp = 4444;
    description = coins_alloc_description(coins);
    assert(str_eq("4444 silver, 333 electrum, 22 gold, 1 platinum", description));
    free_or_die(description);

    coins.cp = 55555;
    description = coins_alloc_description(coins);
    assert(str_eq("55555 copper, 4444 silver, 333 electrum, 22 gold, 1 platinum", description));
    free_or_die(description);
}


void
coins_test(void)
{
    coins_alloc_description_test();
}
