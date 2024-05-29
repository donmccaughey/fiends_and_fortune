#include "jewelry.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <base/base.h>
#include <background/background.h>
#include <json/json.h>
#include <mechanics/mechanics.h>

#include "astr.h"
#include "coins.h"


struct jewelry_form_table {
    int score;
    enum jewelry_form form;
    char const *name;
};


struct jewelry_rank {
    char const *base_value;
    int base_value_multiplier;
    bool has_gems;
    enum jewelry_material materials[3];
    int materials_count;
};


static struct jewelry_form_table jewelry_form_table[] = {
    { 2, jewelry_form_anklet, "anklet" },
    { 6, jewelry_form_armband, "arm band" },
    { 9, jewelry_form_belt, "belt" },
    { 12, jewelry_form_box, "small box" },
    { 16, jewelry_form_bracelet, "bracelet" },
    { 19, jewelry_form_brooch, "brooch" },
    { 21, jewelry_form_buckle, "buckle" },
    { 25, jewelry_form_chain, "chain" },
    { 26, jewelry_form_chalice, "chalice" },
    { 27, jewelry_form_choker, "choker" },
    { 30, jewelry_form_clasp, "clasp" },
    { 32, jewelry_form_coffer, "coffer" },
    { 33, jewelry_form_collar, "collar" },
    { 35, jewelry_form_comb, "comb" },
    { 36, jewelry_form_coronet, "coronet" },
    { 37, jewelry_form_crown, "crown" },
    { 39, jewelry_form_decanter, "decanter" },
    { 40, jewelry_form_diadem, "diadem" },
    { 45, jewelry_form_earring, "earring" },
    { 47, jewelry_form_fob, "fob" },
    { 52, jewelry_form_goblet, "goblet" },
    { 54, jewelry_form_headband, "headband" },
    { 57, jewelry_form_idol, "idol" },
    { 59, jewelry_form_locket, "locket" },
    { 62, jewelry_form_medal, "medal" },
    { 68, jewelry_form_medallion, "medallion" },
    { 75, jewelry_form_necklace, "necklace" },
    { 78, jewelry_form_pendant, "pendant" },
    { 83, jewelry_form_pin, "pin" },
    { 84, jewelry_form_orb, "orb" },
    { 93, jewelry_form_ring, "ring" },
    { 94, jewelry_form_sceptre, "sceptre" },
    { 96, jewelry_form_seal, "seal" },
    { 99, jewelry_form_statuette, "statuette" },
    { 100, jewelry_form_tiara, "tiara" }
};
static size_t jewelry_form_table_count = ARRAY_COUNT(jewelry_form_table);


static char const *const jewelry_material_names[] = {
    "fake",
    "ivory",
    "silver",
    "silver and gold",
    "gold",
    "jade",
    "coral",
    "platinum",
    "silver with gems",
    "gold with gems",
    "platinum with gems",
};
static size_t const jewelry_material_names_count = ARRAY_COUNT(jewelry_material_names);


static char const *const jewelry_material_formats[] = {
    "fake %s",
    "ivory %s",
    "silver %s",
    "silver and gold %s",
    "gold %s",
    "jade %s",
    "coral %s",
    "platinum %s",
    "silver %s with gems",
    "gold %s with gems",
    "platinum %s with gems"
};


static struct jewelry_rank const jewelry_ranks[] = {
    {
        .base_value="",
        .base_value_multiplier=0,
        .has_gems=false,
        .materials={},
        .materials_count=0
    },
    {
        .base_value="1d10",
        .base_value_multiplier=gp_to_cp(100),
        .has_gems=false,
        .materials={
            jewelry_material_ivory,
            jewelry_material_silver
        },
        .materials_count=2
    },
    {
        .base_value="2d6",
        .base_value_multiplier=gp_to_cp(100),
        .has_gems=false,
        .materials={
            jewelry_material_silver_and_gold
        },
        .materials_count=1
    },
    {
        .base_value="3d6",
        .base_value_multiplier=gp_to_cp(100),
        .has_gems=false,
        .materials={
            jewelry_material_gold
        },
        .materials_count=1
    },
    {
        .base_value="5d6",
        .base_value_multiplier=gp_to_cp(100),
        .has_gems=false,
        .materials={
            jewelry_material_jade,
            jewelry_material_coral,
            jewelry_material_platinum
        },
        .materials_count=3
    },
    {
        .base_value="1d6",
        .base_value_multiplier=gp_to_cp(1000),
        .has_gems=true,
        .materials={
            jewelry_material_silver_with_gems
        },
        .materials_count=1
    },
    {
        .base_value="2d4",
        .base_value_multiplier=gp_to_cp(1000),
        .has_gems=true,
        .materials={
            jewelry_material_gold_with_gems
        },
        .materials_count=1
    },
    {
        .base_value="2d6",
        .base_value_multiplier=gp_to_cp(1000),
        .has_gems=true,
        .materials={
            jewelry_material_platinum_with_gems
        },
        .materials_count=1
    }
};
static size_t const jewelry_rank_count = ARRAY_COUNT(jewelry_ranks);
static int const jewelry_min_rank = 1;
static int const jewelry_max_rank = (int)(jewelry_rank_count - 1);


static char *
jewelry_alloc_true_description_prefix(struct jewelry *jewelry);

static char *
jewelry_alloc_true_description_modifiers(struct jewelry *jewelry);

static char const *
jewelry_form_name(struct jewelry *jewelry);

static int
jewelry_material_for_name(char const *name, int default_value);

static char const *
jewelry_material_name(struct jewelry *jewelry);


static char *
jewelry_alloc_true_description(struct jewelry *jewelry)
{
    char *prefix = jewelry_alloc_true_description_prefix(jewelry);
    char *modifiers = jewelry_alloc_true_description_modifiers(jewelry);
    int value_in_cp = jewelry_value_in_cp(jewelry);
    char *value_description = coins_alloc_gp_cp_description(value_in_cp);

    char const *separator = astr_is_empty(modifiers) ? "" : ": ";
    char *true_description = str_alloc_formatted("%s (%s%s%s)",
                                                 prefix,
                                                 modifiers,
                                                 separator,
                                                 value_description);

    free(modifiers);
    free(value_description);
    free(prefix);

    return true_description;
}


static char *
jewelry_alloc_true_description_prefix(struct jewelry *jewelry)
{
    char const *format = jewelry_material_formats[jewelry->material];
    char const *name = jewelry_form_table[jewelry->form].name;
    return str_alloc_formatted(format, name);
}


static char *
jewelry_alloc_true_description_modifiers(struct jewelry *jewelry)
{
    char *description = str_alloc_empty();
    if (jewelry->workmanship_bonus) {
        str_realloc_append_formatted(&description, "workmanship %+i",
                                     jewelry->workmanship_bonus);
    }
    if (jewelry->exceptional_stone_bonus) {
        if ( ! astr_is_empty(description)) {
            str_realloc_append_formatted(&description, ", ");
        }
        str_realloc_append_formatted(&description, "exceptional stone %+i",
                                     jewelry->exceptional_stone_bonus);
    }
    return description;
}


struct cJSON *
jewelry_create_json_object(struct jewelry *jewelry)
{
    struct cJSON *json_object = cJSON_CreateObject();
    cJSON_AddStringToObject(json_object, "struct", "jewelry");
    cJSON_AddNumberToObject(json_object, "rev", 0);
    cJSON_AddBoolToObject(json_object, "has_gems", jewelry->has_gems);
    cJSON_AddStringToObject(json_object, "form", jewelry_form_name(jewelry));
    cJSON_AddStringToObject(json_object, "material", jewelry_material_name(jewelry));
    cJSON_AddNumberToObject(json_object, "workmanship_bonus", jewelry->workmanship_bonus);
    cJSON_AddNumberToObject(json_object, "exceptional_stone_bonus", jewelry->exceptional_stone_bonus);
    cJSON_AddNumberToObject(json_object, "value_in_cp", jewelry->value_in_cp);
    return json_object;
}


void
jewelry_finalize(struct jewelry *jewelry)
{
    free(jewelry->true_description);
}


static int
jewelry_form_for_name(char const *name, int default_value)
{
    if (name) {
        for (size_t i = 0; i < jewelry_form_table_count; ++i) {
            if (astr_eq(name, jewelry_form_table[i].name)) return jewelry_form_anklet + i;
        }
    }
    return default_value;
}


static char const *
jewelry_form_name(struct jewelry *jewelry)
{
    assert(jewelry->form >= jewelry_form_anklet);
    assert(jewelry->form <= jewelry_form_tiara);
    int index = jewelry->form - jewelry_form_anklet;
    return jewelry_form_table[index].name;
}


void
jewelry_generate(struct jewelry *jewelry, struct rnd *rnd)
{
    int rank;
    int score = roll("1d100", rnd);
    if (score <= 10) {
        rank = 1;
    } else if (score <= 20) {
        rank = 2;
    } else if (score <= 40) {
        rank = 3;
    } else if (score <= 50) {
        rank = 4;
    } else if (score <= 70) {
        rank = 5;
    } else if (score <= 90) {
        rank = 6;
    } else {
        rank = 7;
    }
    assert(rank >= jewelry_min_rank);
    assert(rank <= jewelry_max_rank);
    
    jewelry->has_gems = jewelry_ranks[rank].has_gems;
    
    if (jewelry_ranks[rank].materials_count == 1) {
        jewelry->material = jewelry_ranks[rank].materials[0];
    } else {
        struct dice dice = dice_make(1, jewelry_ranks[rank].materials_count);
        score = dice_roll(dice, rnd, NULL);
        jewelry->material = jewelry_ranks[rank].materials[score - 1];
    }
    
    score = roll(jewelry_ranks[rank].base_value, rnd);
    jewelry->value_in_cp = score * jewelry_ranks[rank].base_value_multiplier;
    
    score = roll("1d100", rnd);
    for (int i = 0; i < jewelry_form_table_count; ++i) {
        if (score <= jewelry_form_table[i].score) {
            jewelry->form = jewelry_form_table[i].form;
            break;
        }
    }
    
    int const max_workmanship_bonus = 12;
    do {
        score = roll("1d10", rnd);
        if (score == 1) {
            ++jewelry->workmanship_bonus;
        }
    } while (score == 1 && jewelry->workmanship_bonus < max_workmanship_bonus);
    for (int i = 0; i < jewelry->workmanship_bonus; ++i) {
        int max_value_in_cp = dice_max_score(dice_parse(jewelry_ranks[rank].base_value))
                            * jewelry_ranks[rank].base_value_multiplier;
        if (jewelry->value_in_cp < max_value_in_cp) {
            jewelry->value_in_cp = max_value_in_cp;
        } else if (rank < jewelry_max_rank) {
            ++rank;
            score = roll(jewelry_ranks[rank].base_value, rnd);
            jewelry->value_in_cp = score * jewelry_ranks[rank].base_value_multiplier;
        }
    }
    
    int const max_exceptional_stone_bonus = 128;
    if (jewelry->has_gems) {
        score = roll("1d8", rnd);
        if (score == 1) {
            jewelry->exceptional_stone_bonus = 1;
            do {
                score = roll("1d6", rnd);
                if (score == 1) jewelry->exceptional_stone_bonus *= 2;
            } while (   score == 1
                     && jewelry->exceptional_stone_bonus < max_exceptional_stone_bonus);
        }
        jewelry->value_in_cp += gp_to_cp(jewelry->exceptional_stone_bonus * 5000);
    }

    jewelry->true_description = jewelry_alloc_true_description(jewelry);
}


void
jewelry_initialize(struct jewelry *jewelry)
{
    memset(jewelry, 0, sizeof(struct jewelry));
}


void
jewelry_initialize_from_json_object(struct jewelry *jewelry,
                                    struct cJSON *json_object)
{
    jewelry_initialize(jewelry);

    if ( ! cJSON_IsObject(json_object)) return;
    if ( ! json_object_has_struct_member(json_object, "jewelry")) return;

    jewelry->has_gems = json_object_get_bool_value(json_object, "has_gems", false);
    jewelry->form = json_object_get_string_enum_value(json_object, "form", jewelry_form_for_name, jewelry_form_anklet);
    jewelry->material = json_object_get_string_enum_value(json_object, "material", jewelry_material_for_name, jewelry_material_fake);
    jewelry->workmanship_bonus = json_object_get_int_value(json_object, "workmanship_bonus", 0);
    jewelry->exceptional_stone_bonus = json_object_get_int_value(json_object, "exceptional_stone_bonus", 0);
    jewelry->value_in_cp = json_object_get_int_value(json_object, "value_in_cp", 0);
    jewelry->true_description = jewelry_alloc_true_description(jewelry);
}


static int
jewelry_material_for_name(char const *name, int default_value)
{
    if (name) {
        for (size_t i = 0; i < jewelry_material_names_count; ++i) {
            if (astr_eq(name, jewelry_material_names[i])) return jewelry_material_fake + i;
        }
    }
    return default_value;
}


static char const *
jewelry_material_name(struct jewelry *jewelry)
{
    assert(jewelry->material >= jewelry_material_fake);
    assert(jewelry->material <= jewelry_material_platinum_with_gems);
    int index = jewelry->material - jewelry_material_fake;
    return jewelry_material_names[index];
}


int
jewelry_value_in_cp(struct jewelry *jewelry)
{
    return jewelry->value_in_cp;
}
