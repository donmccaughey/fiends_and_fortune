#include <assert.h>
#include <base/base.h>
#include <json/json.h>
#include <treasure/treasure.h>

#include "astr.h"


void
treasure_map_test(void);


static void
treasure_map_create_json_object_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_median();
    struct treasure_map map;
    treasure_map_initialize(&map);
    treasure_map_generate(&map, rnd);
    struct cJSON *json_object = treasure_map_create_json_object(&map);

    assert(cJSON_IsObject(json_object));

    char *json_string = cJSON_PrintUnformatted(json_object);
    char const *expected = "{"
                           "\"struct\":\"treasure_map\","
                           "\"rev\":0,"
                           "\"is_false\":false,"
                           "\"treasure\":{"
                               "\"struct\":\"treasure\","
                               "\"rev\":0,"
                               "\"type\":null,"
                               "\"coins\":{"
                                   "\"struct\":\"coins\","
                                   "\"rev\":0,"
                                   "\"pp\":1500,"
                                   "\"gp\":0,"
                                   "\"ep\":0,"
                                   "\"sp\":0,"
                                   "\"cp\":0"
                               "},"
                               "\"gems\":[],"
                               "\"jewelry\":[],"
                               "\"maps\":[],"
                               "\"magic_items\":[]"
                           "},"
                           "\"true_description\":\"map to monetary treasure of 1500 platinum 7 miles to the south, guarded in a lair\""
                           "}";
    assert(astr_eq(expected, json_string));

    free(json_string);
    cJSON_Delete(json_object);

    treasure_map_finalize(&map);
    rnd_free(rnd);
}


static void
treasure_map_initialize_test(void)
{
    struct treasure_map map;
    treasure_map_initialize(&map);

    assert( ! map.is_false);
    assert(coins_is_zero(map.treasure.coins));
    assert(0 == map.treasure.gems_count);
    assert(0 == map.treasure.jewelry_count);
    assert(0 == map.treasure.maps_count);
    assert(0 == map.treasure.magic_items_count);
    assert(NULL == map.true_description);

    treasure_map_finalize(&map);
}


static void
treasure_map_initialize_from_json_object_for_empty_object_test(void)
{
    char const *json_string = "{}";
    struct cJSON *json_object = cJSON_Parse(json_string);
    struct treasure_map map;
    treasure_map_initialize_from_json_object(&map, json_object);
    cJSON_Delete(json_object);

    assert( ! map.is_false);
    assert(coins_is_zero(map.treasure.coins));
    assert(0 == map.treasure.gems_count);
    assert(0 == map.treasure.jewelry_count);
    assert(0 == map.treasure.maps_count);
    assert(0 == map.treasure.magic_items_count);
    assert(NULL == map.true_description);

    treasure_map_finalize(&map);
}


static void
treasure_map_initialize_from_json_object_for_empty_array_test(void)
{
    char const *json_string = "[]";
    struct cJSON *json_object = cJSON_Parse(json_string);
    struct treasure_map map;
    treasure_map_initialize_from_json_object(&map, json_object);
    cJSON_Delete(json_object);

    assert( ! map.is_false);
    assert(coins_is_zero(map.treasure.coins));
    assert(0 == map.treasure.gems_count);
    assert(0 == map.treasure.jewelry_count);
    assert(0 == map.treasure.maps_count);
    assert(0 == map.treasure.magic_items_count);
    assert(NULL == map.true_description);

    treasure_map_finalize(&map);
}


static void
treasure_map_initialize_from_json_object_for_complete_object_test(void)
{
    char const *json_string = "{\n"
                              "  \"struct\": \"treasure_map\",\n"
                              "  \"rev\": 0,\n"
                              "  \"is_false\": false,\n"
                              "  \"treasure\": {\n"
                              "    \"struct\": \"treasure\",\n"
                              "    \"rev\": 0,\n"
                              "    \"type\": null,\n"
                              "    \"coins\": {\n"
                              "      \"struct\": \"coins\",\n"
                              "      \"rev\": 0,\n"
                              "      \"pp\": 1500,\n"
                              "      \"gp\": 0,\n"
                              "      \"ep\": 0,\n"
                              "      \"sp\": 0,\n"
                              "      \"cp\": 0\n"
                              "    },\n"
                              "    \"gems\": [],\n"
                              "    \"jewelry\": [],\n"
                              "    \"maps\": [],\n"
                              "    \"magic_items\": []\n"
                              "  },\n"
                              "  \"true_description\": \"map to monetary treasure of 1500 platinum 7 miles to the south, guarded in a lair\""
                              "}\n";
    struct cJSON *json_object = cJSON_Parse(json_string);
    struct treasure_map map;
    treasure_map_initialize_from_json_object(&map, json_object);
    cJSON_Delete(json_object);

    assert( ! map.is_false);
    assert(1500 == map.treasure.coins.pp);
    assert(0 == map.treasure.coins.gp);
    assert(0 == map.treasure.coins.ep);
    assert(0 == map.treasure.coins.sp);
    assert(0 == map.treasure.coins.cp);
    assert(0 == map.treasure.gems_count);
    assert(0 == map.treasure.jewelry_count);
    assert(0 == map.treasure.maps_count);
    assert(0 == map.treasure.magic_items_count);
    assert(astr_eq("map to monetary treasure of 1500 platinum 7 miles to the south, guarded in a lair",
                  map.true_description));

    treasure_map_finalize(&map);
}


static void
treasure_map_generate_with_fake_min_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_min();
    struct treasure_map map;
    treasure_map_initialize(&map);
    treasure_map_generate(&map, rnd);

    assert(map.is_false);
    assert(coins_is_zero(map.treasure.coins));
    assert(0 == map.treasure.gems_count);
    assert(0 == map.treasure.jewelry_count);
    assert(0 == map.treasure.maps_count);
    assert(0 == map.treasure.magic_items_count);
    assert(astr_eq("false map to treasure of (no treasure) in nearby labyrinth to the north", map.true_description));

    treasure_map_finalize(&map);
    rnd_free(rnd);
}


static void
treasure_map_generate_with_fake_max_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_max();
    struct treasure_map map;
    treasure_map_initialize(&map);
    treasure_map_generate(&map, rnd);

    assert( ! map.is_false);
    assert(coins_is_zero(map.treasure.coins));
    assert(0 == map.treasure.gems_count);
    assert(0 == map.treasure.jewelry_count);
    assert(0 == map.treasure.maps_count);
    assert(17 == map.treasure.magic_items_count);
    assert(astr_eq("map to combined hoard of 17 magic items 500 miles to the northwest, secreted in a town", map.true_description));

    treasure_map_finalize(&map);
    rnd_free(rnd);
}


static void
treasure_map_generate_with_fake_median_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_median();
    struct treasure_map map;
    treasure_map_initialize(&map);
    treasure_map_generate(&map, rnd);

    assert( ! map.is_false);
    assert(0 == map.treasure.coins.cp);
    assert(0 == map.treasure.coins.sp);
    assert(0 == map.treasure.coins.ep);
    assert(0 == map.treasure.coins.gp);
    assert(1500 == map.treasure.coins.pp);
    assert(0 == map.treasure.gems_count);
    assert(0 == map.treasure.jewelry_count);
    assert(0 == map.treasure.maps_count);
    assert(0 == map.treasure.magic_items_count);
    assert(astr_eq("map to monetary treasure of 1500 platinum 7 miles to the south, guarded in a lair", map.true_description));

    treasure_map_finalize(&map);
    rnd_free(rnd);
}


void
treasure_map_test(void)
{
    treasure_map_create_json_object_test();

    treasure_map_initialize_test();
    treasure_map_initialize_from_json_object_for_empty_object_test();
    treasure_map_initialize_from_json_object_for_empty_array_test();
    treasure_map_initialize_from_json_object_for_complete_object_test();

    treasure_map_generate_with_fake_min_test();
    treasure_map_generate_with_fake_max_test();
    treasure_map_generate_with_fake_median_test();
}

