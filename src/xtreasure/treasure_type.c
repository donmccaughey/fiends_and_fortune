#include "treasure_type.h"

#include <assert.h>
#include <stdlib.h>

#include "common/alloc_or_die.h"
#include "common/dice.h"
#include "common/fail.h"
#include "common/str.h"

#include "gem.h"
#include "jewelry.h"
#include "MagicItem.h"
#include "Treasure.h"
#include "treasure_map.h"


struct coins_gems_or_jewelry {
    bool is_possible;
    bool is_per_individual;
    int percent_chance;
    char const *amount;
    int multiplier;
};


struct maps_or_magic_type {
    int amount;
    char const *variable_amount;
    bool is_map_possible;
    PossibleMagicItems possible_magic_items;
};


struct maps_or_magic {
    bool is_possible;
    int percent_chance;
    struct maps_or_magic_type types[6];
    int type_count;
};


struct treasure_type {
    char letter;
    struct coins_gems_or_jewelry copper;
    struct coins_gems_or_jewelry silver;
    struct coins_gems_or_jewelry electrum;
    struct coins_gems_or_jewelry gold;
    struct coins_gems_or_jewelry platinum;
    struct coins_gems_or_jewelry gems;
    struct coins_gems_or_jewelry jewelry;
    struct maps_or_magic maps_or_magic;
};


static struct treasure_type treasure_types[] = {
    {
        .letter='A',
        .copper={ true, false, 25, "1D6", 1000 },
        .silver={ true, false, 30, "1D6", 1000 },
        .electrum={ true, false, 35, "1D6", 1000 },
        .gold={ true, false, 40, "1D10", 1000 },
        .platinum={ true, false, 25, "1D4", 100 },
        .gems={ true, false, 60, "4D10", 1 },
        .jewelry={ true, false, 50, "3D10", 1 },
        .maps_or_magic={
            true, 30,
            {
                { 3, "", true, ANY_MAGIC_ITEM }
            },
            1
        }
    },
    {
        .letter='B',
        .copper={ true, false, 50, "1D8", 1000 },
        .silver={ true, false, 25, "1D6", 1000 },
        .electrum={ true, false, 25, "1D4", 1000 },
        .gold={ true, false, 25, "1D3", 1000 },
        .platinum={ false, false, 0, "", 0 },
        .gems={ true, false, 30, "1D8", 1 },
        .jewelry={ true, false, 20, "1D4", 1 },
        .maps_or_magic={
            true, 10,
            {
                { 1, "", false, MAGIC_WEAPON_OR_ARMOR }
            },
            1
        }
    },
    {
        .letter='C',
        .copper={ true, false, 20, "1D12", 1000 },
        .silver={ true, false, 30, "1D6", 1000 },
        .electrum={ true, false, 10, "1D4", 1000 },
        .gold={ false, false, 0, "", 0 },
        .platinum={ false, false, 0, "", 0 },
        .gems={ true, false, 25, "1D6", 1 },
        .jewelry={ true, false, 20, "1D3", 1 },
        .maps_or_magic={
            true, 10,
            {
                { 2, "", true, ANY_MAGIC_ITEM }
            },
            1
        }
    },
    {
        .letter='D',
        .copper={ true, false, 10, "1D8", 1000 },
        .silver={ true, false, 15, "1D12", 1000 },
        .electrum={ true, false, 15, "1D8", 1000 },
        .gold={ true, false, 50, "1D6", 1000 },
        .platinum={ false, false, 0, "", 0 },
        .gems={ true, false, 30, "1D10", 1 },
        .jewelry={ true, false, 25, "1D6", 1 },
        .maps_or_magic={
            true, 15,
            {
                { 2, "", true, ANY_MAGIC_ITEM },
                { 1, "", false, POTION }
            },
            2
        }
    },
    {
        .letter='E',
        .copper={ true, false, 5, "1D10", 1000 },
        .silver={ true, false, 25, "1D12", 1000 },
        .electrum={ true, false, 25, "1D6", 1000 },
        .gold={ true, false, 25, "1D8", 1000 },
        .platinum={ false, false, 0, "", 0 },
        .gems={ true, false, 15, "1D12", 1 },
        .jewelry={ true, false, 10, "1D8", 1 },
        .maps_or_magic={
            true, 25,
            {
                { 3, "", true, ANY_MAGIC_ITEM },
                { 1, "", false, SCROLL }
            },
            2
        }
    },
    {
        .letter='F',
        .copper={ false, false, 0, "", 0 },
        .silver={ true, false, 10, "1D20", 1000 },
        .electrum={ true, false, 15, "1D12", 1000 },
        .gold={ true, false, 40, "1D10", 1000 },
        .platinum={ true, false, 35, "1D8", 100 },
        .gems={ true, false, 20, "3D10", 1 },
        .jewelry={ true, false, 10, "1D10", 1 },
        .maps_or_magic={
            true, 30,
            {
                { 3, "", true, NON_WEAPON_MAGIC },
                { 1, "", false, POTION },
                { 1, "", false, SCROLL }
            },
            3
        }
    },
    {
        .letter='G',
        .copper={ false, false, 0, "", 0 },
        .silver={ false, false, 0, "", 0 },
        .electrum={ false, false, 0, "", 0 },
        .gold={ true, false, 50, "10D4", 1000 },
        .platinum={ true, false, 50, "1D20", 100 },
        .gems={ true, false, 30, "5D4", 1 },
        .jewelry={ true, false, 25, "1D10", 1 },
        .maps_or_magic={
            true, 35,
            {
                { 4, "", true, ANY_MAGIC_ITEM },
                { 1, "", false, SCROLL }
            },
            2
        }
    },
    {
        .letter='H',
        .copper={ true, false, 25, "5D6", 1000 },
        .silver={ true, false, 40, "1D100", 1000 },
        .electrum={ true, false, 40, "10D4", 1000 },
        .gold={ true, false, 55, "10D6", 1000 },
        .platinum={ true, false, 25, "5D10", 100 },
        .gems={ true, false, 50, "1D100", 1 },
        .jewelry={ true, false, 50, "10D4", 1 },
        .maps_or_magic={
            true, 15,
            {
                { 4, "", true, ANY_MAGIC_ITEM },
                { 1, "", false, POTION },
                { 1, "", false, SCROLL }
            },
            3
        }
    },
    {
        .letter='I',
        .copper={ false, false, 0, "", 0 },
        .silver={ false, false, 0, "", 0 },
        .electrum={ false, false, 0, "", 0 },
        .gold={ false, false, 0, "", 0 },
        .platinum={ true, false, 30, "3D6", 100 },
        .gems={ true, false, 55, "2D10", 1 },
        .jewelry={ true, false, 50, "1D12", 1 },
        .maps_or_magic={
            true, 15,
            {
                { 1, "", true, ANY_MAGIC_ITEM }
            },
            1
        }
    },
    {
        .letter='J',
        .copper={ true, true, 100, "3D8", 1 },
        .silver={ false, false, 0, "", 0 },
        .electrum={ false, false, 0, "", 0 },
        .gold={ false, false, 0, "", 0 },
        .platinum={ false, false, 0, "", 0 },
        .gems={ false, false, 0, "", 0 },
        .jewelry={ false, false, 0, "", 0 },
        .maps_or_magic={ false, 0, {}, 0 }
    },
    {
        .letter='K',
        .copper={ false, false, 0, "", 0 },
        .silver={ true, true, 100, "3D6", 1 },
        .electrum={ false, false, 0, "", 0 },
        .gold={ false, false, 0, "", 0 },
        .platinum={ false, false, 0, "", 0 },
        .gems={ false, false, 0, "", 0 },
        .jewelry={ false, false, 0, "", 0 },
        .maps_or_magic={ false, 0, {}, 0 }
    },
    {
        .letter='L',
        .copper={ false, false, 0, "", 0 },
        .silver={ false, false, 0, "", 0 },
        .electrum={ true, true, 100, "2D6", 1 },
        .gold={ false, false, 0, "", 0 },
        .platinum={ false, false, 0, "", 0 },
        .gems={ false, false, 0, "", 0 },
        .jewelry={ false, false, 0, "", 0 },
        .maps_or_magic={ false, 0, {}, 0 }
    },
    {
        .letter='M',
        .copper={ false, false, 0, "", 0 },
        .silver={ false, false, 0, "", 0 },
        .electrum={ false, false, 0, "", 0 },
        .gold={ true, true, 100, "2D4", 1 },
        .platinum={ false, false, 0, "", 0 },
        .gems={ false, false, 0, "", 0 },
        .jewelry={ false, false, 0, "", 0 },
        .maps_or_magic={ false, 0, {}, 0 }
    },
    {
        .letter='N',
        .copper={ false, false, 0, "", 0 },
        .silver={ false, false, 0, "", 0 },
        .electrum={ false, false, 0, "", 0 },
        .gold={ false, false, 0, "", 0 },
        .platinum={ true, true, 100, "1D6", 1 },
        .gems={ false, false, 0, "", 0 },
        .jewelry={ false, false, 0, "", 0 },
        .maps_or_magic={ false, 0, {}, 0 }
    },
    {
        .letter='O',
        .copper={ true, false, 25, "1D4", 1000 },
        .silver={ true, false, 20, "1D3", 1000 },
        .electrum={ false, false, 0, "", 0 },
        .gold={ false, false, 0, "", 0 },
        .platinum={ false, false, 0, "", 0 },
        .gems={ false, false, 0, "", 0 },
        .jewelry={ false, false, 0, "", 0 },
        .maps_or_magic={ false, 0, {}, 0 }
    },
    {
        .letter='P',
        .copper={ false, false, 0, "", 0 },
        .silver={ true, false, 30, "1D6", 1000 },
        .electrum={ true, false, 25, "1D2", 1000 },
        .gold={ false, false, 0, "", 0 },
        .platinum={ false, false, 0, "", 0 },
        .gems={ false, false, 0, "", 0 },
        .jewelry={ false, false, 0, "", 0 },
        .maps_or_magic={ false, 0, {}, 0 }
    },
    {
        .letter='Q',
        .copper={ false, false, 0, "", 0 },
        .silver={ false, false, 0, "", 0 },
        .electrum={ false, false, 0, "", 0 },
        .gold={ false, false, 0, "", 0 },
        .platinum={ false, false, 0, "", 0 },
        .gems={ true, false, 50, "1D4", 1 },
        .jewelry={ false, false, 0, "", 0 },
        .maps_or_magic={ false, 0, {}, 0 }
    },
    {
        .letter='R',
        .copper={ false, false, 0, "", 0 },
        .silver={ false, false, 0, "", 0 },
        .electrum={ false, false, 0, "", 0 },
        .gold={ true, false, 40, "2D4", 1000 },
        .platinum={ true, false, 50, "10D6", 100 },
        .gems={ true, false, 55, "4D8", 1 },
        .jewelry={ true, false, 45, "1D12", 1 },
        .maps_or_magic={ false, 0, {}, 0 }
    },
    {
        .letter='S',
        .copper={ false, false, 0, "", 0 },
        .silver={ false, false, 0, "", 0 },
        .electrum={ false, false, 0, "", 0 },
        .gold={ false, false, 0, "", 0 },
        .platinum={ false, false, 0, "", 0 },
        .gems={ false, false, 0, "", 0 },
        .jewelry={ false, false, 0, "", 0 },
        .maps_or_magic={
            true, 40,
            {
                { 0, "2D4", false, POTION }
            },
            1
        }
    },
    {
        .letter='T',
        .copper={ false, false, 0, "", 0 },
        .silver={ false, false, 0, "", 0 },
        .electrum={ false, false, 0, "", 0 },
        .gold={ false, false, 0, "", 0 },
        .platinum={ false, false, 0, "", 0 },
        .gems={ false, false, 0, "", 0 },
        .jewelry={ false, false, 0, "", 0 },
        .maps_or_magic={
            true, 50,
            {
                { 0, "1D4", false, SCROLL }
            },
            1
        }
    },
    {
        .letter='U',
        .copper={ false, false, 0, "", 0 },
        .silver={ false, false, 0, "", 0 },
        .electrum={ false, false, 0, "", 0 },
        .gold={ false, false, 0, "", 0 },
        .platinum={ false, false, 0, "", 0 },
        .gems={ true, false, 90, "10D8", 1 },
        .jewelry={ true, false, 80, "5D6", 1 },
        .maps_or_magic={
            true, 70,
            {
                { 1, "", false, RING },
                { 1, "", false, ROD_STAFF_WAND },
                { 1, "", false, MISC_MAGIC },
                { 1, "", false, ARMOR_SHIELD },
                { 1, "", false, SWORD },
                { 1, "", false, MISC_WEAPON }
            },
            6
        }
    },
    {
        .letter='V',
        .copper={ false, false, 0, "", 0 },
        .silver={ false, false, 0, "", 0 },
        .electrum={ false, false, 0, "", 0 },
        .gold={ false, false, 0, "", 0 },
        .platinum={ false, false, 0, "", 0 },
        .gems={ false, false, 0, "", 0 },
        .jewelry={ false, false, 0, "", 0 },
        .maps_or_magic={
            true, 85,
            {
                { 2, "", false, RING },
                { 2, "", false, ROD_STAFF_WAND },
                { 2, "", false, MISC_MAGIC },
                { 2, "", false, ARMOR_SHIELD },
                { 2, "", false, SWORD },
                { 2, "", false, MISC_WEAPON }
            },
            6
        }
    },
    {
        .letter='W',
        .copper={ false, false, 0, "", 0 },
        .silver={ false, false, 0, "", 0 },
        .electrum={ false, false, 0, "", 0 },
        .gold={ true, false, 60, "5D6", 1000 },
        .platinum={ true, false, 15, "1D8", 100 },
        .gems={ true, false, 60, "10D8", 1 },
        .jewelry={ true, false, 50, "5D8", 1 },
        .maps_or_magic={
            true, 55,
            {
                { 1, "", true, NO_MAGIC_ITEM }
            },
            1
        }
    },
    {
        .letter='X',
        .copper={ false, false, 0, "", 0 },
        .silver={ false, false, 0, "", 0 },
        .electrum={ false, false, 0, "", 0 },
        .gold={ false, false, 0, "", 0 },
        .platinum={ false, false, 0, "", 0 },
        .gems={ false, false, 0, "", 0 },
        .jewelry={ false, false, 0, "", 0 },
        .maps_or_magic={
            true, 60,
            {
                { 1, "", false, MISC_MAGIC },
                { 1, "", false, POTION }
            },
            2
        }
    },
    {
        .letter='Y',
        .copper={ false, false, 0, "", 0 },
        .silver={ false, false, 0, "", 0 },
        .electrum={ false, false, 0, "", 0 },
        .gold={ true, false, 70, "2D6", 1000 },
        .platinum={ false, false, 0, "", 0 },
        .gems={ false, false, 0, "", 0 },
        .jewelry={ false, false, 0, "", 0 },
        .maps_or_magic={ false, 0, {}, 0 }
    },
    {
        .letter='Z',
        .copper={ true, false, 20, "1D3", 1000 },
        .silver={ true, false, 25, "1D4", 1000 },
        .electrum={ true, false, 25, "1D4", 1000 },
        .gold={ true, false, 30, "1D4", 1000 },
        .platinum={ true, false, 30, "1D6", 100 },
        .gems={ true, false, 55, "10D6", 1 },
        .jewelry={ true, false, 50, "5D6", 1 },
        .maps_or_magic={
            true, 50,
            {
                { 3, "", false, ANY_MAGIC_ITEM }
            },
            1
        }
    }
};


static char *
describe_coins_gems_or_jewelry(struct coins_gems_or_jewelry *coins_gems_or_jewelry);

static char *
describe_maps_or_magic(struct maps_or_magic *maps_or_magic);

static void
generate_treasure_coins(int *coins,
                        struct rnd *rnd,
                        struct coins_gems_or_jewelry *coins_type,
                        int individual_count);

static void
generate_treasure_gems(struct Treasure *treasure, struct rnd *rnd);

static void
generate_treasure_jewelry(struct Treasure *treasure, struct rnd *rnd);

static void
generate_treasure_maps_or_magic(struct Treasure *treasure, struct rnd *rnd);

static void
generate_treasure_maps_or_magic_type(struct maps_or_magic_type *type,
                                     struct Treasure *treasure,
                                     struct rnd *rnd);

static char const *
possible_maps_or_magic_name(bool is_map_possible,
                            PossibleMagicItems possible_magic_items);


char *
treasure_type_alloc_description(struct treasure_type *treasure_type,
                                bool include_header)
{
    /*          1000       1000       1000       1000       100                          */
    /* Type    copper     silver    electrum     gold     platinum     gems      jewelry */
    /*  A     1-6 :25%   1-6 :30%   1-6 :35%   1-10:40%   1-4 :25%   4-40:60%   3-30:50% */
    /*                                                                                   */
    /*  J    3-24 pieces                                                                 */
    /*      per individual                                                               */
    static char const *header =
    "          |  1,000's  |  1,000's  |  1,000's  |  1,000's  |   100's   |           |           |    Maps   \n"
    "Treasure  |    of     |    of     |    of     |    of     |    of     |           |           |     or    \n"
    "  Type    |  Copper   |  Silver   | Electrum  |   Gold    | Platinum  |   Gems    |  Jewelry  |   Magic   \n"
    "----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------\n";
    char *letter_format = "    %c    ";
    
    char *letter;
    char *copper = describe_coins_gems_or_jewelry(&treasure_type->copper);
    char *silver = describe_coins_gems_or_jewelry(&treasure_type->silver);
    char *electrum = describe_coins_gems_or_jewelry(&treasure_type->electrum);
    char *gold = describe_coins_gems_or_jewelry(&treasure_type->gold);
    char *platinum = describe_coins_gems_or_jewelry(&treasure_type->platinum);
    char *gems = describe_coins_gems_or_jewelry(&treasure_type->gems);
    char *jewelry = describe_coins_gems_or_jewelry(&treasure_type->jewelry);
    char *maps_or_magic = describe_maps_or_magic(&treasure_type->maps_or_magic);
    
    letter = str_alloc_formatted(letter_format, treasure_type->letter);
    
    char *format = "%s%s | %s | %s | %s | %s | %s | %s | %s | %s\n";
    char *description = str_alloc_formatted(format,
                                            (include_header ? header : ""),
                                            letter, copper, silver, electrum,
                                            gold, platinum, gems, jewelry,
                                            maps_or_magic);
    free_or_die(letter);
    free_or_die(copper);
    free_or_die(silver);
    free_or_die(electrum);
    free_or_die(gold);
    free_or_die(platinum);
    free_or_die(gems);
    free_or_die(jewelry);
    free_or_die(maps_or_magic);
    
    return description;
}


static char *
describe_coins_gems_or_jewelry(struct coins_gems_or_jewelry *coins_gems_or_jewelry)
{
    if (!coins_gems_or_jewelry->is_possible) return strdup_or_die("   nil   ");

    if (coins_gems_or_jewelry->is_per_individual) {
        struct dice amount = dice_parse(coins_gems_or_jewelry->amount);
        char *range = dice_alloc_range_description(amount);
        char *description = str_alloc_centered_and_formatted(9, "%s per",
                                                             range);
        free_or_die(range);
        return description;
    } else {
        struct dice amount = dice_parse(coins_gems_or_jewelry->amount);
        char *range = dice_alloc_range_description(amount);
        char *description = str_alloc_centered_and_formatted(9, "%s:%2i%%",
                                                             range,
                                                             coins_gems_or_jewelry->percent_chance);
        free_or_die(range);
        return description;
    }
}


static char *
describe_maps_or_magic(struct maps_or_magic *maps_or_magic)
{
    if (!maps_or_magic->is_possible) return strdup_or_die("   nil   ");
    
    char *type_descriptions[6] = {};
    for (int i = 0; i < maps_or_magic->type_count; ++i) {
        struct maps_or_magic_type *type = &maps_or_magic->types[i];
        char const *type_name = possible_maps_or_magic_name(type->is_map_possible,
                                                            type->possible_magic_items);
        if (str_not_empty(type->variable_amount)) {
            struct dice variable_amount = dice_parse(type->variable_amount);
            char *range = dice_alloc_range_description(variable_amount);
            type_descriptions[i] = str_alloc_formatted("%s %ss",
                                                       range, type_name);
            free_or_die(range);
        } else if (   type->is_map_possible
                   && type->possible_magic_items == ANY_MAGIC_ITEM)
        {
            type_descriptions[i] = str_alloc_formatted("any %i", type->amount);
        } else if (type->possible_magic_items == NON_WEAPON_MAGIC) {
            type_descriptions[i] = str_alloc_formatted("any %i except sword or misc weapon",
                                                       type->amount);
        } else if (type->possible_magic_items == ANY_MAGIC_ITEM) {
            type_descriptions[i] = str_alloc_formatted("any %i magic",
                                                       type->amount);
        } else {
            char const *plural = (type->amount == 1) ? "" : "s";
            type_descriptions[i] = str_alloc_formatted("%i %s%s",
                                                       type->amount,
                                                       type_name,
                                                       plural);
        }
    }

    char *description = NULL;
    for (int i = 0; i < maps_or_magic->type_count; ++i) {
        char const *separator = (i) ? ", " : "";
        str_realloc_append_formatted(&description, "%s%s",
                                     separator, type_descriptions[i]);
        free_or_die(type_descriptions[i]);
    }
    str_realloc_append_formatted(&description, ": %i%%",
                                 maps_or_magic->percent_chance);
    return description;
}


void
treasure_generate(struct treasure_type *treasure_type,
                  struct Treasure *treasure,
                  struct rnd *rnd,
                  int individual_count)
{
    treasure->type = treasure_type;
    
    generate_treasure_coins(&treasure->coins.cp, rnd,
                            &treasure_type->copper, individual_count);
    generate_treasure_coins(&treasure->coins.sp, rnd,
                            &treasure_type->silver, individual_count);
    generate_treasure_coins(&treasure->coins.ep, rnd,
                            &treasure_type->electrum, individual_count);
    generate_treasure_coins(&treasure->coins.gp, rnd,
                            &treasure_type->gold, individual_count);
    generate_treasure_coins(&treasure->coins.pp, rnd,
                            &treasure_type->platinum, individual_count);
    generate_treasure_gems(treasure, rnd);
    generate_treasure_jewelry(treasure, rnd);
    generate_treasure_maps_or_magic(treasure, rnd);
}


static void
generate_treasure_coins(int *coins,
                        struct rnd *rnd,
                        struct coins_gems_or_jewelry *coins_type,
                        int individual_count)
{
    *coins = 0;
    if (coins_type->is_possible) {
        if (coins_type->is_per_individual) {
            assert(individual_count > 0);
            for (int i = 0; i < individual_count; ++i) {
                *coins += roll(coins_type->amount, rnd);
            }
        } else {
            int percentRoll = roll("1d100", rnd);
            if (percentRoll <= coins_type->percent_chance) {
                *coins = roll(coins_type->amount, rnd) * coins_type->multiplier;
            }
        }
    }
}


static void
generate_treasure_gems(struct Treasure *treasure, struct rnd *rnd)
{
    treasure->gems = NULL;
    treasure->gemsCount = 0;
    if (treasure->type->gems.is_possible) {
        int percent_score = roll("1d100", rnd);
        if (percent_score <= treasure->type->gems.percent_chance) {
            treasure->gemsCount = roll(treasure->type->gems.amount, rnd);
            treasure->gems = calloc_or_die(treasure->gemsCount, sizeof(struct gem));
            for (int i = 0; i < treasure->gemsCount; ++i) {
                gem_initialize(&treasure->gems[i]);
                gem_generate(&treasure->gems[i], rnd);
            }
        }
    }
}


static void
generate_treasure_jewelry(struct Treasure *treasure, struct rnd *rnd)
{
    treasure->jewelry = NULL;
    treasure->jewelryCount = 0;
    if (treasure->type->jewelry.is_possible) {
        int percent_score = roll("1d100", rnd);
        if (percent_score <= treasure->type->jewelry.percent_chance) {
            treasure->jewelryCount = roll(treasure->type->jewelry.amount, rnd);
            treasure->jewelry = calloc_or_die(treasure->jewelryCount,
                                              sizeof(struct jewelry));
            for (int i = 0; i < treasure->jewelryCount; ++i) {
                jewelry_initialize(&treasure->jewelry[i]);
                jewelry_generate(&treasure->jewelry[i], rnd);
            }
        }
    }
}


static void
generate_treasure_maps_or_magic(struct Treasure *treasure, struct rnd *rnd)
{
    if (treasure->type->maps_or_magic.is_possible) {
        int percent_score = roll("1d100", rnd);
        if (percent_score <= treasure->type->maps_or_magic.percent_chance) {
            for (int i = 0; i < treasure->type->maps_or_magic.type_count; ++i) {
                struct maps_or_magic_type *type = &treasure->type->maps_or_magic.types[i];
                generate_treasure_maps_or_magic_type(type, treasure, rnd);
            }
        }
    }
}


static void
generate_treasure_maps_or_magic_type(struct maps_or_magic_type *type,
                                     struct Treasure *treasure,
                                     struct rnd *rnd)
{
    int amount = type->amount;
    if (str_not_empty(type->variable_amount)) {
        amount = roll(type->variable_amount, rnd);
    }
    
    int magic_items_count = 0;
    int maps_count = 0;
    for (int i = 0; i < amount; ++i) {
        if (type->is_map_possible) {
            bool is_magic_item_possible = type->possible_magic_items & ANY_MAGIC_ITEM;
            if (is_magic_item_possible) {
                (roll("1d100", rnd) <= 10) ? ++maps_count : ++magic_items_count;
            } else {
                ++maps_count;
            }
        } else {
            ++magic_items_count;
        }
    }
    
    if (maps_count) {
        generateMapsForTreasure(treasure, rnd, maps_count);
    }
    if (magic_items_count) {
        generateMagicItemsForTreasure(treasure, rnd, magic_items_count,
                                      type->possible_magic_items);
    }
}


static char const *
possible_maps_or_magic_name(bool is_map_possible,
                            PossibleMagicItems possible_magic_items)
{
    if (is_map_possible) {
        switch (possible_magic_items) {
            case NO_MAGIC_ITEM: return "map";
            case ANY_MAGIC_ITEM: return "any";
            case NON_WEAPON_MAGIC: return "map or non weapon magic";
            default: 
                fail("possible_magic_items = %0x", possible_magic_items);
                return "(map or magic)";
        }
    } else {
        switch (possible_magic_items) {
            case POTION: return "potion";
            case SCROLL: return "scroll";
            case RING: return "ring";
            case ROD_STAFF_WAND: return "rod/staff/wand";
            case MISC_MAGIC: return "misc magic";
            case ARMOR_SHIELD: return "armor/shield";
            case SWORD: return "sword";
            case MISC_WEAPON: return "misc weapon";
                
            case ANY_MAGIC_ITEM: return "any magic";
            case MAGIC_WEAPON_OR_ARMOR: return "sword, armor or misc weapon";
            default: 
                fail("possible_magic_items = %0x", possible_magic_items);
                return "(magic)";
        }
    }
}


struct treasure_type *
treasure_type_by_letter(char letter)
{
    assert(letter >= 'A' && letter <= 'Z');
    return &treasure_types[letter - 'A'];
}
