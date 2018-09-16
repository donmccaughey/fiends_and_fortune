#include <assert.h>
#include <base/base.h>
#include <cJSON/cJSON.h>
#include <treasure/treasure.h>


void
coins_test(void);


static void
coins_alloc_description_test(void)
{
    struct coins coins = coins_make_zero();
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
coins_create_json_object_test(void)
{
    struct coins coins = coins_make(1, 2, 3, 4, 5);
    struct cJSON *json = coins_create_json_object(&coins);

    assert(cJSON_IsObject(json));

    char *json_string = cJSON_PrintUnformatted(json);
    char const *expected = "{"
                           "\"struct\":\"coins\","
                           "\"rev\":0,"
                           "\"pp\":1,"
                           "\"gp\":2,"
                           "\"ep\":3,"
                           "\"sp\":4,"
                           "\"cp\":5"
                           "}";
    assert(str_eq(expected, json_string));

    free(json_string);
    cJSON_Delete(json);
}


static void
coins_create_json_object_for_zero_test(void)
{
    struct coins coins = coins_make_zero();
    struct cJSON *json = coins_create_json_object(&coins);

    assert(cJSON_IsObject(json));

    char *json_string = cJSON_PrintUnformatted(json);
    char const *expected = "{"
                           "\"struct\":\"coins\","
                           "\"rev\":0,"
                           "\"pp\":0,"
                           "\"gp\":0,"
                           "\"ep\":0,"
                           "\"sp\":0,"
                           "\"cp\":0"
                           "}";
    assert(str_eq(expected, json_string));

    free(json_string);
    cJSON_Delete(json);
}


static void
coins_is_zero_test(void)
{
    struct coins coins = coins_make_zero();

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
coins_make_from_gp_cp_test(void)
{
    struct coins coins = coins_make_from_gp_cp(0, 0);
    assert(coins_is_zero(coins));

    coins = coins_make_from_gp_cp(1, 2);
    assert(0 == coins.pp);
    assert(1 == coins.gp);
    assert(0 == coins.ep);
    assert(0 == coins.sp);
    assert(2 == coins.cp);

    coins = coins_make_from_gp_cp(1, 200);
    assert(0 == coins.pp);
    assert(2 == coins.gp);
    assert(0 == coins.ep);
    assert(0 == coins.sp);
    assert(0 == coins.cp);

    coins = coins_make_from_gp_cp(1, 203);
    assert(0 == coins.pp);
    assert(2 == coins.gp);
    assert(0 == coins.ep);
    assert(0 == coins.sp);
    assert(3 == coins.cp);
}


static void
coins_make_from_json_object_for_empty_object_test(void)
{
    char const *json_string = "{}";
    struct cJSON *json = cJSON_Parse(json_string);
    struct coins coins = coins_make_from_json_object(json);

    assert(coins_is_zero(coins));

    cJSON_Delete(json);
}


static void
coins_make_from_json_object_for_empty_array_test(void)
{
    char const *json_string = "[]";
    struct cJSON *json = cJSON_Parse(json_string);
    struct coins coins = coins_make_from_json_object(json);

    assert(coins_is_zero(coins));

    cJSON_Delete(json);
}


static void
coins_make_from_json_object_for_complete_object_test(void)
{
    char const *json_string = "{\n"
                              "  \"struct\": \"coins\",\n"
                              "  \"rev\": 0,\n"
                              "  \"pp\": 5,\n"
                              "  \"gp\": 10,\n"
                              "  \"ep\": 50,\n"
                              "  \"sp\": 100,\n"
                              "  \"cp\": 200\n"
                              "}";
    struct cJSON *json = cJSON_Parse(json_string);
    struct coins coins = coins_make_from_json_object(json);
    
    assert(5 == coins.pp);
    assert(10 == coins.gp);
    assert(50 == coins.ep);
    assert(100 == coins.sp);
    assert(200 == coins.cp);

    cJSON_Delete(json);
}


static void
coins_make_from_json_object_for_missing_struct_key_test(void)
{
    char const *json_string = "{\n"
                              "  \"rev\": 0,\n"
                              "  \"pp\": 5,\n"
                              "  \"gp\": 10,\n"
                              "  \"ep\": 50,\n"
                              "  \"sp\": 100,\n"
                              "  \"cp\": 200\n"
                              "}";
    struct cJSON *json = cJSON_Parse(json_string);
    struct coins coins = coins_make_from_json_object(json);

    assert(coins_is_zero(coins));

    cJSON_Delete(json);
}


static void
coins_make_zero_test(void)
{
    struct coins coins = coins_make_zero();
    assert(coins_is_zero(coins));
}


static void
coins_normalize_test(void)
{
    struct coins coins = coins_make_zero();
    struct coins normalized = coins_normalize(coins);
    assert(coins_is_zero(normalized));

    coins = coins_make(1, 5, 2, 10, 11);
    normalized = coins_normalize(coins);
    assert(2 == normalized.pp);
    assert(1 == normalized.gp);
    assert(1 == normalized.ep);
    assert(1 == normalized.sp);
    assert(1 == normalized.cp);

    coins = coins_make(0, 4, 1, 9, 10);
    normalized = coins_normalize(coins);
    assert(1 == normalized.pp);
    assert(0 == normalized.gp);
    assert(0 == normalized.ep);
    assert(0 == normalized.sp);
    assert(0 == normalized.cp);
}


static void
coins_normalize_to_gp_cp_test(void)
{
    struct coins coins = coins_make_zero();
    struct coins normalized = coins_normalize_to_gp_cp(coins);
    assert(coins_is_zero(normalized));

    coins = coins_make(1, 5, 2, 10, 11);
    normalized = coins_normalize_to_gp_cp(coins);
    assert(0 == normalized.pp);
    assert(11 == normalized.gp);
    assert(0 == normalized.ep);
    assert(0 == normalized.sp);
    assert(111 == normalized.cp);

    coins = coins_make(0, 4, 1, 9, 10);
    normalized = coins_normalize_to_gp_cp(coins);
    assert(0 == normalized.pp);
    assert(5 == normalized.gp);
    assert(0 == normalized.ep);
    assert(0 == normalized.sp);
    assert(0 == normalized.cp);

    coins = coins_make(0, 4, 2, 9, 10);
    normalized = coins_normalize_to_gp_cp(coins);
    assert(0 == normalized.pp);
    assert(5 == normalized.gp);
    assert(0 == normalized.ep);
    assert(0 == normalized.sp);
    assert(100 == normalized.cp);
}


// ----- upward conversions -----

static void
coins_cp_to_sp_test(void)
{
    struct coins coins;
    struct coins converted;

    coins = coins_make_zero();
    converted = coins_cp_to_sp(coins);
    assert(0 == converted.pp);
    assert(0 == converted.gp);
    assert(0 == converted.ep);
    assert(0 == converted.sp);
    assert(0 == converted.cp);

    coins = coins_make(1, 2, 3, 4, 5);
    converted = coins_cp_to_sp(coins);
    assert(1 == converted.pp);
    assert(2 == converted.gp);
    assert(3 == converted.ep);
    assert(4 == converted.sp);
    assert(5 == converted.cp);

    coins = coins_make(1, 2, 3, 4, 11);
    converted = coins_cp_to_sp(coins);
    assert(1 == converted.pp);
    assert(2 == converted.gp);
    assert(3 == converted.ep);
    assert(5 == converted.sp);
    assert(1 == converted.cp);
}


static void
coins_sp_to_ep_test(void)
{
    struct coins coins;
    struct coins converted;

    coins = coins_make_zero();
    converted = coins_sp_to_ep(coins);
    assert(0 == converted.pp);
    assert(0 == converted.gp);
    assert(0 == converted.ep);
    assert(0 == converted.sp);
    assert(0 == converted.cp);

    coins = coins_make(1, 2, 3, 4, 5);
    converted = coins_sp_to_ep(coins);
    assert(1 == converted.pp);
    assert(2 == converted.gp);
    assert(3 == converted.ep);
    assert(4 == converted.sp);
    assert(5 == converted.cp);

    coins = coins_make(1, 2, 3, 11, 5);
    converted = coins_sp_to_ep(coins);
    assert(1 == converted.pp);
    assert(2 == converted.gp);
    assert(4 == converted.ep);
    assert(1 == converted.sp);
    assert(5 == converted.cp);
}


static void
coins_ep_to_gp_test(void)
{
    struct coins coins;
    struct coins converted;

    coins = coins_make_zero();
    converted = coins_ep_to_gp(coins);
    assert(0 == converted.pp);
    assert(0 == converted.gp);
    assert(0 == converted.ep);
    assert(0 == converted.sp);
    assert(0 == converted.cp);

    coins = coins_make(1, 2, 1, 4, 5);
    converted = coins_ep_to_gp(coins);
    assert(1 == converted.pp);
    assert(2 == converted.gp);
    assert(1 == converted.ep);
    assert(4 == converted.sp);
    assert(5 == converted.cp);

    coins = coins_make(1, 2, 3, 4, 5);
    converted = coins_ep_to_gp(coins);
    assert(1 == converted.pp);
    assert(3 == converted.gp);
    assert(1 == converted.ep);
    assert(4 == converted.sp);
    assert(5 == converted.cp);
}


static void
coins_gp_to_pp_test(void)
{
    struct coins coins;
    struct coins converted;

    coins = coins_make_zero();
    converted = coins_gp_to_pp(coins);
    assert(0 == converted.pp);
    assert(0 == converted.gp);
    assert(0 == converted.ep);
    assert(0 == converted.sp);
    assert(0 == converted.cp);

    coins = coins_make(1, 2, 3, 4, 5);
    converted = coins_gp_to_pp(coins);
    assert(1 == converted.pp);
    assert(2 == converted.gp);
    assert(3 == converted.ep);
    assert(4 == converted.sp);
    assert(5 == converted.cp);

    coins = coins_make(1, 6, 3, 4, 5);
    converted = coins_gp_to_pp(coins);
    assert(2 == converted.pp);
    assert(1 == converted.gp);
    assert(3 == converted.ep);
    assert(4 == converted.sp);
    assert(5 == converted.cp);
}


// ----- downward conversions -----

static void
coins_pp_to_gp_test(void)
{
    struct coins coins;
    struct coins converted;

    coins = coins_make_zero();
    converted = coins_pp_to_gp(coins);
    assert(0 == converted.pp);
    assert(0 == converted.gp);
    assert(0 == converted.ep);
    assert(0 == converted.sp);
    assert(0 == converted.cp);

    coins = coins_make(1, 2, 3, 4, 5);
    converted = coins_pp_to_gp(coins);
    assert(0 == converted.pp);
    assert(7 == converted.gp);
    assert(3 == converted.ep);
    assert(4 == converted.sp);
    assert(5 == converted.cp);
}


static void
coins_gp_to_ep_test(void)
{
    struct coins coins;
    struct coins converted;

    coins = coins_make_zero();
    converted = coins_gp_to_ep(coins);
    assert(0 == converted.pp);
    assert(0 == converted.gp);
    assert(0 == converted.ep);
    assert(0 == converted.sp);
    assert(0 == converted.cp);

    coins = coins_make(1, 2, 3, 4, 5);
    converted = coins_gp_to_ep(coins);
    assert(1 == converted.pp);
    assert(0 == converted.gp);
    assert(7 == converted.ep);
    assert(4 == converted.sp);
    assert(5 == converted.cp);
}


static void
coins_ep_to_sp_test(void)
{
    struct coins coins;
    struct coins converted;

    coins = coins_make_zero();
    converted = coins_ep_to_sp(coins);
    assert(0 == converted.pp);
    assert(0 == converted.gp);
    assert(0 == converted.ep);
    assert(0 == converted.sp);
    assert(0 == converted.cp);

    coins = coins_make(1, 2, 3, 4, 5);
    converted = coins_ep_to_sp(coins);
    assert(1 == converted.pp);
    assert(2 == converted.gp);
    assert(0 == converted.ep);
    assert(34 == converted.sp);
    assert(5 == converted.cp);
}


static void
coins_sp_to_cp_test(void)
{
    struct coins coins;
    struct coins converted;

    coins = coins_make_zero();
    converted = coins_sp_to_cp(coins);
    assert(0 == converted.pp);
    assert(0 == converted.gp);
    assert(0 == converted.ep);
    assert(0 == converted.sp);
    assert(0 == converted.cp);

    coins = coins_make(1, 2, 3, 4, 5);
    converted = coins_sp_to_cp(coins);
    assert(1 == converted.pp);
    assert(2 == converted.gp);
    assert(3 == converted.ep);
    assert(0 == converted.sp);
    assert(45 == converted.cp);
}


void
coins_test(void)
{
    coins_alloc_description_test();
    coins_alloc_gp_cp_description_test();
    coins_create_json_object_test();
    coins_create_json_object_for_zero_test();
    coins_is_zero_test();
    coins_make_test();
    coins_make_from_cp_test();
    coins_make_from_gp_cp_test();
    coins_make_from_json_object_for_empty_object_test();
    coins_make_from_json_object_for_empty_array_test();
    coins_make_from_json_object_for_complete_object_test();
    coins_make_from_json_object_for_missing_struct_key_test();
    coins_make_zero_test();
    coins_normalize_test();
    coins_normalize_to_gp_cp_test();
    coins_cp_to_sp_test();
    coins_sp_to_ep_test();
    coins_ep_to_gp_test();
    coins_gp_to_pp_test();
    coins_pp_to_gp_test();
    coins_gp_to_ep_test();
    coins_ep_to_sp_test();
    coins_sp_to_cp_test();
}
