#include <assert.h>
#include <stdlib.h>
#include <background/background.h>
#include <base/base.h>
#include <json/json.h>
#include <treasure/treasure.h>

#include "astr.h"


void
gem_test(void);


static void
gem_create_json_object_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_median();
    struct gem gem;
    gem_initialize(&gem);
    gem_generate(&gem, rnd);
    struct cJSON *json_object = gem_create_json_object(&gem);

    assert(cJSON_IsObject(json_object));

    char *json_string = cJSON_PrintUnformatted(json_object);
    char const *expected = "{"
                           "\"struct\":\"gem\","
                           "\"rev\":0,"
                           "\"size\":\"average\","
                           "\"type\":\"fancy\","
                           "\"kind\":\"jade\","
                           "\"colors\":\"green and white\","
                           "\"value_percent_modifier\":0,"
                           "\"value_rank_modifier\":0"
                           "}";
    assert(astr_eq(expected, json_string));

    free(json_string);
    cJSON_Delete(json_object);

    gem_finalize(&gem);
    rnd_free(rnd);
}


static void
gem_initialize_test(void)
{
    struct gem gem;
    gem_initialize(&gem);

    assert(gem_size_average == gem.size);
    assert(gem_type_unknown == gem.type);
    assert(gem_kind_unknown == gem.kind);
    assert(NULL == gem.colors);
    assert(0 == gem.value_percent_modifier);
    assert(0 == gem.value_rank_modifier);
    assert(NULL == gem.true_description);
    assert(NULL == gem.visible_description);

    gem_finalize(&gem);
}


static void
gem_initialize_from_json_object_for_empty_object_test(void)
{
    char const *json_string = "{}";
    struct cJSON *json_object = cJSON_Parse(json_string);
    struct gem gem;
    gem_initialize_from_json_object(&gem, json_object);
    cJSON_Delete(json_object);

    assert(gem_size_average == gem.size);
    assert(gem_type_unknown == gem.type);
    assert(gem_kind_unknown == gem.kind);
    assert(NULL == gem.colors);
    assert(0 == gem.value_percent_modifier);
    assert(0 == gem.value_rank_modifier);
    assert(NULL == gem.true_description);
    assert(NULL == gem.visible_description);

    gem_finalize(&gem);
}


static void
gem_initialize_from_json_object_for_empty_array_test(void)
{
    char const *json_string = "[]";
    struct cJSON *json_object = cJSON_Parse(json_string);
    struct gem gem;
    gem_initialize_from_json_object(&gem, json_object);
    cJSON_Delete(json_object);

    assert(gem_size_average == gem.size);
    assert(gem_type_unknown == gem.type);
    assert(gem_kind_unknown == gem.kind);
    assert(NULL == gem.colors);
    assert(0 == gem.value_percent_modifier);
    assert(0 == gem.value_rank_modifier);
    assert(NULL == gem.true_description);
    assert(NULL == gem.visible_description);

    gem_finalize(&gem);
}


static void
gem_initialize_from_json_object_for_complete_object_test(void)
{
    char const *json_string = "{\n"
                              "  \"struct\": \"gem\",\n"
                              "  \"rev\": 0,\n"
                              "  \"size\": \"large\",\n"
                              "  \"type\": \"precious\",\n"
                              "  \"kind\": \"moonstone\",\n"
                              "  \"colors\": \"white with pale blue glow\",\n"
                              "  \"value_percent_modifier\": 90,\n"
                              "  \"value_rank_modifier\": 2\n"
                              "}";
    struct cJSON *json_object = cJSON_Parse(json_string);
    struct gem gem;
    gem_initialize_from_json_object(&gem, json_object);

    assert(gem_size_large == gem.size);
    assert(gem_type_precious_stone == gem.type);
    assert(gem_kind_moonstone == gem.kind);
    assert(astr_eq("white with pale blue glow", gem.colors));
    assert(90 == gem.value_percent_modifier);
    assert(2 == gem.value_rank_modifier);
    assert(astr_eq("large moonstone (precious, rank +2, -10%: 9000 gp)", gem.true_description));
    assert(astr_eq("large translucent white with pale blue glow stone", gem.visible_description));

    gem_finalize(&gem);
    cJSON_Delete(json_object);
}


static void
gem_value_in_cp_for_default_test(void)
{
    struct gem gem;
    gem_initialize(&gem);
    gem.type = gem_type_unknown;
    gem.size = gem_size_average;

    assert(0 == gem_value_in_cp(&gem));

    gem_finalize(&gem);
}


static void
gem_value_in_cp_for_each_type_test(void)
{
    struct gem gem;
    gem_initialize(&gem);

    gem.type = gem_type_ornamental_stone;
    assert(gp_to_cp(10) == gem_value_in_cp(&gem));

    gem.type = gem_type_semi_precious_stone;
    assert(gp_to_cp(50) == gem_value_in_cp(&gem));

    gem.type = gem_type_fancy_stone;
    assert(gp_to_cp(100) == gem_value_in_cp(&gem));

    gem.type = gem_type_precious_stone;
    assert(gp_to_cp(500) == gem_value_in_cp(&gem));

    gem.type = gem_type_gem_stone;
    assert(gp_to_cp(1000) == gem_value_in_cp(&gem));

    gem.type = gem_type_jewel_stone;
    assert(gp_to_cp(5000) == gem_value_in_cp(&gem));

    gem_finalize(&gem);
}


static void
gem_value_in_cp_for_each_size_test(void)
{
    struct gem gem;
    gem_initialize(&gem);

    // for ornamental

    gem.type = gem_type_ornamental_stone;
    gem.size = gem_size_very_small;
    assert(gp_to_cp(1) == gem_value_in_cp(&gem));

    gem.size = gem_size_small;
    assert(gp_to_cp(5) == gem_value_in_cp(&gem));

    gem.size = gem_size_average;
    assert(gp_to_cp(10) == gem_value_in_cp(&gem));

    gem.size = gem_size_large;
    assert(gp_to_cp(50) == gem_value_in_cp(&gem));

    gem.size = gem_size_very_large;
    assert(gp_to_cp(100) == gem_value_in_cp(&gem));

    gem.size = gem_size_huge;
    assert(gp_to_cp(500) == gem_value_in_cp(&gem));

    // for jewel

    gem.type = gem_type_jewel_stone;
    gem.size = gem_size_very_small;
    assert(gp_to_cp(500) == gem_value_in_cp(&gem));

    gem.size = gem_size_small;
    assert(gp_to_cp(1000) == gem_value_in_cp(&gem));

    gem.size = gem_size_average;
    assert(gp_to_cp(5000) == gem_value_in_cp(&gem));

    gem.size = gem_size_large;
    assert(gp_to_cp(10000) == gem_value_in_cp(&gem));

    gem.size = gem_size_very_large;
    assert(gp_to_cp(25000) == gem_value_in_cp(&gem));

    gem.size = gem_size_huge;
    assert(gp_to_cp(50000) == gem_value_in_cp(&gem));

    gem_finalize(&gem);
}


static void
gem_value_in_cp_for_low_value_rank_test(void)
{
    struct gem gem;
    gem_initialize(&gem);

    gem.type = gem_type_ornamental_stone;
    gem.size = gem_size_very_small;
    gem.value_rank_modifier = 0;
    assert(gp_to_cp(1) == gem_value_in_cp(&gem));

    gem.value_rank_modifier = -1;
    assert(sp_to_cp(10) == gem_value_in_cp(&gem));

    gem.value_rank_modifier = -2;
    assert(sp_to_cp(5) == gem_value_in_cp(&gem));

    // lowest value

    gem.value_rank_modifier = -3;
    assert(sp_to_cp(1) == gem_value_in_cp(&gem));

    gem.value_rank_modifier = -4;
    assert(sp_to_cp(1) == gem_value_in_cp(&gem));

    gem.value_rank_modifier = -5;
    assert(sp_to_cp(1) == gem_value_in_cp(&gem));

    gem_finalize(&gem);
}


static void
gem_value_in_cp_for_high_value_rank_test(void)
{
    struct gem gem;
    gem_initialize(&gem);

    gem.type = gem_type_jewel_stone;
    gem.size = gem_size_huge;
    gem.value_rank_modifier = 0;
    assert(gp_to_cp(50000) == gem_value_in_cp(&gem));

    gem.value_rank_modifier = 1;
    assert(gp_to_cp(100000) == gem_value_in_cp(&gem));

    gem.value_rank_modifier = 2;
    assert(gp_to_cp(250000) == gem_value_in_cp(&gem));

    gem.value_rank_modifier = 3;
    assert(gp_to_cp(500000) == gem_value_in_cp(&gem));

    // highest value

    gem.value_rank_modifier = 4;
    assert(gp_to_cp(1000000) == gem_value_in_cp(&gem));

    gem.value_rank_modifier = 5;
    assert(gp_to_cp(1000000) == gem_value_in_cp(&gem));

    gem.value_rank_modifier = 6;
    assert(gp_to_cp(1000000) == gem_value_in_cp(&gem));

    gem.value_rank_modifier = 7;
    assert(gp_to_cp(1000000) == gem_value_in_cp(&gem));

    gem_finalize(&gem);
}


static void
gem_value_in_cp_for_value_percent_modifier(void)
{
    struct gem gem;
    gem_initialize(&gem);

    gem.type = gem_type_gem_stone;
    gem.value_percent_modifier = 0;
    assert(gp_to_cp(1000) == gem_value_in_cp(&gem));

    gem.value_percent_modifier = 100;
    assert(gp_to_cp(1000) == gem_value_in_cp(&gem));

    gem.value_percent_modifier = 160;
    assert(gp_to_cp(1600) == gem_value_in_cp(&gem));

    gem.value_percent_modifier = 200;
    assert(gp_to_cp(2000) == gem_value_in_cp(&gem));

    gem.value_percent_modifier = 60;
    assert(gp_to_cp(600) == gem_value_in_cp(&gem));

    gem_finalize(&gem);
}


static void
gem_generate_with_fake_min_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_min();
    struct gem gem;
    gem_initialize(&gem);
    gem_generate(&gem, rnd);

    assert(gem_size_very_small == gem.size);
    assert(gem_type_ornamental_stone == gem.type);
    assert(gem_kind_azurite == gem.kind);
    assert(astr_eq("mottled deep blue", gem.colors));
    assert(0 == gem.value_percent_modifier);
    assert(7 == gem.value_rank_modifier);
    assert(astr_eq("very small azurite (ornamental, rank +7: 5000 gp)", gem.true_description));
    assert(astr_eq("very small opaque mottled deep blue stone", gem.visible_description));

    gem_finalize(&gem);
    rnd_free(rnd);
}


static void
gem_generate_with_fake_max_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_max();
    struct gem gem;
    gem_initialize(&gem);
    gem_generate(&gem, rnd);

    assert(gem_size_huge == gem.size);
    assert(gem_type_jewel_stone == gem.type);
    assert(gem_kind_ruby == gem.kind);
    assert(astr_eq("deep crimson", gem.colors));
    assert(0 == gem.value_percent_modifier);
    assert(-5 == gem.value_rank_modifier);
    assert(astr_eq("huge ruby (jewel, rank -5: 500 gp)", gem.true_description));
    assert(astr_eq("huge transparent deep crimson stone", gem.visible_description));

    gem_finalize(&gem);
    rnd_free(rnd);
}


static void
gem_generate_with_fake_median_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_median();
    struct gem gem;
    gem_initialize(&gem);
    gem_generate(&gem, rnd);

    assert(gem_size_average == gem.size);
    assert(gem_type_fancy_stone == gem.type);
    assert(gem_kind_jade == gem.kind);
    assert(astr_eq("green and white", gem.colors));
    assert(0 == gem.value_percent_modifier);
    assert(0 == gem.value_rank_modifier);
    assert(astr_eq("average jade (fancy: 100 gp)", gem.true_description));
    assert(astr_eq("average translucent green and white stone", gem.visible_description));

    gem_finalize(&gem);
    rnd_free(rnd);
}


static void
gem_generate_for_double_value_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_fixed(1);
    struct gem gem;
    gem_initialize(&gem);
    gem_generate(&gem, rnd);

    assert(gem_size_very_small == gem.size);
    assert(gem_type_ornamental_stone == gem.type);
    assert(gem_kind_banded_agate == gem.kind);
    assert(astr_eq("striped blue", gem.colors));
    assert(200 == gem.value_percent_modifier);
    assert(0 == gem.value_rank_modifier);
    assert(astr_eq("very small banded agate (ornamental, +100%: 2 gp)", gem.true_description));
    assert(astr_eq("very small translucent striped blue stone", gem.visible_description));

    gem_finalize(&gem);
    rnd_free(rnd);
}


static void
gem_generate_for_increased_value_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_fixed(2);
    struct gem gem;
    gem_initialize(&gem);
    gem_generate(&gem, rnd);

    assert(gem_size_very_small == gem.size);
    assert(gem_type_ornamental_stone == gem.type);
    assert(gem_kind_blue_quartz == gem.kind);
    assert(astr_eq("pale blue", gem.colors));
    assert(130 == gem.value_percent_modifier);
    assert(0 == gem.value_rank_modifier);
    assert(astr_eq("very small blue quartz (ornamental, +30%: 1 gp, 60 cp)", gem.true_description));
    assert(astr_eq("very small transparent pale blue stone", gem.visible_description));

    gem_finalize(&gem);
    rnd_free(rnd);
}


static void
gem_generate_for_decreased_value_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_fixed(8);
    struct gem gem;
    gem_initialize(&gem);
    gem_generate(&gem, rnd);

    assert(gem_size_very_small == gem.size);
    assert(gem_type_ornamental_stone == gem.type);
    assert(gem_kind_obsidian == gem.kind);
    assert(astr_eq("black", gem.colors));
    assert(90 == gem.value_percent_modifier);
    assert(0 == gem.value_rank_modifier);
    assert(astr_eq("very small obsidian (ornamental, -10%: 180 cp)", gem.true_description));
    assert(astr_eq("very small opaque black stone", gem.visible_description));

    gem_finalize(&gem);
    rnd_free(rnd);
}


void
gem_test(void)
{
    gem_create_json_object_test();
    gem_initialize_test();
    gem_initialize_from_json_object_for_empty_object_test();
    gem_initialize_from_json_object_for_empty_array_test();
    gem_initialize_from_json_object_for_complete_object_test();
    gem_value_in_cp_for_default_test();
    gem_value_in_cp_for_each_type_test();
    gem_value_in_cp_for_each_size_test();
    gem_value_in_cp_for_low_value_rank_test();
    gem_value_in_cp_for_high_value_rank_test();
    gem_value_in_cp_for_value_percent_modifier();
    gem_generate_with_fake_min_test();
    gem_generate_with_fake_max_test();
    gem_generate_with_fake_median_test();
    gem_generate_for_double_value_test();
    gem_generate_for_increased_value_test();
    gem_generate_for_decreased_value_test();
}
