#include "gem.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <base/base.h>
#include <background/background.h>
#include <json/json.h>
#include <mechanics/mechanics.h>

#include "astr.h"
#include "coins.h"


static char const *const gem_opacity_names[] = {
    "transparent",
    "translucent",
    "opaque",
};

static char const *const gem_size_names[] = {
    "very small",
    "small",
    "average",
    "large",
    "very large",
    "huge"
};
static size_t gem_size_names_count = ARRAY_COUNT(gem_size_names);

static char const *const gem_type_names[] = {
    "unknown",
    "ornamental",
    "semi-precious",
    "fancy",
    "precious",
    "gem",
    "jewel"
};
static size_t gem_type_names_count = ARRAY_COUNT(gem_type_names);

static char const *const gem_kind_names[] = {
    "unknown",
    /* ornamental */
    "azurite",
    "banded agate",
    "blue quartz",
    "eye_agate",
    "hematite",
    "lapis lazuli",
    "malachite",
    "moss agate",
    "obsidian",
    "rhodochrosite",
    "tiger eye",
    "turquoise",
    /* semi-precious */
    "bloodstone",
    "carnelian",
    "chalcedony",
    "chrysoprase",
    "citrine",
    "jasper",
    "moonstone",
    "onyx",
    "rock crystal",
    "sardonyx",
    "smoky quartz",
    "star rose quartz",
    "zircon",
    /* fancy */
    "amber",
    "alexandrite",
    "amethyst",
    "chrysoberyl",
    "coral",
    "jade",
    "jet",
    "tourmaline",
    /* fancy or precious */
    "garnet",
    "pearl",
    "spinel",
    /* precious */
    "aquamarine",
    "peridot",
    "topaz",
    /* gem */
    "black opal",
    "emerald",
    "fire opal",
    "opal",
    "oriental amethyst",
    "oriental topaz",
    "sapphire",
    "star ruby",
    "star sapphire",
    /* jewel */
    "black sapphire",
    "diamond",
    "jacinth",
    "oriental emerald",
    "ruby",
};
static size_t gem_kind_names_count = ARRAY_COUNT(gem_kind_names);

static int const gem_values_by_rank_in_cp[] = {
    0,
    sp_to_cp(1),
    sp_to_cp(5),
    sp_to_cp(10),
    gp_to_cp(1),
    gp_to_cp(5),
    gp_to_cp(10),   // ornamental
    gp_to_cp(50),   // semi-precious
    gp_to_cp(100),  // fancy
    gp_to_cp(500),  // precious
    gp_to_cp(1000), // gem
    gp_to_cp(5000), // jewel
    gp_to_cp(10000),
    gp_to_cp(25000),
    gp_to_cp(50000),
    gp_to_cp(100000),
    gp_to_cp(250000),
    gp_to_cp(500000),
    gp_to_cp(1000000),
};

static size_t const gem_values_by_rank_in_cp_count = ARRAY_COUNT(gem_values_by_rank_in_cp);

// colors constants in sorted ASCII order
static char const *const colors_apple_green = "apple green";
static char const *const colors_bands_of_black_and_white = "bands of black and white";
static char const *const colors_bands_of_red_and_white = "bands of red and white";
static char const *const colors_black = "black";
static char const *const colors_blue = "blue";
static char const *const colors_blue_pale = "blue pale";
static char const *const colors_brown = "brown";
static char const *const colors_brown_pale = "brown pale";
static char const *const colors_brown_green = "brown-green";
static char const *const colors_circles_of_blue = "circles of blue";
static char const *const colors_circles_of_brown = "circles of brown";
static char const *const colors_circles_of_gray = "circles of gray";
static char const *const colors_circles_of_green = "circles of green";
static char const *const colors_circles_of_white = "circles of white";
static char const *const colors_clear = "clear";
static char const *const colors_clear_blue = "clear blue";
static char const *const colors_clear_blue_with_white_star_center = "clear blue with white 'star' center";
static char const *const colors_clear_blue_white = "clear blue-white";
static char const *const colors_clear_bright_green = "clear bright green";
static char const *const colors_clear_pale_blue_green = "clear pale blue-green";
static char const *const colors_clear_red = "clear red";
static char const *const colors_clear_red_with_white_star_center = "clear red with white 'star' center";
static char const *const colors_clear_white = "clear white";
static char const *const colors_crimson = "crimson";
static char const *const colors_dark_gray_with_red_flecks = "dark gray with red flecks";
static char const *const colors_dark_green = "dark green";
static char const *const colors_dark_green_with_black_mottling_and_golden_flecks = "dark green with black mottling and golden flecks";
static char const *const colors_deep_black = "deep black";
static char const *const colors_deep_bright_green = "deep bright green";
static char const *const colors_deep_crimson = "deep crimson";
static char const *const colors_deep_crimson_with_white_star_center = "deep crimson with white 'star' center";
static char const *const colors_deep_green = "deep green";
static char const *const colors_deep_purple = "deep purple";
static char const *const colors_emerald_green = "emerald green";
static char const *const colors_fiery_orange = "fiery orange";
static char const *const colors_fiery_red = "fiery red";
static char const *const colors_fiery_yellow = "fiery yellow";
static char const *const colors_golden_yellow = "golden yellow";
static char const *const colors_gray = "gray";
static char const *const colors_gray_black = "gray-black";
static char const *const colors_green = "green";
static char const *const colors_green_and_white = "green and white";
static char const *const colors_green_pale = "green pale";
static char const *const colors_light_and_dark_blue_with_yellow_flecks = "light and dark blue with yellow flecks";
static char const *const colors_light_blue = "light blue";
static char const *const colors_light_blue_green = "light blue-green";
static char const *const colors_light_gray = "light gray";
static char const *const colors_light_green = "light green";
static char const *const colors_light_pink = "light pink";
static char const *const colors_light_yellow = "light yellow";
static char const *const colors_lustrous_black_with_glowing_highlights = "lustrous black with glowing highlights";
static char const *const colors_lustrous_white = "lustrous white";
static char const *const colors_medium_blue = "medium blue";
static char const *const colors_medium_blue_with_white_star_center = "medium blue with white 'star' center";
static char const *const colors_mottled_deep_blue = "mottled deep blue";
static char const *const colors_orange = "orange";
static char const *const colors_pale_blue = "pale blue";
static char const *const colors_pale_blue_with_green_and_golden_mottling = "pale blue with green and golden mottling";
static char const *const colors_pale_blue_green = "pale blue-green";
static char const *const colors_pale_yellow_brown = "pale yellow brown";
static char const *const colors_pink = "pink";
static char const *const colors_pinkdeep_greenish = "pinkdeep greenish";
static char const *const colors_pinkish = "pinkish";
static char const *const colors_red = "red";
static char const *const colors_red_brown = "red-brown";
static char const *const colors_reddish_brown = "reddish brown";
static char const *const colors_reddish_pale = "reddish pale";
static char const *const colors_rich_brown_with_golden_center_under_hue = "rich brown with golden center under-hue";
static char const *const colors_rich_gold = "rich gold";
static char const *const colors_rich_olive_green = "rich olive green";
static char const *const colors_rich_purple = "rich purple";
static char const *const colors_rosy_stone_with_white_star_center = "rosy stone with white 'star' center";
static char const *const colors_striated_light_and_dark_green = "striated light and dark green";
static char const *const colors_striped_blue = "striped blue";
static char const *const colors_striped_brown = "striped brown";
static char const *const colors_striped_reddish = "striped reddish";
static char const *const colors_striped_white = "striped white";
static char const *const colors_very_deep_blue = "very deep blue";
static char const *const colors_violet = "violet";
static char const *const colors_watery_gold = "watery gold";
static char const *const colors_white = "white";
static char const *const colors_white_with_pale_blue_glow = "white with pale blue glow";
static char const *const colors_yellow_green = "yellow green";
static char const *const colors_yellow_white_with_grayish_markings = "yellow-white with grayish markings";
static char const *const colors_yellow_white_with_greenish_markings = "yellow-white with greenish markings";
static char const *const colors_yellowish = "yellowish";

// colors in sorted ASCII order
static char const *const colors[] = {
    colors_apple_green,
    colors_bands_of_black_and_white,
    colors_bands_of_red_and_white,
    colors_black,
    colors_blue,
    colors_blue_pale,
    colors_brown,
    colors_brown_pale,
    colors_brown_green,
    colors_circles_of_blue,
    colors_circles_of_brown,
    colors_circles_of_gray,
    colors_circles_of_green,
    colors_circles_of_white,
    colors_clear,
    colors_clear_blue,
    colors_clear_blue_with_white_star_center,
    colors_clear_blue_white,
    colors_clear_bright_green,
    colors_clear_pale_blue_green,
    colors_clear_red,
    colors_clear_red_with_white_star_center,
    colors_clear_white,
    colors_crimson,
    colors_dark_gray_with_red_flecks,
    colors_dark_green,
    colors_dark_green_with_black_mottling_and_golden_flecks,
    colors_deep_black,
    colors_deep_bright_green,
    colors_deep_crimson,
    colors_deep_crimson_with_white_star_center,
    colors_deep_green,
    colors_deep_purple,
    colors_emerald_green,
    colors_fiery_orange,
    colors_fiery_red,
    colors_fiery_yellow,
    colors_golden_yellow,
    colors_gray,
    colors_gray_black,
    colors_green,
    colors_green_and_white,
    colors_green_pale,
    colors_light_and_dark_blue_with_yellow_flecks,
    colors_light_blue,
    colors_light_blue_green,
    colors_light_gray,
    colors_light_green,
    colors_light_pink,
    colors_light_yellow,
    colors_lustrous_black_with_glowing_highlights,
    colors_lustrous_white,
    colors_medium_blue,
    colors_medium_blue_with_white_star_center,
    colors_mottled_deep_blue,
    colors_orange,
    colors_pale_blue,
    colors_pale_blue_with_green_and_golden_mottling,
    colors_pale_blue_green,
    colors_pale_yellow_brown,
    colors_pink,
    colors_pinkdeep_greenish,
    colors_pinkish,
    colors_red,
    colors_red_brown,
    colors_reddish_brown,
    colors_reddish_pale,
    colors_rich_brown_with_golden_center_under_hue,
    colors_rich_gold,
    colors_rich_olive_green,
    colors_rich_purple,
    colors_rosy_stone_with_white_star_center,
    colors_striated_light_and_dark_green,
    colors_striped_blue,
    colors_striped_brown,
    colors_striped_reddish,
    colors_striped_white,
    colors_very_deep_blue,
    colors_violet,
    colors_watery_gold,
    colors_white,
    colors_white_with_pale_blue_glow,
    colors_yellow_green,
    colors_yellow_white_with_grayish_markings,
    colors_yellow_white_with_greenish_markings,
    colors_yellowish,
};
static size_t colors_count = ARRAY_COUNT(colors);


static char *
gem_alloc_true_description_modifiers(struct gem *gem);

static char *
gem_alloc_true_description_prefix(struct gem *gem);

static int
gem_kind_for_name(char const *name, int default_value);

static char const *
gem_kind_name(struct gem *gem);

static enum gem_opacity
gem_opacity(struct gem *gem);

static char const *
gem_opacity_name(struct gem *gem);

static int
gem_size_for_name(char const *name, int default_value);

static char const *
gem_size_name(struct gem *gem);

static int
gem_type_for_name(char const *name, int default_value);

static char const *
gem_type_name(struct gem *gem);


static int
compare_colors(void const *first, void const *second)
{
    char *const *first_color = first;
    char *const *second_color = second;
    return strcmp(*first_color, *second_color);
}


static char const *
select_random_string(struct rnd *rnd, char const **strings, int count)
{
    int index = rnd_next_uniform_value(rnd, count);
    return strings[index];
}


static char const *
gem_generate_colors(struct gem *gem, struct rnd *rnd)
{
    switch (gem->kind) {
        case gem_kind_unknown:
            return colors_gray;
        /* ornamental */
        case gem_kind_azurite: {
            return colors_mottled_deep_blue;
        }
        case gem_kind_banded_agate:
            // striped brown and blue and white and reddish
            return select_random_string(rnd,
                                        (char const *[]){
                                                colors_striped_brown,
                                                colors_striped_blue,
                                                colors_striped_white,
                                                colors_striped_reddish,
                                        },
                                        4);
        case gem_kind_blue_quartz:
            return colors_pale_blue;
        case gem_kind_eye_agate:
            // circles of gray, white, brown, blue and/or green
            return select_random_string(rnd,
                                        (char const *[]){
                                                colors_circles_of_gray,
                                                colors_circles_of_white,
                                                colors_circles_of_brown,
                                                colors_circles_of_blue,
                                                colors_circles_of_green
                                        },
                                        5);
        case gem_kind_hematite:
            return colors_gray_black;
        case gem_kind_lapis_lazuli:
            return colors_light_and_dark_blue_with_yellow_flecks;
        case gem_kind_malachite:
            return colors_striated_light_and_dark_green;
        case gem_kind_moss_agate:
            // pink or yellow-white with grayish or greenish 'moss markings'
            return select_random_string(rnd,
                                        (char const *[]){
                                                colors_pink,
                                                colors_yellow_white_with_grayish_markings,
                                                colors_yellow_white_with_greenish_markings
                                        },
                                        3);
        case gem_kind_obsidian:
            return colors_black;
        case gem_kind_rhodochrosite:
            return colors_light_pink;
        case gem_kind_tiger_eye:
            return colors_rich_brown_with_golden_center_under_hue;
        case gem_kind_turquoise:
            return colors_light_blue_green;
        /* semi-precious */
        case gem_kind_bloodstone:
            return colors_dark_gray_with_red_flecks;
        case gem_kind_carnelian:
            // orange to reddish brown
            return select_random_string(rnd,
                                        (char const *[]){ colors_orange, colors_reddish_brown },
                                        2);
        case gem_kind_chalcedony:
            return colors_white;
        case gem_kind_chrysoprase:
            // apple green to emerald green
            return select_random_string(rnd,
                                        (char const *[]){ colors_apple_green, colors_emerald_green },
                                        2);
        case gem_kind_citrine:
            return colors_pale_yellow_brown;
        case gem_kind_jasper:
            // blue, black to brown
            return select_random_string(rnd,
                                        (char const *[]){ colors_blue, colors_black, colors_brown },
                                        3);
        case gem_kind_moonstone:
            return colors_white_with_pale_blue_glow;
        case gem_kind_onyx:
            // bands of black and white or pure black or white
            return select_random_string(rnd,
                                        (char const *[]){
                                                colors_bands_of_black_and_white,
                                                colors_black,
                                                colors_white
                                        },
                                        3);
        case gem_kind_rock_crystal:
            return colors_clear;
        case gem_kind_sardonyx:
            // bands of sard (red) and onyx (white) or sard
            return select_random_string(rnd,
                                        (char const *[]){ colors_bands_of_red_and_white, colors_red },
                                        2);
        case gem_kind_smoky_quartz:
            // gray, yellow, or blue (Cairngorm), all light
            return select_random_string(rnd,
                                        (char const *[]){
                                                colors_light_gray,
                                                colors_light_yellow,
                                                colors_light_blue
                                        },
                                        3);
        case gem_kind_star_rose_quartz:
            return colors_rosy_stone_with_white_star_center;
        case gem_kind_zircon:
            return colors_clear_pale_blue_green;
        /* fancy */
        case gem_kind_amber:
            // watery gold to rich gold
            return select_random_string(rnd,
                                        (char const *[]){ colors_watery_gold, colors_rich_gold },
                                        2);
        case gem_kind_alexandrite:
            return colors_dark_green;
        case gem_kind_amethyst:
            return colors_deep_purple;
        case gem_kind_chrysoberyl:
            // yellow green to green
            return select_random_string(rnd,
                                        (char const *[]){ colors_yellow_green, colors_green },
                                        2);
        case gem_kind_coral:
            return colors_crimson;
        case gem_kind_jade:
            // light green, deep green, green and white, white
            return select_random_string(rnd,
                                        (char const *[]){
                                                colors_light_green,
                                                colors_deep_green,
                                                colors_green_and_white,
                                                colors_white
                                        },
                                        4);
        case gem_kind_jet:
            return colors_deep_black;
        case gem_kind_tourmaline:
            // green pale, blue pale, brown pale, or reddish pale
            return select_random_string(rnd,
                                        (char const *[]){
                                                colors_green_pale,
                                                colors_blue_pale,
                                                colors_brown_pale,
                                                colors_reddish_pale
                                        },
                                        4);
        /* fancy or precious */
        case gem_kind_garnet:
            // red, brown-green, or violet (the most prized)
            return select_random_string(rnd,
                                        (char const *[]){
                                                colors_red,
                                                colors_brown_green,
                                                colors_violet
                                        },
                                        3);
        case gem_kind_pearl:
            // lustrous white, yellowish, pinkish, etc. to pure black (the most prized)
            return select_random_string(rnd,
                                        (char const *[]){
                                                colors_lustrous_white,
                                                colors_yellowish,
                                                colors_pinkish,
                                                colors_black
                                        },
                                        4);
        case gem_kind_spinel:
            // red, red-brown, deep green, or very deep blue (the most prized)
            return select_random_string(rnd,
                                        (char const *[]){
                                                colors_red,
                                                colors_red_brown,
                                                colors_pinkdeep_greenish,
                                                colors_very_deep_blue
                                        },
                                        4);
        /* precious */
        case gem_kind_aquamarine:
            return colors_pale_blue_green;
        case gem_kind_peridot:
            return colors_rich_olive_green;
        case gem_kind_topaz:
            return colors_golden_yellow;
        /* gem */
        case gem_kind_black_opal:
            return colors_dark_green_with_black_mottling_and_golden_flecks;
        case gem_kind_emerald:
            return colors_deep_bright_green;
        case gem_kind_fire_opal:
            return colors_fiery_red;
        case gem_kind_opal:
            return colors_pale_blue_with_green_and_golden_mottling;
        case gem_kind_oriental_amethyst:
            return colors_rich_purple;
        case gem_kind_oriental_topaz:
            return colors_fiery_yellow;
        case gem_kind_sapphire:
            // clear to medium blue
            return select_random_string(rnd,
                                        (char const *[]){ colors_clear_blue, colors_medium_blue },
                                        2);
        case gem_kind_star_ruby:
            // translucent ruby with white 'star' center
            return select_random_string(rnd,
                                        (char const *[]){
                                                colors_clear_red_with_white_star_center,
                                                colors_deep_crimson_with_white_star_center
                                        },
                                        2);
        case gem_kind_star_sapphire:
            // translucent sapphire with white 'star' center
            return select_random_string(rnd,
                                        (char const *[]){
                                                colors_clear_blue_with_white_star_center,
                                                colors_medium_blue_with_white_star_center
                                        },
                                        2);
        /* jewel */
        case gem_kind_black_sapphire:
            return colors_lustrous_black_with_glowing_highlights;
        case gem_kind_diamond:
            // clear blue-white with lesser stones clear white or pale tints
            return select_random_string(rnd,
                                        (char const *[]){
                                                colors_clear_blue_white,
                                                colors_clear_white,
                                        },
                                        2);
        case gem_kind_jacinth:
            return colors_fiery_orange;
        case gem_kind_oriental_emerald:
            return colors_clear_bright_green;
        case gem_kind_ruby:
            // clear red to deep crimson
            return select_random_string(rnd,
                                        (char const *[]){ colors_clear_red, colors_deep_crimson },
                                        2);
    }
    return colors_gray;
}


static char *
gem_alloc_true_description(struct gem *gem)
{
    char *prefix = gem_alloc_true_description_prefix(gem);
    char *modifiers = gem_alloc_true_description_modifiers(gem);
    int value_in_cp = gem_value_in_cp(gem);
    char *value = coins_alloc_gp_cp_description(value_in_cp);

    char const *separator = astr_is_empty(modifiers) ? "" :  ": ";
    char *true_description = astr_f("%s (%s%s%s)",
                                    prefix, modifiers,
                                    separator, value);

    free(value);
    free(modifiers);
    free(prefix);

    return true_description;
}


static char *
gem_alloc_true_description_prefix(struct gem *gem)
{
    return astr_f("%s %s", gem_size_name(gem), gem_kind_name(gem));
}


static char *
gem_alloc_true_description_modifiers(struct gem *gem)
{
    char *description = astr_f("%s", gem_type_name(gem));
    if (gem->value_rank_modifier) {
        str_realloc_append_formatted(&description, ", rank %+i",
                                     gem->value_rank_modifier);
    }
    if (gem->value_percent_modifier) {
        str_realloc_append_formatted(&description, ", %+i%%",
                                     gem->value_percent_modifier - 100);
    }
    return description;
}


static char *
gem_alloc_visible_description(struct gem *gem)
{
    return astr_f("%s %s %s stone",
                  gem_size_name(gem),
                  gem_opacity_name(gem),
                  gem->colors);
}


struct cJSON *
gem_create_json_object(struct gem *gem)
{
    struct cJSON *json_object = cJSON_CreateObject();
    cJSON_AddStringToObject(json_object, "struct", "gem");
    cJSON_AddNumberToObject(json_object, "rev", 0);
    cJSON_AddStringToObject(json_object, "size", gem_size_name(gem));
    cJSON_AddStringToObject(json_object, "type", gem_type_name(gem));
    cJSON_AddStringToObject(json_object, "kind", gem_kind_name(gem));
    cJSON_AddStringToObject(json_object, "colors", gem->colors);
    cJSON_AddNumberToObject(json_object, "value_percent_modifier", gem->value_percent_modifier);
    cJSON_AddNumberToObject(json_object, "value_rank_modifier", gem->value_rank_modifier);
    return json_object;
}


void
gem_finalize(struct gem *gem)
{
    free(gem->true_description);
    free(gem->visible_description);
}


int
gem_value_in_cp(struct gem *gem)
{
    if (gem_type_unknown == gem->type) return 0;

    int const base_value_index = 5;
    int rank = base_value_index + gem->type + gem->size + gem->value_rank_modifier;
    if (rank < 1) {
        rank = 1;
    } else if (rank >= gem_values_by_rank_in_cp_count) {
        rank = (int)gem_values_by_rank_in_cp_count - 1;
    }
    
    int value_in_cp = gem_values_by_rank_in_cp[rank];
    if (gem->value_percent_modifier) {
        value_in_cp = (value_in_cp * gem->value_percent_modifier) / 100;
    }
    return value_in_cp;
}


void
gem_generate(struct gem *gem, struct rnd *rnd)
{
    int score = roll("1d100", rnd);
    if (score <= 25) {
        gem->type = gem_type_ornamental_stone;
        gem->kind = rnd_next_uniform_value_in_range(rnd,
                                                    gem_kind_azurite,
                                                    gem_kind_turquoise);
    } else if (score <= 50) {
        gem->type = gem_type_semi_precious_stone;
        gem->kind = rnd_next_uniform_value_in_range(rnd,
                                                    gem_kind_bloodstone,
                                                    gem_kind_zircon);
    } else if (score <= 70) {
        gem->type = gem_type_fancy_stone;
        gem->kind = rnd_next_uniform_value_in_range(rnd,
                                                    gem_kind_amber,
                                                    gem_kind_spinel);
    } else if (score <= 90) {
        gem->type = gem_type_precious_stone;
        gem->kind = rnd_next_uniform_value_in_range(rnd,
                                                    gem_kind_garnet,
                                                    gem_kind_topaz);
    } else if (score <= 99) {
        gem->type = gem_type_gem_stone;
        gem->kind = rnd_next_uniform_value_in_range(rnd,
                                                    gem_kind_black_opal,
                                                    gem_kind_star_sapphire);
    } else {
        gem->type = gem_type_jewel_stone;
        gem->kind = rnd_next_uniform_value_in_range(rnd,
                                                    gem_kind_black_sapphire,
                                                    gem_kind_ruby);
    }
    
    score = roll("1d100", rnd);
    if (score <= 25) {
        gem->size = gem_size_very_small;
    } else if (score <= 50) {
        gem->size = gem_size_small;
    } else if (score <= 70) {
        gem->size = gem_size_average;
    } else if (score <= 90) {
        gem->size = gem_size_large;
    } else if (score <= 99) {
        gem->size = gem_size_very_large;
    } else {
        gem->size = gem_size_huge;
    }
    
    do {
        score = roll("1d10", rnd);
        if (score == 1) {
            if (gem->value_rank_modifier >= 0 && gem->value_rank_modifier < 7) {
                ++gem->value_rank_modifier;
            }
        } else if (score == 2) {
            gem->value_percent_modifier = 200;
        } else if (score == 3) {
            gem->value_percent_modifier = 100 + (roll("1d6", rnd) * 10);
        } else if (score <= 8) {
            /* no change */
        } else if (score == 9) {
            gem->value_percent_modifier = 100 - (roll("1d4", rnd) * 10);
        } else {
            if (   gem->value_rank_modifier > -5
                && gem->value_rank_modifier <= 0)
            {
                --gem->value_rank_modifier;
            }
        }
    } while (   (score == 1 || score == 10)
             && gem->value_rank_modifier > -5
             && gem->value_rank_modifier < 7);
    
    gem->colors = gem_generate_colors(gem, rnd);
    gem->true_description = gem_alloc_true_description(gem);
    gem->visible_description = gem_alloc_visible_description(gem);
}


void
gem_initialize(struct gem *gem)
{
    memset(gem, 0, sizeof(struct gem));
}


void
gem_initialize_from_json_object(struct gem *gem, struct cJSON *json_object)
{
    gem_initialize(gem);

    if ( ! cJSON_IsObject(json_object)) return;
    if ( ! json_object_has_struct_member(json_object, "gem")) return;

    gem->size = json_object_get_string_enum_value(json_object, "size", gem_size_for_name, gem_size_average);
    gem->type = json_object_get_string_enum_value(json_object, "type", gem_type_for_name, gem_type_unknown);
    gem->kind = json_object_get_string_enum_value(json_object, "kind", gem_kind_for_name, gem_kind_unknown);

    char const *json_colors = json_object_get_string_value(json_object, "colors", colors_gray);
    char *const *colors_value = bsearch(&json_colors, colors, colors_count, sizeof colors[0], compare_colors);
    gem->colors = colors_value ? *colors_value : colors_gray;

    gem->value_percent_modifier = json_object_get_int_value(json_object,
                                                            "value_percent_modifier",
                                                            0);
    gem->value_rank_modifier = json_object_get_int_value(json_object,
                                                         "value_rank_modifier",
                                                         0);
    gem->true_description = gem_alloc_true_description(gem);
    gem->visible_description = gem_alloc_visible_description(gem);
}


static int
gem_kind_for_name(char const *name, int default_value)
{
    if (name) {
        for (size_t i = 0; i < gem_kind_names_count; ++i) {
            if (astr_eq(name, gem_kind_names[i])) return gem_kind_unknown + i;
        }
    }
    return default_value;
}


static char const *
gem_kind_name(struct gem *gem)
{
    int index = gem->kind - gem_kind_unknown;
    return gem_kind_names[index];
}


static enum gem_opacity
gem_opacity(struct gem *gem)
{
    static struct {
        enum gem_kind kind;
        enum gem_opacity opacity;
    } gem_kind_to_opacity[] = {
        { gem_kind_unknown, gem_opacity_transparent },
        /* ornamental */
        { gem_kind_azurite, gem_opacity_opaque },
        { gem_kind_banded_agate, gem_opacity_translucent },
        { gem_kind_blue_quartz, gem_opacity_transparent },
        { gem_kind_eye_agate, gem_opacity_translucent },
        { gem_kind_hematite, gem_opacity_opaque },
        { gem_kind_lapis_lazuli, gem_opacity_transparent },
        { gem_kind_malachite, gem_opacity_opaque },
        { gem_kind_moss_agate, gem_opacity_translucent },
        { gem_kind_obsidian, gem_opacity_opaque },
        { gem_kind_rhodochrosite, gem_opacity_opaque },
        { gem_kind_tiger_eye, gem_opacity_translucent },
        { gem_kind_turquoise, gem_opacity_opaque },
        /* semi-precious */
        { gem_kind_bloodstone, gem_opacity_opaque },
        { gem_kind_carnelian, gem_opacity_transparent },
        { gem_kind_chalcedony, gem_opacity_opaque },
        { gem_kind_chrysoprase, gem_opacity_translucent },
        { gem_kind_citrine, gem_opacity_transparent },
        { gem_kind_jasper, gem_opacity_opaque },
        { gem_kind_moonstone, gem_opacity_translucent },
        { gem_kind_onyx, gem_opacity_opaque },
        { gem_kind_rock_crystal, gem_opacity_transparent },
        { gem_kind_sardonyx, gem_opacity_opaque },
        { gem_kind_smoky_quartz, gem_opacity_transparent },
        { gem_kind_star_rose_quartz, gem_opacity_transparent },
        { gem_kind_zircon, gem_opacity_transparent },
        /* fancy */
        { gem_kind_amber, gem_opacity_transparent },
        { gem_kind_alexandrite, gem_opacity_transparent },
        { gem_kind_amethyst, gem_opacity_transparent },
        { gem_kind_chrysoberyl, gem_opacity_transparent },
        { gem_kind_coral, gem_opacity_opaque },
        { gem_kind_jade, gem_opacity_translucent },
        { gem_kind_jet, gem_opacity_opaque },
        { gem_kind_tourmaline, gem_opacity_transparent },
        /* fancy or precious */
        { gem_kind_garnet, gem_opacity_transparent },
        { gem_kind_pearl, gem_opacity_opaque },
        { gem_kind_spinel, gem_opacity_transparent },
        /* precious */
        { gem_kind_aquamarine, gem_opacity_transparent },
        { gem_kind_peridot, gem_opacity_transparent },
        { gem_kind_topaz, gem_opacity_transparent },
        /* gem */
        { gem_kind_black_opal, gem_opacity_translucent },
        { gem_kind_emerald, gem_opacity_transparent },
        { gem_kind_fire_opal, gem_opacity_translucent },
        { gem_kind_opal, gem_opacity_translucent },
        { gem_kind_oriental_amethyst, gem_opacity_transparent },
        { gem_kind_oriental_topaz, gem_opacity_transparent },
        { gem_kind_sapphire, gem_opacity_transparent },
        { gem_kind_star_ruby, gem_opacity_translucent },
        { gem_kind_star_sapphire, gem_opacity_translucent },
        /* jewel */
        { gem_kind_black_sapphire, gem_opacity_translucent },
        { gem_kind_diamond, gem_opacity_transparent },
        { gem_kind_jacinth, gem_opacity_transparent },
        { gem_kind_oriental_emerald, gem_opacity_transparent },
        { gem_kind_ruby, gem_opacity_transparent },
    };
    assert(gem->kind >= gem_kind_unknown);
    assert(gem->kind <= gem_kind_ruby);
    return gem_kind_to_opacity[gem->kind].opacity;
}


static char const *
gem_opacity_name(struct gem *gem)
{
    int index = gem_opacity(gem) - gem_opacity_transparent;
    return gem_opacity_names[index];
}


static int
gem_size_for_name(char const *name, int default_value)
{
    if (name) {
        for (size_t i = 0; i < gem_size_names_count; ++i) {
            if (astr_eq(name, gem_size_names[i])) return gem_size_very_small + i;
        }
    }
    return default_value;
}


static char const *
gem_size_name(struct gem *gem)
{
    int index = gem->size - gem_size_very_small;
    return gem_size_names[index];
}


static int
gem_type_for_name(char const *name, int default_value)
{
    if (name) {
        for (size_t i = 0; i < gem_type_names_count; ++i) {
            if (astr_eq(name, gem_type_names[i])) return gem_type_unknown + i;
        }
    }
    return default_value;
}


static char const *
gem_type_name(struct gem *gem)
{
    int index = gem->type - gem_type_unknown;
    return gem_type_names[index];
}
