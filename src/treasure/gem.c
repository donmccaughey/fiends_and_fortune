#include "gem.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <base/base.h>
#include <background/background.h>
#include <mechanics/mechanics.h>

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

static char const *const gem_type_names[] = {
    "unknown",
    "ornamental",
    "semi-precious",
    "fancy",
    "precious",
    "gem",
    "jewel"
};

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

static size_t const gem_values_by_rank_in_cp_count = sizeof gem_values_by_rank_in_cp
                                                   / sizeof(gem_values_by_rank_in_cp[0]);

static char *
gem_alloc_true_description_modifiers(struct gem *gem);

static char *
gem_alloc_true_description_prefix(struct gem *gem);

static char const *
gem_kind_name(struct gem *gem);

static enum gem_opacity
gem_opacity(struct gem *gem);

static char const *
gem_opacity_name(struct gem *gem);

static char const *
gem_size_name(struct gem *gem);

static char const *
gem_type_name(struct gem *gem);


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
            return "gray";
        /* ornamental */
        case gem_kind_azurite:
            return "mottled deep blue";
        case gem_kind_banded_agate:
            // striped brown and blue and white and reddish
            return select_random_string(rnd,
                                        (char const *[]){
                                                "striped brown",
                                                "striped blue",
                                                "striped white",
                                                "striped reddish",
                                        },
                                        4);
        case gem_kind_blue_quartz:
            return "pale blue";
        case gem_kind_eye_agate:
            // circles of gray, white, brown, blue and/or green
            return select_random_string(rnd,
                                        (char const *[]){
                                                "circles of gray",
                                                "circles of white",
                                                "circles of brown",
                                                "circles of blue",
                                                "circles of green"
                                        },
                                        5);
        case gem_kind_hematite:
            return "gray-black";
        case gem_kind_lapis_lazuli:
            return "light and dark blue with yellow flecks";
        case gem_kind_malachite:
            return "striated light and dark green";
        case gem_kind_moss_agate:
            // pink or yellow-white with grayish or greenish 'moss markings'
            return select_random_string(rnd,
                                        (char const *[]){
                                                "pink",
                                                "yellow-white with grayish markings",
                                                "yellow-white with greenish markings"
                                        },
                                        3);
        case gem_kind_obsidian:
            return "black";
        case gem_kind_rhodochrosite:
            return "light pink";
        case gem_kind_tiger_eye:
            return "rich brown with golden center under-hue";
        case gem_kind_turquoise:
            return "light blue-green";
        /* semi-precious */
        case gem_kind_bloodstone:
            return "dark gray with red flecks";
        case gem_kind_carnelian:
            // orange to reddish brown
            return select_random_string(rnd,
                                        (char const *[]){ "orange", "reddish brown" },
                                        2);
        case gem_kind_chalcedony:
            return "white";
        case gem_kind_chrysoprase:
            // apple green to emerald green
            return select_random_string(rnd,
                                        (char const *[]){ "apple green", "emerald green" },
                                        2);
        case gem_kind_citrine:
            return "pale yellow brown";
        case gem_kind_jasper:
            // blue, black to brown
            return select_random_string(rnd,
                                        (char const *[]){ "blue", "black", "brown" },
                                        3);
        case gem_kind_moonstone:
            return "white with pale blue glow";
        case gem_kind_onyx:
            // bands of black and white or pure black or white
            return select_random_string(rnd,
                                        (char const *[]){
                                                "bands of black and white",
                                                "black",
                                                "white"
                                        },
                                        3);
        case gem_kind_rock_crystal:
            return "clear";
        case gem_kind_sardonyx:
            // bands of sard (red) and onyx (white) or sard
            return select_random_string(rnd,
                                        (char const *[]){ "bands of red and white", "red" },
                                        2);
        case gem_kind_smoky_quartz:
            // gray, yellow, or blue (Cairngorm), all light
            return select_random_string(rnd,
                                        (char const *[]){
                                                "light gray",
                                                "light yellow",
                                                "light blue"
                                        },
                                        3);
        case gem_kind_star_rose_quartz:
            return "rosy stone with white 'star' center";
        case gem_kind_zircon:
            return "clear pale blue-green";
        /* fancy */
        case gem_kind_amber:
            // watery gold to rich gold
            return select_random_string(rnd,
                                        (char const *[]){ "watery gold", "rich gold" },
                                        2);
        case gem_kind_alexandrite:
            return "dark green";
        case gem_kind_amethyst:
            return "deep purple";
        case gem_kind_chrysoberyl:
            // yellow green to green
            return select_random_string(rnd,
                                        (char const *[]){ "yellow green", "green" },
                                        2);
        case gem_kind_coral:
            return "crimson";
        case gem_kind_jade:
            // light green, deep green, green and white, white
            return select_random_string(rnd,
                                        (char const *[]){
                                                "light green",
                                                "deep green",
                                                "green and white",
                                                "white"
                                        },
                                        4);
        case gem_kind_jet:
            return "deep black";
        case gem_kind_tourmaline:
            // green pale, blue pale, brown pale, or reddish pale
            return select_random_string(rnd,
                                        (char const *[]){
                                                "green pale",
                                                "blue pale",
                                                "brown pale",
                                                "reddish pale"
                                        },
                                        4);
        /* fancy or precious */
        case gem_kind_garnet:
            // red, brown-green, or violet (the most prized)
            return select_random_string(rnd,
                                        (char const *[]){
                                                "red",
                                                "brown-green",
                                                "violet"
                                        },
                                        3);
        case gem_kind_pearl:
            // lustrous white, yellowish, pinkish, etc. to pure black (the most prized)
            return select_random_string(rnd,
                                        (char const *[]){
                                                "lustrous white",
                                                "yellowish",
                                                "pinkish",
                                                "black"
                                        },
                                        4);
        case gem_kind_spinel:
            // red, red-brown, deep green, or very deep blue (the most prized)
            return select_random_string(rnd,
                                        (char const *[]){
                                                "red",
                                                "red-brown",
                                                "pinkdeep greenish",
                                                "very deep blue"
                                        },
                                        4);
        /* precious */
        case gem_kind_aquamarine:
            return "pale blue-green";
        case gem_kind_peridot:
            return "rich olive green";
        case gem_kind_topaz:
            return "golden yellow";
        /* gem */
        case gem_kind_black_opal:
            return "dark green with black mottling and golden flecks";
        case gem_kind_emerald:
            return "deep bright green";
        case gem_kind_fire_opal:
            return "fiery red";
        case gem_kind_opal:
            return "pale blue with green and golden mottling";
        case gem_kind_oriental_amethyst:
            return "rich purple";
        case gem_kind_oriental_topaz:
            return "fiery yellow";
        case gem_kind_sapphire:
            // clear to medium blue
            return select_random_string(rnd,
                                        (char const *[]){ "clear blue", "medium blue" },
                                        2);
        case gem_kind_star_ruby:
            // translucent ruby with white 'star' center
            return select_random_string(rnd,
                                        (char const *[]){
                                                "clear red with white 'star' center",
                                                "deep crimson with white 'star' center"
                                        },
                                        2);
        case gem_kind_star_sapphire:
            // translucent sapphire with white 'star' center
            return select_random_string(rnd,
                                        (char const *[]){
                                                "clear blue with white 'star' center",
                                                "medium blue with white 'star' center"
                                        },
                                        2);
        /* jewel */
        case gem_kind_black_sapphire:
            return "lustrous black with glowing highlights";
        case gem_kind_diamond:
            // clear blue-white with lesser stones clear white or pale tints
            return select_random_string(rnd,
                                        (char const *[]){
                                                "clear blue-white",
                                                "clear white",
                                        },
                                        2);
        case gem_kind_jacinth:
            return "fiery orange";
        case gem_kind_oriental_emerald:
            return "clear bright green";
        case gem_kind_ruby:
            // clear red to deep crimson
            return select_random_string(rnd,
                                        (char const *[]){ "clear red", "deep crimson" },
                                        2);
    }
    return "gray";
}


static char *
gem_alloc_true_description(struct gem *gem)
{
    char *prefix = gem_alloc_true_description_prefix(gem);
    char *modifiers = gem_alloc_true_description_modifiers(gem);
    int value_in_cp = gem_value_in_cp(gem);
    char *value = coins_alloc_gp_cp_description(value_in_cp);

    char const *separator = str_not_empty(modifiers) ? ": " : "";
    char *true_description = str_alloc_formatted("%s (%s%s%s)",
                                                 prefix, modifiers,
                                                 separator, value);

    free_or_die(value);
    free_or_die(modifiers);
    free_or_die(prefix);

    return true_description;
}


static char *
gem_alloc_true_description_prefix(struct gem *gem)
{
    return str_alloc_formatted("%s %s", gem_size_name(gem), gem_kind_name(gem));
}


static char *
gem_alloc_true_description_modifiers(struct gem *gem)
{
    char *description = str_alloc_formatted("%s", gem_type_name(gem));
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
    return str_alloc_formatted("%s %s %s stone",
                               gem_size_name(gem),
                               gem_opacity_name(gem),
                               gem->colors);
}


void
gem_finalize(struct gem *gem)
{
    free_or_die(gem->true_description);
    free_or_die(gem->visible_description);
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


static char const *
gem_size_name(struct gem *gem)
{
    int index = gem->size - gem_size_very_small;
    return gem_size_names[index];
}


static char const *
gem_type_name(struct gem *gem)
{
    int index = gem->type - gem_type_unknown;
    return gem_type_names[index];
}
