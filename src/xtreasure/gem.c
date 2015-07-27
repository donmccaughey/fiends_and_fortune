#include "gem.h"

#include <stdlib.h>
#include <string.h>

#include "alloc_or_die.h"
#include "coins.h"
#include "dice.h"
#include "str.h"


static char const *const gem_size_names[] = {
    "very small",
    "small",
    "average",
    "large",
    "very large",
    "huge"
};

static char const *const gem_type_names[] = {
    "fake stone",
    "ornamental stone",
    "semi-precious stone",
    "fancy stone",
    "precious stone",
    "gem stone",
    "jewel stone"
};

static int const gem_values_by_rank_in_cp[] = {
    0,
    sp_to_cp(1),
    sp_to_cp(5),
    sp_to_cp(10),
    gp_to_cp(1),
    gp_to_cp(5),
    gp_to_cp(10),
    gp_to_cp(50),
    gp_to_cp(100),
    gp_to_cp(500),
    gp_to_cp(1000),
    gp_to_cp(5000),
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


static char const *
gem_size_name(struct gem *gem);

static char const *
gem_type_name(struct gem *gem);


static char *
gem_describe(struct gem *gem)
{
    char *description;
    asprintf_or_die(&description, "%s %s",
                    gem_size_name(gem), gem_type_name(gem));
    return description;
}


static char *
gem_describe_modifiers(struct gem *gem)
{
    char *description = str_alloc_empty();
    if (gem->value_rank_modifier) {
        description = str_realloc_append_formatted(description,
                                                   "rank %+i", gem->value_rank_modifier);
    }
    if (gem->value_percent_modifier) {
        if (description[0]) {
            description = str_realloc_append_formatted(description, ", ");
        }
        description = str_realloc_append_formatted(description,
                                                   "%+i%%", gem->value_percent_modifier - 100);
    }
    return description;
}


void
gem_finalize(struct gem *gem)
{
    free_or_die(gem->true_description);
}


int
gem_value_in_cp(struct gem *gem)
{
    int rank = gem->type + gem->size + gem->value_rank_modifier;
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
    } else if (score <= 50) {
        gem->type = gem_type_semi_precious_stone;
    } else if (score <= 70) {
        gem->type = gem_type_fancy_stone;
    } else if (score <= 90) {
        gem->type = gem_type_precious_stone;
    } else if (score <= 99) {
        gem->type = gem_type_gem_stone;
    } else {
        gem->type = gem_type_jewel_stone;
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
    
    char *description = gem_describe(gem);
    char *modifiers = gem_describe_modifiers(gem);
    int value_in_cp = gem_value_in_cp(gem);
    char *value_description = coins_alloc_gp_cp_description(value_in_cp);
    
    char const *separator = modifiers[0] ? ": " : "";
    asprintf_or_die(&gem->true_description, "%s (%s%s%s)",
                    description, modifiers, separator, value_description);
    
    free_or_die(value_description);
    free_or_die(modifiers);
    free_or_die(description);
}


void
gem_initialize(struct gem *gem)
{
    memset(gem, 0, sizeof(struct gem));
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
    int index = gem->type - gem_type_fake;
    return gem_type_names[index];
}
