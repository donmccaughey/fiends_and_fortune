#include <assert.h>
#include <background/background.h>
#include <base/base.h>
#include <json/json.h>
#include <treasure/treasure.h>

#include "astr.h"


void
magic_item_test(void);


static void
magic_item_create_json_object_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_median();
    struct magic_item magic_item;
    magic_item_initialize(&magic_item);
    magic_item_generate(&magic_item, rnd, ANY_MAGIC_ITEM);
    struct cJSON *json_object = magic_item_create_json_object(&magic_item);

    assert(cJSON_IsObject(json_object));

    char *json_string = cJSON_PrintUnformatted(json_object);
    char const *expected = "{"
                           "\"struct\":\"magic_item\","
                           "\"rev\":0,"
                           "\"experience_points\":2000,"
                           "\"true_description\":\"cloak of protection +2\","
                           "\"true_details\":[],"
                           "\"true_value_in_cp\":4000000,"
                           "\"type\":\"miscellaneous\""
                           "}";
    assert(astr_eq(expected, json_string));

    free(json_string);
    cJSON_Delete(json_object);

    magic_item_finalize(&magic_item);
    rnd_free(rnd);
}


static void
magic_item_create_json_object_with_true_details_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_fixed(82);
    struct magic_item magic_item;
    magic_item_initialize(&magic_item);
    magic_item_generate(&magic_item, rnd, SWORD);
    struct cJSON *json_object = magic_item_create_json_object(&magic_item);

    assert(cJSON_IsObject(json_object));

    char *json_string = cJSON_PrintUnformatted(json_object);
    char const *expected = "{"
                           "\"struct\":\"magic_item\","
                           "\"rev\":0,"
                           "\"experience_points\":5000,"
                           "\"true_description\":\"broadsword of life stealing (unusual)\","
                           "\"true_details\":["
                           "\"intelligence 12 (semi-empathy)\","
                           "\"neutral good alignment\","
                           "\"detect invisible objects in a 1\\\" radius\","
                           "\"personality strength 17 (ego 5)\""
                           "],"
                           "\"true_value_in_cp\":5000000,"
                           "\"type\":\"sword\""
                           "}";
    assert(astr_eq(expected, json_string));

    free(json_string);
    cJSON_Delete(json_object);

    magic_item_finalize(&magic_item);
    rnd_free(rnd);
}


static void
magic_item_initialize_test(void)
{
    struct magic_item magic_item;
    magic_item_initialize(&magic_item);

    assert(0 == magic_item.experience_points);
    assert(NULL == magic_item.true_description);
    assert(NULL == magic_item.true_details);
    assert(0 == magic_item.true_value_in_cp);
    assert(magic_item_type_unknown == magic_item.type);

    magic_item_finalize(&magic_item);
}


static void
magic_item_initialize_from_json_object_for_empty_object_test(void)
{
    char const *json_string = "{}";
    struct cJSON *json_object = cJSON_Parse(json_string);
    struct magic_item magic_item;
    magic_item_initialize_from_json_object(&magic_item, json_object);
    cJSON_Delete(json_object);

    assert(0 == magic_item.experience_points);
    assert(NULL == magic_item.true_description);
    assert(NULL == magic_item.true_details);
    assert(0 == magic_item.true_value_in_cp);
    assert(magic_item_type_unknown == magic_item.type);

    magic_item_finalize(&magic_item);
}


static void
magic_item_initialize_from_json_object_for_empty_array_test(void)
{
    char const *json_string = "[]";
    struct cJSON *json_object = cJSON_Parse(json_string);
    struct magic_item magic_item;
    magic_item_initialize_from_json_object(&magic_item, json_object);
    cJSON_Delete(json_object);

    assert(0 == magic_item.experience_points);
    assert(NULL == magic_item.true_description);
    assert(NULL == magic_item.true_details);
    assert(0 == magic_item.true_value_in_cp);
    assert(magic_item_type_unknown == magic_item.type);

    magic_item_finalize(&magic_item);
}


static void
magic_item_initialize_from_json_object_for_complete_object_test(void)
{
    char const *json_string = "{\n"
                              "  \"struct\": \"magic_item\",\n"
                              "  \"rev\": 0,\n"
                              "  \"experience_points\": 2000,\n"
                              "  \"true_description\": \"cloak of protection +2\",\n"
                              "  \"true_details\": [],\n"
                              "  \"true_value_in_cp\": 4000000,\n"
                              "  \"type\": \"miscellaneous\"\n"
                              "}";
    struct cJSON *json_object = cJSON_Parse(json_string);
    struct magic_item magic_item;
    magic_item_initialize_from_json_object(&magic_item, json_object);
    cJSON_Delete(json_object);

    assert(2000 == magic_item.experience_points);
    assert(astr_eq("cloak of protection +2", magic_item.true_description));
    assert(NULL == magic_item.true_details);
    assert(4000000 == magic_item.true_value_in_cp);
    assert(magic_item_type_misc == magic_item.type);

    magic_item_finalize(&magic_item);
}


static void
magic_item_initialize_from_json_object_for_complete_object_with_true_details_test(void)
{
    char const *json_string = "{\n"
                              "  \"struct\": \"magic_item\",\n"
                              "  \"rev\": 0,\n"
                              "  \"experience_points\": 5000,\n"
                              "  \"true_description\": \"broadsword of life stealing (unusual)\",\n"
                              "  \"true_details\": [\n"
                              "    \"intelligence 12 (semi-empathy)\",\n"
                              "    \"neutral good alignment\",\n"
                              "    \"detect invisible objects in a 1\\\" radius\",\n"
                              "    \"personality strength 17 (ego 5)\"\n"
                              "  ],\n"
                              "  \"true_value_in_cp\": 5000000,\n"
                              "  \"type\": \"sword\"\n"
                              "}";
    struct cJSON *json_object = cJSON_Parse(json_string);
    struct magic_item magic_item;
    magic_item_initialize_from_json_object(&magic_item, json_object);
    cJSON_Delete(json_object);

    assert(5000 == magic_item.experience_points);
    assert(astr_eq("broadsword of life stealing (unusual)", magic_item.true_description));
    assert(magic_item.true_details);
    assert(astr_eq("intelligence 12 (semi-empathy)", magic_item.true_details[0]));
    assert(astr_eq("neutral good alignment", magic_item.true_details[1]));
    assert(astr_eq("detect invisible objects in a 1\" radius", magic_item.true_details[2]));
    assert(astr_eq("personality strength 17 (ego 5)", magic_item.true_details[3]));
    assert(NULL == magic_item.true_details[4]);
    assert(5000000 == magic_item.true_value_in_cp);
    assert(magic_item_type_sword == magic_item.type);

    magic_item_finalize(&magic_item);
}


static void
magic_item_generate_for_any_with_fake_min_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_min();
    struct magic_item magic_item;
    magic_item_initialize(&magic_item);
    magic_item_generate(&magic_item, rnd, ANY_MAGIC_ITEM);

    assert(250 == magic_item.experience_points);
    assert(astr_eq("animal control potion", magic_item.true_description));
    assert(NULL == magic_item.true_details);
    assert(gp_to_cp(400) == magic_item.true_value_in_cp);
    assert(magic_item_type_potion == magic_item.type);

    magic_item_finalize(&magic_item);
    rnd_free(rnd);
}


static void
magic_item_generate_for_any_with_fake_max_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_max();
    struct magic_item magic_item;
    magic_item_initialize(&magic_item);
    magic_item_generate(&magic_item, rnd, ANY_MAGIC_ITEM);

    assert(1500 == magic_item.experience_points);
    assert(astr_eq("trident (military fork) +3", magic_item.true_description));
    assert(NULL == magic_item.true_details);
    assert(gp_to_cp(12500) == magic_item.true_value_in_cp);
    assert(magic_item_type_misc_weapon == magic_item.type);

    magic_item_finalize(&magic_item);
    rnd_free(rnd);
}


static void
magic_item_generate_for_any_with_fake_median_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_median();
    struct magic_item magic_item;
    magic_item_initialize(&magic_item);
    magic_item_generate(&magic_item, rnd, ANY_MAGIC_ITEM);

    assert(2000 == magic_item.experience_points);
    assert(astr_eq("cloak of protection +2", magic_item.true_description));
    assert(NULL == magic_item.true_details);
    assert(gp_to_cp(20000) == magic_item.true_value_in_cp);
    assert(magic_item_type_misc == magic_item.type);

    magic_item_finalize(&magic_item);
    rnd_free(rnd);
}


static void
magic_item_generate_for_sword_with_fake_min_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_min();
    struct magic_item magic_item;
    magic_item_initialize(&magic_item);
    magic_item_generate(&magic_item, rnd, SWORD);

    assert(400 == magic_item.experience_points);
    assert(astr_eq("longsword +1", magic_item.true_description));
    assert(NULL == magic_item.true_details);
    assert(gp_to_cp(2000) == magic_item.true_value_in_cp);
    assert(magic_item_type_sword == magic_item.type);

    magic_item_finalize(&magic_item);
    rnd_free(rnd);
}


static void
magic_item_generate_for_sword_with_fake_max_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_max();
    struct magic_item magic_item;
    magic_item_initialize(&magic_item);
    magic_item_generate(&magic_item, rnd, SWORD);

    assert(900 == magic_item.experience_points);
    assert(astr_eq("two-handed sword, cursed berserking (unusual)", magic_item.true_description));
    assert(NULL != magic_item.true_details);

    assert(astr_eq("intelligence 17 (speech and telepathy)", magic_item.true_details[0]));
    assert(astr_eq("neutral good alignment", magic_item.true_details[1]));
    assert(astr_eq("X-ray vision, 4\" range - 8 times/day, 1 turn per use", magic_item.true_details[2]));
    assert(astr_eq("special purpose: slay non-human monsters", magic_item.true_details[3]));
    assert(astr_eq("special purpose power: +2 on all saving throws, -1 on each die of damage sustained", magic_item.true_details[4]));
    assert(astr_eq("speaks Xorn, Troll, Titan, Sylph, Sprite, Shedu, Satyr, Salamander, Pixie, Orcish, neutral good", magic_item.true_details[5]));
    assert(astr_eq("personality strength 45 (ego 28)", magic_item.true_details[6]));
    assert(NULL == magic_item.true_details[7]);

    assert(0 == magic_item.true_value_in_cp);
    assert(magic_item_type_sword == magic_item.type);

    magic_item_finalize(&magic_item);
    rnd_free(rnd);
}


static void
magic_item_generate_for_unusual_sword_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_fixed(82);
    struct magic_item magic_item;
    magic_item_initialize(&magic_item);
    magic_item_generate(&magic_item, rnd, SWORD);

    assert(5000 == magic_item.experience_points);
    assert(astr_eq("broadsword of life stealing (unusual)", magic_item.true_description));
    assert(NULL != magic_item.true_details);

    assert(astr_eq("intelligence 12 (semi-empathy)", magic_item.true_details[0]));
    assert(astr_eq("neutral good alignment", magic_item.true_details[1]));
    assert(astr_eq("detect invisible objects in a 1\" radius", magic_item.true_details[2]));
    assert(astr_eq("personality strength 17 (ego 5)", magic_item.true_details[3]));
    assert(NULL == magic_item.true_details[4]);

    assert(gp_to_cp(25000) == magic_item.true_value_in_cp);
    assert(magic_item_type_sword == magic_item.type);

    magic_item_finalize(&magic_item);
    rnd_free(rnd);
}


void
magic_item_test(void)
{
    magic_item_create_json_object_test();
    magic_item_create_json_object_with_true_details_test();

    magic_item_initialize_test();
    magic_item_initialize_from_json_object_for_empty_object_test();
    magic_item_initialize_from_json_object_for_empty_array_test();
    magic_item_initialize_from_json_object_for_complete_object_test();
    magic_item_initialize_from_json_object_for_complete_object_with_true_details_test();

    magic_item_generate_for_any_with_fake_min_test();
    magic_item_generate_for_any_with_fake_max_test();
    magic_item_generate_for_any_with_fake_median_test();

    magic_item_generate_for_sword_with_fake_min_test();
    magic_item_generate_for_sword_with_fake_max_test();
    magic_item_generate_for_unusual_sword_test();
}
