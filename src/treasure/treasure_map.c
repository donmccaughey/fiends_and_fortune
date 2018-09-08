#include "treasure_map.h"

#include <stdlib.h>
#include <string.h>
#include <background/background.h>
#include <base/base.h>
#include <mechanics/mechanics.h>

#include "gem.h"
#include "jewelry.h"


enum treasure_map_type {
    treasure_map_type_false_map,
    treasure_map_type_map_to_monetary_treasure,
    treasure_map_type_map_to_magic_treasure,
    treasure_map_type_map_to_combined_hoard
};


static char const *treasure_map_types[] = {
    "treasure",
    "monetary treasure",
    "magic treasure",
    "combined hoard"
};


static void
generate_combined_hoard(struct treasure *treasure, struct rnd *rnd);

static void
generate_magic_treasure(struct treasure *treasure, struct rnd *rnd);

static void
generate_magic_treasure_1_to_5_item_and_potions(struct treasure *treasure,
                                                struct rnd *rnd);

static void
generate_magic_treasure_6_to_8_two_items(struct treasure *treasure,
                                         struct rnd *rnd);

static void
generate_magic_treasure_9_to_12_sword_armor_and_weapon(struct treasure *treasure,
                                                       struct rnd *rnd);

static void
generate_magic_treasure_13_to_14_three_items(struct treasure *treasure,
                                             struct rnd *rnd);

static void
generate_magic_treasure_15_to_18_potions_and_scrolls(struct treasure *treasure,
                                                     struct rnd *rnd);

static void
generate_magic_treasure_19_four_items(struct treasure *treasure,
                                      struct rnd *rnd);

static void
generate_magic_treasure_20_five_items(struct treasure *treasure,
                                      struct rnd *rnd);

static void
generate_monetary_treasure(struct treasure *treasure, struct rnd *rnd);

static void
generate_monetary_treasure_1_to_2_copper_and_silver(struct treasure *treasure,
                                                    struct rnd *rnd);

static void
generate_monetary_treasure_3_to_5_electrum(struct treasure *treasure,
                                           struct rnd *rnd);

static void
generate_monetary_treasure_6_to_10_gold(struct treasure *treasure,
                                        struct rnd *rnd);

static void
generate_monetary_treasure_11_to_12_platinum(struct treasure *treasure,
                                             struct rnd *rnd);

static void
generate_monetary_treasure_13_to_15_gems(struct treasure *treasure,
                                         struct rnd *rnd);

static void
generate_monetary_treasure_16_to_17_jewelry(struct treasure *treasure,
                                            struct rnd *rnd);


void
treasure_map_finalize(struct treasure_map *treasure_map)
{
    treasure_finalize(&treasure_map->treasure);
    free_or_die(treasure_map->true_description);
}


static void
generate_combined_hoard(struct treasure *treasure, struct rnd *rnd)
{
    int score = roll("1d100", rnd);
    if (score <= 20) {
        generate_monetary_treasure_1_to_2_copper_and_silver(treasure, rnd);
        generate_magic_treasure_1_to_5_item_and_potions(treasure, rnd);
    } else if (score <= 40) {
        generate_monetary_treasure_6_to_10_gold(treasure, rnd);
        generate_magic_treasure_1_to_5_item_and_potions(treasure, rnd);
    } else if (score <= 55) {
        generate_monetary_treasure_3_to_5_electrum(treasure, rnd);
        generate_monetary_treasure_6_to_10_gold(treasure, rnd);
        generate_magic_treasure_1_to_5_item_and_potions(treasure, rnd);
        generate_magic_treasure_15_to_18_potions_and_scrolls(treasure, rnd);
    } else if (score <= 65) {
        generate_monetary_treasure_1_to_2_copper_and_silver(treasure, rnd);
        generate_monetary_treasure_3_to_5_electrum(treasure, rnd);
        generate_monetary_treasure_6_to_10_gold(treasure, rnd);
        generate_magic_treasure_9_to_12_sword_armor_and_weapon(treasure, rnd);
        generate_magic_treasure_13_to_14_three_items(treasure, rnd);
    } else if (score <= 75) {
        generate_monetary_treasure_6_to_10_gold(treasure, rnd);
        generate_monetary_treasure_11_to_12_platinum(treasure, rnd);
        generate_magic_treasure_6_to_8_two_items(treasure, rnd);
        generate_magic_treasure_15_to_18_potions_and_scrolls(treasure, rnd);
    } else if (score <= 80) {
        generate_monetary_treasure_3_to_5_electrum(treasure, rnd);
        generate_monetary_treasure_6_to_10_gold(treasure, rnd);
        generate_monetary_treasure_11_to_12_platinum(treasure, rnd);
        generate_monetary_treasure_16_to_17_jewelry(treasure, rnd);
        generate_magic_treasure_1_to_5_item_and_potions(treasure, rnd);
        generate_magic_treasure_9_to_12_sword_armor_and_weapon(treasure, rnd);
    } else if (score <= 85) {
        generate_monetary_treasure_1_to_2_copper_and_silver(treasure, rnd);
        generate_monetary_treasure_3_to_5_electrum(treasure, rnd);
        generate_monetary_treasure_6_to_10_gold(treasure, rnd);
        generate_monetary_treasure_11_to_12_platinum(treasure, rnd);
        generate_monetary_treasure_13_to_15_gems(treasure, rnd);
        generate_monetary_treasure_16_to_17_jewelry(treasure, rnd);
        /* TODO: map to generate_magic_treasure_1_to_5_item_and_potions() */
    } else if (score <= 90) {
        generate_monetary_treasure_1_to_2_copper_and_silver(treasure, rnd);
        generate_monetary_treasure_3_to_5_electrum(treasure, rnd);
        generate_monetary_treasure_6_to_10_gold(treasure, rnd);
        generate_monetary_treasure_11_to_12_platinum(treasure, rnd);
        generate_monetary_treasure_13_to_15_gems(treasure, rnd);
        generate_monetary_treasure_16_to_17_jewelry(treasure, rnd);
        /* TODO: map to generate_magic_treasure_19_four_items() */
    } else if (score <= 96) {
        /* TODO: map to generate_monetary_treasure_1_to_2_copper_and_silver(),
         generate_monetary_treasure_3_to_5_electrum() */
        generate_magic_treasure_20_five_items(treasure, rnd);
    } else {
        /* TODO: map to generate_monetary_treasure_11_to_12_platinum(),
         generate_monetary_treasure_13_to_15_gems() */
        generate_magic_treasure_15_to_18_potions_and_scrolls(treasure, rnd);
        generate_magic_treasure_20_five_items(treasure, rnd);
    }
}


static void
generate_magic_treasure(struct treasure *treasure, struct rnd *rnd)
{
    int score = roll("1d20", rnd);
    if (score <= 5) {
        generate_magic_treasure_1_to_5_item_and_potions(treasure, rnd);
    } else if (score <= 8) {
        generate_magic_treasure_6_to_8_two_items(treasure, rnd);
    } else if (score <= 12) {
        generate_magic_treasure_9_to_12_sword_armor_and_weapon(treasure, rnd);
    } else if (score <= 14) {
        generate_magic_treasure_13_to_14_three_items(treasure, rnd);
    } else if (score <= 18) {
        generate_magic_treasure_15_to_18_potions_and_scrolls(treasure, rnd);
    } else if (score <= 19) {
        generate_magic_treasure_19_four_items(treasure, rnd);
    } else {
        generate_magic_treasure_20_five_items(treasure, rnd);
    }
}


static void
generate_magic_treasure_1_to_5_item_and_potions(struct treasure *treasure,
                                                struct rnd *rnd)
{
    treasure_generate_magic_items(treasure, rnd, 1, ANY_MAGIC_ITEM);
    treasure_generate_magic_items(treasure, rnd, 4, POTION);
}


static void
generate_magic_treasure_6_to_8_two_items(struct treasure *treasure,
                                         struct rnd *rnd)
{
    treasure_generate_magic_items(treasure, rnd, 2, ANY_MAGIC_ITEM);
}


static void
generate_magic_treasure_9_to_12_sword_armor_and_weapon(struct treasure *treasure,
                                                       struct rnd *rnd)
{
    treasure_generate_magic_items(treasure, rnd, 1, SWORD);
    treasure_generate_magic_items(treasure, rnd, 1, ARMOR_SHIELD);
    treasure_generate_magic_items(treasure, rnd, 1, MISC_WEAPON);
}


static void
generate_magic_treasure_13_to_14_three_items(struct treasure *treasure,
                                             struct rnd *rnd)
{
    treasure_generate_magic_items(treasure, rnd, 3,
                                  (ANY_MAGIC_ITEM & ~SWORD & ~POTION));
}


static void
generate_magic_treasure_15_to_18_potions_and_scrolls(struct treasure *treasure,
                                                     struct rnd *rnd)
{
    treasure_generate_magic_items(treasure, rnd, 6, POTION);
    treasure_generate_magic_items(treasure, rnd, 6, SCROLL);
}


static void generate_magic_treasure_19_four_items(struct treasure *treasure,
                                                  struct rnd *rnd)
{
    treasure_generate_magic_items(treasure, rnd, 1, RING);
    treasure_generate_magic_items(treasure, rnd, 1, ROD_STAFF_WAND);
    treasure_generate_magic_items(treasure, rnd, 2, ANY_MAGIC_ITEM);
}


static void
generate_magic_treasure_20_five_items(struct treasure *treasure,
                                      struct rnd *rnd)
{
    treasure_generate_magic_items(treasure, rnd, 1, ROD_STAFF_WAND);
    treasure_generate_magic_items(treasure, rnd, 1, MISC_MAGIC);
    treasure_generate_magic_items(treasure, rnd, 3, ANY_MAGIC_ITEM);
}


static void
generate_monetary_treasure(struct treasure *treasure, struct rnd *rnd)
{
    int multiple_rolls = 0;
    int max_score = 20;
    do {
        int score = dice_roll(dice_make(1, max_score), rnd, NULL);
        if (score <= 2) {
            generate_monetary_treasure_1_to_2_copper_and_silver(treasure, rnd);
            --multiple_rolls;
        } else if (score <= 5) {
            generate_monetary_treasure_3_to_5_electrum(treasure, rnd);
            --multiple_rolls;
        } else if (score <= 10) {
            generate_monetary_treasure_6_to_10_gold(treasure, rnd);
            --multiple_rolls;
        } else if (score <= 12) {
            generate_monetary_treasure_11_to_12_platinum(treasure, rnd);
            --multiple_rolls;
        } else if (score <= 15) {
            generate_monetary_treasure_13_to_15_gems(treasure, rnd);
            --multiple_rolls;
        } else if (score <= 17) {
            generate_monetary_treasure_16_to_17_jewelry(treasure, rnd);
            --multiple_rolls;
        } else if (score <= 18) {
            max_score = 17;
            multiple_rolls = 2;
        } else if (score <= 19) {
            max_score = 17;
            multiple_rolls = 3;
        } else {
            generate_monetary_treasure_1_to_2_copper_and_silver(treasure, rnd);
            generate_monetary_treasure_3_to_5_electrum(treasure, rnd);
            generate_monetary_treasure_6_to_10_gold(treasure, rnd);
            generate_monetary_treasure_11_to_12_platinum(treasure, rnd);
            generate_monetary_treasure_13_to_15_gems(treasure, rnd);
            generate_monetary_treasure_16_to_17_jewelry(treasure, rnd);
        }
    } while (multiple_rolls > 0);
}


static void
generate_monetary_treasure_1_to_2_copper_and_silver(struct treasure *treasure,
                                                    struct rnd *rnd)
{
    treasure->coins.cp += roll("2d4", rnd) * 10000;
    treasure->coins.sp += (roll("1d4", rnd) + 1) * 10000;
}


static void
generate_monetary_treasure_3_to_5_electrum(struct treasure *treasure,
                                           struct rnd *rnd)
{
    treasure->coins.ep += roll("5d6", rnd) * 1000;
}


static void
generate_monetary_treasure_6_to_10_gold(struct treasure *treasure,
                                        struct rnd *rnd)
{
    treasure->coins.gp += roll("3d6", rnd) * 1000;
}


static void
generate_monetary_treasure_11_to_12_platinum(struct treasure *treasure,
                                             struct rnd *rnd)
{
    treasure->coins.pp += roll("5d4", rnd) * 100;
}


static void
generate_monetary_treasure_13_to_15_gems(struct treasure *treasure,
                                         struct rnd *rnd)
{
    int count = roll("1d10", rnd) * 10;
    treasure->gems = reallocarray_or_die(treasure->gems,
                                         treasure->gems_count + count,
                                         sizeof(struct gem));
    for (int i = 0; i < count; ++i) {
        int j = treasure->gems_count + i;
        gem_initialize(&treasure->gems[j]);
        gem_generate(&treasure->gems[j], rnd);
    }
    treasure->gems_count += count;
}


static void
generate_monetary_treasure_16_to_17_jewelry(struct treasure *treasure,
                                            struct rnd *rnd)
{
    int count = roll("5d10", rnd);
    treasure->jewelry = reallocarray_or_die(treasure->jewelry,
                                            treasure->jewelry_count + count,
                                            sizeof(struct jewelry));
    for (int i = 0; i < count; ++i) {
        int j = treasure->jewelry_count + i;
        jewelry_initialize(&treasure->jewelry[j]);
        jewelry_generate(&treasure->jewelry[j], rnd);
    }
    treasure->jewelry_count += count;
}


void
treasure_map_generate(struct treasure_map *treasure_map, struct rnd *rnd)
{
    enum treasure_map_type treasure_map_type;
    int score = roll("1d100", rnd);
    if (score <= 5) {
        treasure_map_type = treasure_map_type_false_map;
        treasure_map->is_false = true;
        /* TODO: generate description of false treasure */
    } else if (score <= 70) {
        treasure_map_type = treasure_map_type_map_to_monetary_treasure;
        generate_monetary_treasure(&treasure_map->treasure, rnd);
    } else if (score <= 90) {
        treasure_map_type = treasure_map_type_map_to_magic_treasure;
        generate_magic_treasure(&treasure_map->treasure, rnd);
    } else {
        treasure_map_type = treasure_map_type_map_to_combined_hoard;
        generate_combined_hoard(&treasure_map->treasure, rnd);
    }
    
    int miles;
    score = roll("1d100", rnd);
    if (score <= 20) {
        miles = 0;
    } else if (score <= 60) {
        miles = roll("1d4", rnd) + 4;
    } else if (score <= 90) {
        miles = roll("1d4", rnd) * 10;
    } else {
        miles = roll("1d10", rnd) * 50;
    }
    
    enum direction direction = direction_random(rnd);
    if (miles) {
        char const *disposition;
        score = roll("1d100", rnd);
        if (score <= 10) {
            disposition = "buried and unguarded";
        } else if (score <= 20) {
            disposition = "hidden in water";
        } else if (score <= 70) {
            disposition = "guarded in a lair";
        } else if (score <= 80) {
            disposition = "somewhere in a ruins";
        } else if (score <= 90) {
            disposition = "in a burial crypt";
        } else {
            disposition = "secreted in a town";
        }
        
        char *description = treasure_alloc_description(&treasure_map->treasure);
        treasure_map->true_description = str_alloc_formatted("%smap to %s of %s %i miles to the %s, %s",
                                                             (treasure_map->is_false ? "false " : ""),
                                                             treasure_map_types[treasure_map_type],
                                                             description,
                                                             miles,
                                                             direction_name(direction),
                                                             disposition);
        free_or_die(description);
    } else {
        char *description = treasure_alloc_description(&treasure_map->treasure);
        treasure_map->true_description = str_alloc_formatted("%smap to %s of %s in nearby labyrinth to the %s",
                                                             (treasure_map->is_false ? "false " : ""),
                                                             treasure_map_types[treasure_map_type],
                                                             description,
                                                             direction_name(direction));
        free_or_die(description);
    }
}


void
treasure_map_initialize(struct treasure_map *treasure_map)
{
    memset(treasure_map, 0, sizeof(struct treasure_map));
}
