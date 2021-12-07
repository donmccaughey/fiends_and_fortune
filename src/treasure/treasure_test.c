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
    treasure_generate_magic_items(&treasure, rnd, 3, ANY_MAGIC_ITEM);
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
                               "},"
                               "{"
                                   "\"struct\":\"magic_item\","
                                   "\"rev\":0,"
                                   "\"experience_points\":2000,"
                                   "\"true_description\":\"cloak of protection +2\","
                                   "\"true_details\":[],"
                                   "\"true_value_in_cp\":4000000,"
                                   "\"type\":\"miscellaneous\""
                               "},"
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
treasure_create_json_object_for_type_A_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_min();
    struct treasure treasure;
    treasure_initialize(&treasure);
    struct treasure_type *treasure_type = treasure_type_by_letter('A');
    treasure_type_generate(treasure_type, rnd, &treasure);
    struct cJSON *json_object = treasure_create_json_object(&treasure);

    assert(cJSON_IsObject(json_object));

    char *json_string = cJSON_PrintUnformatted(json_object);
    char const *expected = "{"
                           "\"struct\":\"treasure\","
                           "\"rev\":0,"
                           "\"type\":\"A\","
                           "\"coins\":{"
                               "\"struct\":\"coins\","
                               "\"rev\":0,"
                               "\"pp\":100,"
                               "\"gp\":1000,"
                               "\"ep\":1000,"
                               "\"sp\":1000,"
                               "\"cp\":1000"
                           "},"
                           "\"gems\":["
                               "{\"struct\":\"gem\",\"rev\":0,\"size\":\"very small\",\"type\":\"ornamental\",\"kind\":\"azurite\",\"colors\":\"mottled deep blue\",\"value_percent_modifier\":0,\"value_rank_modifier\":7},"
                               "{\"struct\":\"gem\",\"rev\":0,\"size\":\"very small\",\"type\":\"ornamental\",\"kind\":\"azurite\",\"colors\":\"mottled deep blue\",\"value_percent_modifier\":0,\"value_rank_modifier\":7},"
                               "{\"struct\":\"gem\",\"rev\":0,\"size\":\"very small\",\"type\":\"ornamental\",\"kind\":\"azurite\",\"colors\":\"mottled deep blue\",\"value_percent_modifier\":0,\"value_rank_modifier\":7},"
                               "{\"struct\":\"gem\",\"rev\":0,\"size\":\"very small\",\"type\":\"ornamental\",\"kind\":\"azurite\",\"colors\":\"mottled deep blue\",\"value_percent_modifier\":0,\"value_rank_modifier\":7}"
                           "],"
                           "\"jewelry\":["
                               "{\"struct\":\"jewelry\",\"rev\":0,\"has_gems\":false,\"form\":\"anklet\",\"material\":\"ivory\",\"workmanship_bonus\":12,\"exceptional_stone_bonus\":0,\"value_in_cp\":400000},"
                               "{\"struct\":\"jewelry\",\"rev\":0,\"has_gems\":false,\"form\":\"anklet\",\"material\":\"ivory\",\"workmanship_bonus\":12,\"exceptional_stone_bonus\":0,\"value_in_cp\":400000},"
                               "{\"struct\":\"jewelry\",\"rev\":0,\"has_gems\":false,\"form\":\"anklet\",\"material\":\"ivory\",\"workmanship_bonus\":12,\"exceptional_stone_bonus\":0,\"value_in_cp\":400000}"
                           "],"
                           "\"maps\":["
                               "{\"struct\":\"treasure_map\",\"rev\":0,\"is_false\":true,\"treasure\":{\"struct\":\"treasure\",\"rev\":0,\"type\":null,\"coins\":{\"struct\":\"coins\",\"rev\":0,\"pp\":0,\"gp\":0,\"ep\":0,\"sp\":0,\"cp\":0},\"gems\":[],\"jewelry\":[],\"maps\":[],\"magic_items\":[]},\"true_description\":\"false map to treasure of (no treasure) in nearby labyrinth to the north\"},"
                               "{\"struct\":\"treasure_map\",\"rev\":0,\"is_false\":true,\"treasure\":{\"struct\":\"treasure\",\"rev\":0,\"type\":null,\"coins\":{\"struct\":\"coins\",\"rev\":0,\"pp\":0,\"gp\":0,\"ep\":0,\"sp\":0,\"cp\":0},\"gems\":[],\"jewelry\":[],\"maps\":[],\"magic_items\":[]},\"true_description\":\"false map to treasure of (no treasure) in nearby labyrinth to the north\"},"
                               "{\"struct\":\"treasure_map\",\"rev\":0,\"is_false\":true,\"treasure\":{\"struct\":\"treasure\",\"rev\":0,\"type\":null,\"coins\":{\"struct\":\"coins\",\"rev\":0,\"pp\":0,\"gp\":0,\"ep\":0,\"sp\":0,\"cp\":0},\"gems\":[],\"jewelry\":[],\"maps\":[],\"magic_items\":[]},\"true_description\":\"false map to treasure of (no treasure) in nearby labyrinth to the north\"}"
                           "],"
                           "\"magic_items\":[]"
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
    cJSON_Delete(json_object);

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
    cJSON_Delete(json_object);

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
treasure_initialize_from_json_object_with_gems_test(void)
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
                              "  \"gems\": [\n"
                              "    {\n"
                              "      \"struct\": \"gem\",\n"
                              "      \"rev\": 0,\n"
                              "      \"size\": \"large\",\n"
                              "      \"type\": \"precious\",\n"
                              "      \"kind\": \"moonstone\",\n"
                              "      \"colors\": \"white with pale blue glow\",\n"
                              "      \"value_percent_modifier\": 90,\n"
                              "      \"value_rank_modifier\": 2\n"
                              "    },\n"
                              "    {\n"
                              "      \"struct\": \"gem\",\n"
                              "      \"rev\": 0,\n"
                              "      \"size\": \"very small\",\n"
                              "      \"type\": \"ornamental\",\n"
                              "      \"kind\": \"azurite\",\n"
                              "      \"colors\": \"mottled deep blue\",\n"
                              "      \"value_percent_modifier\": 0,\n"
                              "      \"value_rank_modifier\": 7\n"
                              "    }\n"
                              "  ],\n"
                              "  \"jewelry\": [],\n"
                              "  \"maps\": [],\n"
                              "  \"magic_items\": []\n"
                              "}";
    struct cJSON *json_object = cJSON_Parse(json_string);
    struct treasure treasure;
    treasure_initialize_from_json_object(&treasure, json_object);
    cJSON_Delete(json_object);

    assert(NULL == treasure.type);
    assert(coins_is_zero(treasure.coins));

    assert(NULL != treasure.gems);
    assert(2 == treasure.gems_count);

    assert(gem_size_large == treasure.gems[0].size);
    assert(gem_type_precious_stone == treasure.gems[0].type);
    assert(gem_kind_moonstone == treasure.gems[0].kind);
    assert(str_eq("white with pale blue glow", treasure.gems[0].colors));
    assert(90 == treasure.gems[0].value_percent_modifier);
    assert(2 == treasure.gems[0].value_rank_modifier);
    assert(str_eq("large moonstone (precious, rank +2, -10%: 9000 gp)", treasure.gems[0].true_description));
    assert(str_eq("large translucent white with pale blue glow stone", treasure.gems[0].visible_description));

    assert(gem_size_very_small == treasure.gems[1].size);
    assert(gem_type_ornamental_stone == treasure.gems[1].type);
    assert(gem_kind_azurite == treasure.gems[1].kind);
    assert(str_eq("mottled deep blue", treasure.gems[1].colors));
    assert(0 == treasure.gems[1].value_percent_modifier);
    assert(7 == treasure.gems[1].value_rank_modifier);
    assert(str_eq("very small azurite (ornamental, rank +7: 5000 gp)", treasure.gems[1].true_description));
    assert(str_eq("very small opaque mottled deep blue stone", treasure.gems[1].visible_description));

    assert(NULL == treasure.jewelry);
    assert(0 == treasure.jewelry_count);
    assert(NULL == treasure.maps);
    assert(0 == treasure.maps_count);
    assert(NULL == treasure.magic_items);
    assert(0 == treasure.magic_items_count);

    treasure_finalize(&treasure);
}


static void
treasure_initialize_from_json_object_with_jewelry_test(void)
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
                              "  \"jewelry\": [\n"
                              "    {\n"
                              "      \"struct\": \"jewelry\",\n"
                              "      \"rev\": 0,\n"
                              "      \"has_gems\": true,\n"
                              "      \"form\": \"goblet\",\n"
                              "      \"material\": \"silver with gems\",\n"
                              "      \"workmanship_bonus\": 0,\n"
                              "      \"exceptional_stone_bonus\": 0,\n"
                              "      \"value_in_cp\": 800000\n"
                              "    },\n"
                              "    {\n"
                              "      \"struct\": \"jewelry\",\n"
                              "      \"rev\": 0,\n"
                              "      \"has_gems\": true,\n"
                              "      \"form\": \"necklace\",\n"
                              "      \"material\": \"gold with gems\",\n"
                              "      \"workmanship_bonus\": 0,\n"
                              "      \"exceptional_stone_bonus\": 128,\n"
                              "      \"value_in_cp\": 128400000\n"
                              "    }\n"
                              "  ],\n"
                              "  \"maps\": [],\n"
                              "  \"magic_items\": []\n"
                              "}";
    struct cJSON *json_object = cJSON_Parse(json_string);
    struct treasure treasure;
    treasure_initialize_from_json_object(&treasure, json_object);
    cJSON_Delete(json_object);

    assert(NULL == treasure.type);
    assert(coins_is_zero(treasure.coins));
    assert(NULL == treasure.gems);
    assert(0 == treasure.gems_count);

    assert(NULL != treasure.jewelry);
    assert(2 == treasure.jewelry_count);

    assert(true == treasure.jewelry[0].has_gems);
    assert(jewelry_form_goblet == treasure.jewelry[0].form);
    assert(jewelry_material_silver_with_gems == treasure.jewelry[0].material);
    assert(0 == treasure.jewelry[0].workmanship_bonus);
    assert(0 == treasure.jewelry[0].exceptional_stone_bonus);
    assert(gp_to_cp(4000) == treasure.jewelry[0].value_in_cp);
    assert(str_eq("silver goblet with gems (4000 gp)", treasure.jewelry[0].true_description));

    assert(true == treasure.jewelry[1].has_gems);
    assert(jewelry_form_necklace == treasure.jewelry[1].form);
    assert(jewelry_material_gold_with_gems == treasure.jewelry[1].material);
    assert(0 == treasure.jewelry[1].workmanship_bonus);
    assert(128 == treasure.jewelry[1].exceptional_stone_bonus);
    assert(gp_to_cp(642000) == treasure.jewelry[1].value_in_cp);
    assert(str_eq("gold necklace with gems (exceptional stone +128: 642000 gp)", treasure.jewelry[1].true_description));

    assert(NULL == treasure.maps);
    assert(0 == treasure.maps_count);
    assert(NULL == treasure.magic_items);
    assert(0 == treasure.magic_items_count);

    treasure_finalize(&treasure);
}


static void
treasure_initialize_from_json_object_with_maps_test(void)
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
                              "  \"maps\": [\n"
                              "    {\n"
                              "      \"struct\": \"treasure_map\",\n"
                              "      \"rev\": 0,\n"
                              "      \"is_false\": false,\n"
                              "      \"treasure\": {\n"
                              "        \"struct\": \"treasure\",\n"
                              "        \"rev\": 0,\n"
                              "        \"type\": null,\n"
                              "        \"coins\": {\n"
                              "          \"struct\": \"coins\",\n"
                              "          \"rev\": 0,\n"
                              "          \"pp\": 1500,\n"
                              "          \"gp\": 0,\n"
                              "          \"ep\": 0,\n"
                              "          \"sp\": 0,\n"
                              "          \"cp\": 0\n"
                              "        },\n"
                              "        \"gems\": [],\n"
                              "        \"jewelry\": [],\n"
                              "        \"maps\": [],\n"
                              "        \"magic_items\": []\n"
                              "      },\n"
                              "      \"true_description\": \"map to monetary treasure of 1500 platinum 7 miles to the south, guarded in a lair\""
                              "    },\n"
                              "    {\n"
                              "      \"struct\": \"treasure_map\",\n"
                              "      \"rev\": 0,\n"
                              "      \"is_false\": true,\n"
                              "      \"treasure\": {\n"
                              "        \"struct\": \"treasure\",\n"
                              "        \"rev\": 0,\n"
                              "        \"type\": null,\n"
                              "        \"coins\": {\n"
                              "          \"struct\": \"coins\",\n"
                              "          \"rev\": 0,\n"
                              "          \"pp\": 0,\n"
                              "          \"gp\": 0,\n"
                              "          \"ep\": 0,\n"
                              "          \"sp\": 0,\n"
                              "          \"cp\": 0\n"
                              "        },\n"
                              "        \"gems\": [],\n"
                              "        \"jewelry\": [],\n"
                              "        \"maps\": [],\n"
                              "        \"magic_items\": []\n"
                              "      },\n"
                              "      \"true_description\": \"false map to treasure of (no treasure) in nearby labyrinth to the north\""
                              "    }\n"
                              "  ],\n"
                              "  \"magic_items\": []\n"
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

    assert(NULL != treasure.maps);
    assert(2 == treasure.maps_count);

    assert( ! treasure.maps[0].is_false);
    assert(1500 == treasure.maps[0].treasure.coins.pp);
    assert(0 == treasure.maps[0].treasure.coins.gp);
    assert(0 == treasure.maps[0].treasure.coins.ep);
    assert(0 == treasure.maps[0].treasure.coins.sp);
    assert(0 == treasure.maps[0].treasure.coins.cp);
    assert(0 == treasure.maps[0].treasure.gems_count);
    assert(0 == treasure.maps[0].treasure.jewelry_count);
    assert(0 == treasure.maps[0].treasure.maps_count);
    assert(0 == treasure.maps[0].treasure.magic_items_count);
    assert(str_eq("map to monetary treasure of 1500 platinum 7 miles to the south, guarded in a lair",
                  treasure.maps[0].true_description));

    assert(treasure.maps[1].is_false);
    assert(coins_is_zero(treasure.maps[1].treasure.coins));
    assert(0 == treasure.maps[1].treasure.gems_count);
    assert(0 == treasure.maps[1].treasure.jewelry_count);
    assert(0 == treasure.maps[1].treasure.maps_count);
    assert(0 == treasure.maps[1].treasure.magic_items_count);
    assert(str_eq("false map to treasure of (no treasure) in nearby labyrinth to the north",
                  treasure.maps[1].true_description));

    assert(NULL == treasure.magic_items);
    assert(0 == treasure.magic_items_count);

    treasure_finalize(&treasure);
}


static void
treasure_initialize_from_json_object_with_magic_items_test(void)
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
                              "    },\n"
                              "    {\n"
                              "      \"struct\": \"magic_item\",\n"
                              "      \"rev\": 0,\n"
                              "      \"experience_points\": 5000,\n"
                              "      \"true_description\": \"broadsword of life stealing (unusual)\",\n"
                              "      \"true_details\": [\n"
                              "        \"intelligence 12 (semi-empathy)\",\n"
                              "        \"neutral good alignment\",\n"
                              "        \"detect invisible objects in a 1\\\" radius\",\n"
                              "        \"personality strength 17 (ego 5)\"\n"
                              "      ],\n"
                              "      \"true_value_in_cp\": 5000000,\n"
                              "      \"type\": \"sword\"\n"
                              "    }\n"
                              "  ]\n"
                              "}";
    struct cJSON *json_object = cJSON_Parse(json_string);
    struct treasure treasure;
    treasure_initialize_from_json_object(&treasure, json_object);
    cJSON_Delete(json_object);

    assert(NULL == treasure.type);
    assert(coins_is_zero(treasure.coins));
    assert(NULL == treasure.gems);
    assert(0 == treasure.gems_count);
    assert(NULL == treasure.jewelry);
    assert(0 == treasure.jewelry_count);
    assert(NULL == treasure.maps);
    assert(0 == treasure.maps_count);
    assert(NULL != treasure.magic_items);
    assert(2 == treasure.magic_items_count);

    assert(2000 == treasure.magic_items[0].experience_points);
    assert(str_eq("cloak of protection +2", treasure.magic_items[0].true_description));
    assert(NULL == treasure.magic_items[0].true_details);
    assert(4000000 == treasure.magic_items[0].true_value_in_cp);
    assert(magic_item_type_misc == treasure.magic_items[0].type);

    assert(5000 == treasure.magic_items[1].experience_points);
    assert(str_eq("broadsword of life stealing (unusual)", treasure.magic_items[1].true_description));
    assert(treasure.magic_items[1].true_details);
    assert(str_eq("intelligence 12 (semi-empathy)", treasure.magic_items[1].true_details[0]));
    assert(str_eq("neutral good alignment", treasure.magic_items[1].true_details[1]));
    assert(str_eq("detect invisible objects in a 1\" radius", treasure.magic_items[1].true_details[2]));
    assert(str_eq("personality strength 17 (ego 5)", treasure.magic_items[1].true_details[3]));
    assert(NULL == treasure.magic_items[1].true_details[4]);
    assert(5000000 == treasure.magic_items[1].true_value_in_cp);
    assert(magic_item_type_sword == treasure.magic_items[1].type);

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
    treasure_create_json_object_for_type_A_test();

    treasure_initialize_test();
    treasure_initialize_from_json_object_for_empty_object_test();
    treasure_initialize_from_json_object_for_empty_array_test();

    treasure_initialize_from_json_object_with_gems_test();
    treasure_initialize_from_json_object_with_jewelry_test();
    treasure_initialize_from_json_object_with_maps_test();
    treasure_initialize_from_json_object_with_magic_items_test();

    treasure_generate_magic_items_test();
}
