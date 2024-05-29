#include "coins.h"

#include <base/base.h>
#include <background/background.h>
#include <json/json.h>

#include "astr.h"


char *
coins_alloc_description(struct coins coins)
{
    char *description = astr_empty();
    char const *separator = "";
    if (coins.cp) {
        str_realloc_append_formatted(&description, "%d copper", coins.cp);
        separator = ", ";
    }
    if (coins.sp) {
        str_realloc_append_formatted(&description, "%s%d silver", separator, coins.sp);
        separator = ", ";
    }
    if (coins.ep) {
        str_realloc_append_formatted(&description, "%s%d electrum", separator, coins.ep);
        separator = ", ";
    }
    if (coins.gp) {
        str_realloc_append_formatted(&description, "%s%d gold", separator, coins.gp);
        separator = ", ";
    }
    if (coins.pp) {
        str_realloc_append_formatted(&description, "%s%d platinum", separator, coins.pp);
        separator = ", ";
    }
    return description;
}


char *
coins_alloc_gp_cp_description(int cp)
{
    struct coins coins = coins_make_from_gp_cp(0, cp);
    if (coins.gp && coins.cp) {
        return str_alloc_formatted("%i gp, %i cp", coins.gp, coins.cp);
    } else if (coins.gp) {
        return str_alloc_formatted("%i gp", coins.gp);
    } else {
        return str_alloc_formatted("%i cp", coins.cp);
    }
}


struct cJSON *
coins_create_json_object(struct coins *coins)
{
    struct cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "struct", "coins");
    cJSON_AddNumberToObject(json, "rev", 0);
    cJSON_AddNumberToObject(json, "pp", coins->pp);
    cJSON_AddNumberToObject(json, "gp", coins->gp);
    cJSON_AddNumberToObject(json, "ep", coins->ep);
    cJSON_AddNumberToObject(json, "sp", coins->sp);
    cJSON_AddNumberToObject(json, "cp", coins->cp);
    return json;
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


bool
coins_is_zero(struct coins coins)
{
    return 0 == coins.pp
        && 0 == coins.gp
        && 0 == coins.ep
        && 0 == coins.sp
        && 0 == coins.cp;
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
coins_make_from_json_object(struct cJSON *json_object)
{
    struct coins coins = coins_make_zero();

    if ( ! cJSON_IsObject(json_object)) return coins;
    if ( !json_object_has_struct_member(json_object, "coins")) return coins;

    coins.pp = json_object_get_int_value(json_object, "pp", 0);
    coins.gp = json_object_get_int_value(json_object, "gp", 0);
    coins.ep = json_object_get_int_value(json_object, "ep", 0);
    coins.sp = json_object_get_int_value(json_object, "sp", 0);
    coins.cp = json_object_get_int_value(json_object, "cp", 0);
    return coins;
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
