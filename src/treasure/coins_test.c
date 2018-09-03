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


static void
coins_alloc_gp_cp_description_test(void)
{
    char *description;

    description = coins_alloc_gp_cp_description(0);
    assert(str_eq("0 cp", description));
    free_or_die(description);

    description = coins_alloc_gp_cp_description(199);
    assert(str_eq("199 cp", description));
    free_or_die(description);

    description = coins_alloc_gp_cp_description(200);
    assert(str_eq("1 gp", description));
    free_or_die(description);

    description = coins_alloc_gp_cp_description(201);
    assert(str_eq("1 gp, 1 cp", description));
    free_or_die(description);

    description = coins_alloc_gp_cp_description(399);
    assert(str_eq("1 gp, 199 cp", description));
    free_or_die(description);

    description = coins_alloc_gp_cp_description(400);
    assert(str_eq("2 gp", description));
    free_or_die(description);
}


static void
coins_is_zero_test(void)
{
    struct coins coins = {
            .pp=0,
            .gp=0,
            .ep=0,
            .sp=0,
            .cp=0,
    };

    assert(coins_is_zero(coins));

    coins.cp = 1;
    assert( ! coins_is_zero(coins));

    coins.cp = 0;
    coins.sp = 1;
    assert( ! coins_is_zero(coins));

    coins.sp = 0;
    coins.ep = 1;
    assert( ! coins_is_zero(coins));

    coins.ep = 0;
    coins.gp = 1;
    assert( ! coins_is_zero(coins));

    coins.gp = 0;
    coins.pp = 1;
    assert( ! coins_is_zero(coins));

    coins.pp = 0;
    assert(coins_is_zero(coins));
}


static void
coins_make_test(void)
{
    struct coins coins = coins_make(1, 2, 3, 4, 5);

    assert(1 == coins.pp);
    assert(2 == coins.gp);
    assert(3 == coins.ep);
    assert(4 == coins.sp);
    assert(5 == coins.cp);
}


static void
coins_make_from_cp_test(void)
{
    struct coins coins = coins_make_from_cp(0);
    assert(coins_is_zero(coins));

    coins = coins_make_from_cp(9);
    assert(0 == coins.pp);
    assert(0 == coins.gp);
    assert(0 == coins.ep);
    assert(0 == coins.sp);
    assert(9 == coins.cp);

    coins = coins_make_from_cp(10);
    assert(0 == coins.pp);
    assert(0 == coins.gp);
    assert(0 == coins.ep);
    assert(1 == coins.sp);
    assert(0 == coins.cp);

    coins = coins_make_from_cp(11);
    assert(0 == coins.pp);
    assert(0 == coins.gp);
    assert(0 == coins.ep);
    assert(1 == coins.sp);
    assert(1 == coins.cp);

    coins = coins_make_from_cp(99);
    assert(0 == coins.pp);
    assert(0 == coins.gp);
    assert(0 == coins.ep);
    assert(9 == coins.sp);
    assert(9 == coins.cp);

    coins = coins_make_from_cp(100);
    assert(0 == coins.pp);
    assert(0 == coins.gp);
    assert(1 == coins.ep);
    assert(0 == coins.sp);
    assert(0 == coins.cp);

    coins = coins_make_from_cp(111);
    assert(0 == coins.pp);
    assert(0 == coins.gp);
    assert(1 == coins.ep);
    assert(1 == coins.sp);
    assert(1 == coins.cp);

    coins = coins_make_from_cp(199);
    assert(0 == coins.pp);
    assert(0 == coins.gp);
    assert(1 == coins.ep);
    assert(9 == coins.sp);
    assert(9 == coins.cp);

    coins = coins_make_from_cp(200);
    assert(0 == coins.pp);
    assert(1 == coins.gp);
    assert(0 == coins.ep);
    assert(0 == coins.sp);
    assert(0 == coins.cp);

    coins = coins_make_from_cp(999);
    assert(0 == coins.pp);
    assert(4 == coins.gp);
    assert(1 == coins.ep);
    assert(9 == coins.sp);
    assert(9 == coins.cp);

    coins = coins_make_from_cp(1000);
    assert(1 == coins.pp);
    assert(0 == coins.gp);
    assert(0 == coins.ep);
    assert(0 == coins.sp);
    assert(0 == coins.cp);

    coins = coins_make_from_cp(1999);
    assert(1 == coins.pp);
    assert(4 == coins.gp);
    assert(1 == coins.ep);
    assert(9 == coins.sp);
    assert(9 == coins.cp);
}


static void
coins_make_zero_test(void)
{
    struct coins coins = coins_make_zero();
    assert(coins_is_zero(coins));
}


void
coins_test(void)
{
    coins_alloc_description_test();
    coins_alloc_gp_cp_description_test();
    coins_is_zero_test();
    coins_make_test();
    coins_make_from_cp_test();
    coins_make_zero_test();
}
