#include "jewelry.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <base/base.h>
#include <background/background.h>
#include <mechanics/mechanics.h>

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
static size_t jewelry_form_table_count = sizeof jewelry_form_table
                                       / sizeof jewelry_form_table[0];


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
static size_t const jewelry_rank_count = sizeof jewelry_ranks
                                       / sizeof jewelry_ranks[0];

static int const jewelry_min_rank = 1;
static int const jewelry_max_rank = (int)(jewelry_rank_count - 1);


static char *
jewelry_alloc_description(struct jewelry *jewelry)
{
    char const *format = jewelry_material_formats[jewelry->material];
    char const *name = jewelry_form_table[jewelry->form].name;
    return str_alloc_formatted(format, name);
}


static char *
jewelry_alloc_modifiers_description(struct jewelry *jewelry)
{
    char *description = str_alloc_empty();
    if (jewelry->workmanship_bonus) {
        str_realloc_append_formatted(&description, "workmanship %+i",
                                     jewelry->workmanship_bonus);
    }
    if (jewelry->exceptional_stone_bonus) {
        if (str_not_empty(description)) {
            str_realloc_append_formatted(&description, ", ");
        }
        str_realloc_append_formatted(&description, "exceptional stone %+i",
                                     jewelry->exceptional_stone_bonus);
    }
    return description;
}


void
jewelry_finalize(struct jewelry *jewelry)
{
    free_or_die(jewelry->true_description);
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
    
    char *description = jewelry_alloc_description(jewelry);
    char *modifiers = jewelry_alloc_modifiers_description(jewelry);
    int value_in_cp = jewelry_value_in_cp(jewelry);
    char *value_description = coins_alloc_gp_cp_description(value_in_cp);
    
    char const *separator = str_not_empty(modifiers) ? ": " : "";
    jewelry->true_description = str_alloc_formatted("%s (%s%s%s)",
                                                    description,
                                                    modifiers,
                                                    separator,
                                                    value_description);
    
    free_or_die(modifiers);
    free_or_die(value_description);
    free_or_die(description);
}


void
jewelry_initialize(struct jewelry *jewelry)
{
    memset(jewelry, 0, sizeof(struct jewelry));
}


int
jewelry_value_in_cp(struct jewelry *jewelry)
{
    return jewelry->value_in_cp;
}
