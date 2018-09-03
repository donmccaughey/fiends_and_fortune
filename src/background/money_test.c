#include <assert.h>
#include <base/base.h>
#include <background/background.h>


void
money_test(void);


// ----- platinum -----

static void
pp_to_gp_test(void)
{
    assert(5 == pp_to_gp(1));
}


static void
pp_to_ep_test(void)
{
    assert(10 == pp_to_ep(1));
}


static void
pp_to_sp_test(void)
{
    assert(100 == pp_to_sp(1));
}


static void
pp_to_cp_test(void)
{
    assert(1000 == pp_to_cp(1));
}


// ----- gold -----

static void
gp_to_ep_test(void)
{
    assert(10 == gp_to_ep(5));
}


static void
gp_to_sp_test(void)
{
    assert(100 == gp_to_sp(5));
}


static void
gp_to_cp_test(void)
{
    assert(1000 == gp_to_cp(5));
}


// ----- electrum -----

static void
ep_to_sp_test(void)
{
    assert(100 == ep_to_sp(10));
}


static void
ep_to_cp_test(void)
{
    assert(1000 == ep_to_cp(10));
}


// ----- silver -----

static void
sp_to_cp_test(void)
{
    assert(1000 == sp_to_cp(100));
}


void
money_test(void)
{
    pp_to_gp_test();
    pp_to_ep_test();
    pp_to_sp_test();
    pp_to_cp_test();

    gp_to_ep_test();
    gp_to_sp_test();
    gp_to_cp_test();

    ep_to_sp_test();
    ep_to_cp_test();

    sp_to_cp_test();
}
