#include <assert.h>
#include <background/background.h>
#include <base/base.h>
#include <json/json.h>
#include <treasure/treasure.h>


void
treasure_test(void);


static void
treasure_create_json_object_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_median();
    struct treasure treasure;
    treasure_initialize(&treasure);
    treasure_generate_magic_items(&treasure, rnd, 1, ANY_MAGIC_ITEM);
    struct cJSON *json_object = treasure_create_json_object(&treasure);

    assert(cJSON_IsObject(json_object));

    char *json_string = cJSON_PrintUnformatted(json_object);
    char const *expected = "{"
                           "\"struct\":\"treasure\","
                           "\"rev\":0,"
                           "\"type\":null,"
                           "\"coins\":{"
                               "\"struct\":\"coins\","
                               "\"rev\":0,"
                               "\"pp\":0,"
                               "\"gp\":0,"
                               "\"ep\":0,"
                               "\"sp\":0,"
                               "\"cp\":0"
                           "},"
                           "\"gems\":[],"
                           "\"jewelry\":[],"
                           "\"maps\":[],"
                           "\"magic_items\":["
                               "{"
                                   "\"struct\":\"magic_item\","
                                   "\"rev\":0,"
                                   "\"experience_points\":2000,"
                                   "\"true_description\":\"cloak of protection +2\","
                                   "\"true_details\":[],"
                                   "\"true_value_in_cp\":4000000,"
                                   "\"type\":\"miscellaneous\""
                               "}"
                           "]"
                           "}";
    assert(str_eq(expected, json_string));

    free(json_string);
    cJSON_Delete(json_object);

    treasure_finalize(&treasure);
    rnd_free(rnd);
}


static void
treasure_initialize_test(void)
{
    struct treasure treasure;
    treasure_initialize(&treasure);

    assert(NULL == treasure.type);
    assert(coins_is_zero(treasure.coins));
    assert(NULL == treasure.gems);
    assert(0 == treasure.gems_count);
    assert(NULL == treasure.jewelry);
    assert(0 == treasure.jewelry_count);
    assert(NULL == treasure.maps);
    assert(0 == treasure.maps_count);
    assert(NULL == treasure.magic_items);
    assert(0 == treasure.magic_items_count);

    treasure_finalize(&treasure);
}


static void
treasure_initialize_from_json_object_for_empty_object_test(void)
{
    char const *json_string = "{}";
    struct cJSON *json_object = cJSON_Parse(json_string);
    struct treasure treasure;
    treasure_initialize_from_json_object(&treasure, json_object);

    assert(NULL == treasure.type);
    assert(coins_is_zero(treasure.coins));
    assert(NULL == treasure.gems);
    assert(0 == treasure.gems_count);
    assert(NULL == treasure.jewelry);
    assert(0 == treasure.jewelry_count);
    assert(NULL == treasure.maps);
    assert(0 == treasure.maps_count);
    assert(NULL == treasure.magic_items);
    assert(0 == treasure.magic_items_count);

    treasure_finalize(&treasure);
}


static void
treasure_initialize_from_json_object_for_empty_array_test(void)
{
    char const *json_string = "[]";
    struct cJSON *json_object = cJSON_Parse(json_string);
    struct treasure treasure;
    treasure_initialize_from_json_object(&treasure, json_object);

    assert(NULL == treasure.type);
    assert(coins_is_zero(treasure.coins));
    assert(NULL == treasure.gems);
    assert(0 == treasure.gems_count);
    assert(NULL == treasure.jewelry);
    assert(0 == treasure.jewelry_count);
    assert(NULL == treasure.maps);
    assert(0 == treasure.maps_count);
    assert(NULL == treasure.magic_items);
    assert(0 == treasure.magic_items_count);

    treasure_finalize(&treasure);
}


static void
treasure_initialize_from_json_object_for_complete_object_test(void)
{
    char const *json_string = "{\n"
                              "  \"struct\": \"treasure\",\n"
                              "  \"rev\": 0,\n"
                              "  \"type\": null,\n"
                              "  \"coins\": {\n"
                              "    \"struct\": \"coins\",\n"
                              "    \"rev\": 0,\n"
                              "    \"pp\": 0,\n"
                              "    \"gp\": 0,\n"
                              "    \"ep\": 0,\n"
                              "    \"sp\": 0,\n"
                              "    \"cp\": 0\n"
                              "  },\n"
                              "  \"gems\": [],\n"
                              "  \"jewelry\": [],\n"
                              "  \"maps\": [],\n"
                              "  \"magic_items\": [\n"
                              "    {\n"
                              "      \"struct\": \"magic_item\",\n"
                              "      \"rev\": 0,\n"
                              "      \"experience_points\": 2000,\n"
                              "      \"true_description\": \"cloak of protection +2\",\n"
                              "      \"true_details\": [],\n"
                              "      \"true_value_in_cp\": 4000000,\n"
                              "      \"type\": \"miscellaneous\"\n"
                              "    }\n"
                              "  ]\n"
                              "}";
    struct cJSON *json_object = cJSON_Parse(json_string);
    struct treasure treasure;
    treasure_initialize_from_json_object(&treasure, json_object);

    assert(NULL == treasure.type);
    assert(coins_is_zero(treasure.coins));
    assert(NULL == treasure.gems);
    assert(0 == treasure.gems_count);
    assert(NULL == treasure.jewelry);
    assert(0 == treasure.jewelry_count);
    assert(NULL == treasure.maps);
    assert(0 == treasure.maps_count);
    assert(NULL != treasure.magic_items);
    assert(1 == treasure.magic_items_count);

    assert(2000 == treasure.magic_items[0].experience_points);
    assert(str_eq("cloak of protection +2", treasure.magic_items[0].true_description));
    assert(NULL == treasure.magic_items[0].true_details);
    assert(4000000 == treasure.magic_items[0].true_value_in_cp);
    assert(magic_item_type_misc == treasure.magic_items[0].type);

    treasure_finalize(&treasure);
}


static void
treasure_generate_magic_items_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_min();
    struct treasure treasure;
    treasure_initialize(&treasure);

    treasure_generate_magic_items(&treasure, rnd, 1, ANY_MAGIC_ITEM);

    assert(NULL == treasure.type);
    assert(coins_is_zero(treasure.coins));
    assert(NULL == treasure.gems);
    assert(0 == treasure.gems_count);
    assert(NULL == treasure.jewelry);
    assert(0 == treasure.jewelry_count);
    assert(NULL == treasure.maps);
    assert(0 == treasure.maps_count);

    assert(NULL != treasure.magic_items);
    assert(1 == treasure.magic_items_count);

    assert(magic_item_type_potion == treasure.magic_items[0].type);
    assert(str_eq("animal control potion", treasure.magic_items[0].true_description));

    rnd_free(rnd);
    rnd = rnd_alloc_fake_median();

    treasure_generate_magic_items(&treasure, rnd, 1, ANY_MAGIC_ITEM);

    assert(NULL != treasure.magic_items);
    assert(2 == treasure.magic_items_count);

    assert(magic_item_type_potion == treasure.magic_items[0].type);
    assert(str_eq("animal control potion", treasure.magic_items[0].true_description));
    assert(magic_item_type_misc == treasure.magic_items[1].type);
    assert(str_eq("cloak of protection +2", treasure.magic_items[1].true_description));

    treasure_finalize(&treasure);
    rnd_free(rnd);
}


void
treasure_test(void)
{
    treasure_create_json_object_test();

    treasure_initialize_test();
    treasure_initialize_from_json_object_for_empty_object_test();
    treasure_initialize_from_json_object_for_empty_array_test();
    treasure_initialize_from_json_object_for_complete_object_test();

    treasure_generate_magic_items_test();
}
