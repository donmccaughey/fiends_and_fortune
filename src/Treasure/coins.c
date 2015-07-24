#include "coins.h"

#include "alloc_or_die.h"


char *
coins_alloc_gp_cp_description(int cp)
{
    struct coins coins = coins_make_from_gp_cp(0, cp);
    char *description;
    if (coins.gp && coins.cp) {
        asprintf_or_die(&description, "%i gp, %i cp", coins.gp, coins.cp);
    } else if (coins.gp) {
        asprintf_or_die(&description, "%i gp", coins.gp);
    } else {
        asprintf_or_die(&description, "%i cp", coins.cp);
    }
    return description;
}


struct coins
coins_cp_to_sp(struct coins coins)
{
    int from_cp = coins.cp;
    coins.sp += from_cp / 10;
    coins.cp = from_cp % 10;
    return coins;
}


struct coins
coins_ep_to_gp(struct coins coins)
{
    int from_ep = coins.ep;
    coins.gp += from_ep / 2;
    coins.ep = from_ep % 2;
    return coins;
}


struct coins
coins_ep_to_sp(struct coins coins)
{
    coins.sp += ep_to_sp(coins.ep);
    coins.ep = 0;
    return coins;
}


struct coins
coins_gp_to_ep(struct coins coins)
{
    coins.ep += gp_to_ep(coins.gp);
    coins.gp = 0;
    return coins;
}


inline struct coins
coins_gp_to_pp(struct coins coins)
{
    int from_gp = coins.gp;
    coins.pp += from_gp / 5;
    coins.gp = from_gp % 5;
    return coins;
}


struct coins
coins_make(int pp, int gp, int ep, int sp, int cp)
{
    return (struct coins){
        .pp=pp,
        .gp=gp,
        .ep=ep,
        .sp=sp,
        .cp=cp
    };
}


struct coins
coins_make_from_cp(int cp)
{
    return coins_normalize(coins_make(0, 0, 0, 0, cp));
}


struct coins
coins_make_from_gp_cp(int gp, int cp)
{
    return coins_normalize_to_gp_cp(coins_make(0, gp, 0, 0, cp));
}


struct coins
coins_make_zero(void)
{
    return coins_make(0, 0, 0, 0, 0);
}


struct coins
coins_normalize(struct coins coins)
{
    coins = coins_cp_to_sp(coins);
    coins = coins_sp_to_ep(coins);
    coins = coins_ep_to_gp(coins);
    coins = coins_gp_to_pp(coins);
    return coins;
}



struct coins
coins_normalize_to_gp_cp(struct coins coins)
{
    // up to gp
    coins = coins_cp_to_sp(coins);
    coins = coins_sp_to_ep(coins);
    coins = coins_ep_to_gp(coins);
    // down to gp
    coins = coins_pp_to_gp(coins);
    // down to cp
    coins = coins_ep_to_sp(coins);
    coins = coins_sp_to_cp(coins);
    return coins;
}


struct coins
coins_pp_to_gp(struct coins coins)
{
    coins.gp += pp_to_gp(coins.pp);
    coins.pp = 0;
    return coins;
}


struct coins
coins_sp_to_cp(struct coins coins)
{
    coins.cp += sp_to_cp(coins.sp);
    coins.sp = 0;
    return coins;
}


struct coins
coins_sp_to_ep(struct coins coins)
{
    int from_sp = coins.sp;
    coins.ep += from_sp / 10;
    coins.sp = from_sp % 10;
    return coins;
}


int
coins_to_cp(struct coins coins)
{
    coins = coins_pp_to_gp(coins);
    coins = coins_gp_to_ep(coins);
    coins = coins_ep_to_sp(coins);
    coins = coins_sp_to_cp(coins);
    return coins.cp;
}
