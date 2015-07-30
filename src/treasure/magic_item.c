#include "magic_item.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "character/character.h"

#include "common/alloc_or_die.h"
#include "common/dice.h"
#include "common/fail.h"
#include "common/str.h"

#include "coins.h"


typedef void(*generate_function)(struct magic_item *magic_item, struct rnd *rnd);


struct misc_magic_item {
    int percent;
    char const *name;
    int experience_points;
    int sale_value_in_gp;
    class_restrictions_t class_restrictions;
    generate_function generate;
};


static void
generate_armor_or_shield(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_artifact_or_relic(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_bracers_of_defense(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_bucknards_everfull_purse(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_cloak_of_protection(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_crystal_ball(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_eyes_of_petrification(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_figurine_of_wondrous_power(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_girdle_of_giant_strength(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_horn_of_valhalla(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_ioun_stones(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_instrument_of_the_bards(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_jewel_of_flawlessness(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_medallion_of_ESP(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_misc_magic_item_table1(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_misc_magic_item_table2(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_misc_magic_item_table3(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_misc_magic_item_table4(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_misc_magic_item_table5(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_misc_weapon(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_necklace_of_missiles(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_necklace_of_prayer_beads(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_nolzurs_marvelous_pigments(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_orb_of_dragonkind(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_pearl_of_power(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_potion(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_potion_of_dragon_control(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_potion_of_giant_control(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_potion_of_giant_strength(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_quaals_feather_token(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_ring(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_ring_of_protection(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_rod_staff_or_wand(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_scroll(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_sword(struct magic_item *magic_item, struct rnd *rnd);

static void
generate_teeth_of_dahlver_nar(struct magic_item *magic_item, struct rnd *rnd);


void
magic_item_finalize(struct magic_item *magic_item)
{
    if (magic_item->true_details) {
        int i = 0;
        while (magic_item->true_details[i]) {
            free_or_die(magic_item->true_details[i]);
            ++i;
        }
        free_or_die(magic_item->true_details);
    }
    free_or_die(magic_item->true_description);
}


static void
generate_armor_or_shield(struct magic_item *magic_item, struct rnd *rnd)
{
    static struct armor_or_shield {
        int percent;
        char const *name;
        bool is_armor;
        int experience_points;
        int sale_value_in_gp;
    } const armor_and_shield_table[] = {
        {   5 -  0, "chain mail +1", true, 600, 3500 },
        {   9 -  5, "chain mail +2", true, 1200, 7500 },
        {  11 -  9, "chain mail +3", true, 2000, 12500 },
        {  19 - 11, "leather armor +1", true, 300, 2000 },
        {  26 - 19, "plate mail +1", true, 800, 5000 },
        {  32 - 26, "plate mail +2", true, 1750, 10500 },
        {  35 - 32, "plate mail +3", true, 2750, 15500 },
        {  37 - 35, "plate mail +4", true, 3500, 20500 },
        {  38 - 37, "plate mail +5", true, 4500, 27500 },
        {  39 - 38, "plate mail of etherealness", true, 5000, 30000 },
        {  44 - 39, "plate mail of vulnerability", true, 0, 1500 },
        {  50 - 44, "ring mail +1", true, 400, 2500 },
        {  55 - 50, "scale mail +1", true, 500, 3000 },
        {  59 - 55, "scale mail +2", true, 1100, 6750 },
        {  63 - 59, "splint mail +1", true, 700, 4000 },
        {  66 - 63, "splint mail +2", true, 1500, 8500 },
        {  68 - 66, "splint mail +3", true, 2250, 14500 },
        {  69 - 68, "splint mail +4", true, 3000, 19000 },
        {  75 - 69, "studded leather +1", true, 400, 2500 },
        {  84 - 75, "shield +1", false, 250, 25000 },
        {  89 - 84, "shield +2", false, 500, 5000 },
        {  93 - 89, "shield +3", false, 800, 8000 },
        {  95 - 93, "shield +4", false, 1200, 12000 },
        {  96 - 95, "shield +5", false, 1750, 17500 },
        {  97 - 96, "shield, large, +1, +4 vs missiles", false, 400, 4000 },
        { 100 - 97, "shield -1, missile attractor", false, 0, 750 }
    };
    static size_t const armor_and_shield_table_count = sizeof armor_and_shield_table
                                                     / sizeof armor_and_shield_table[0];
    
    int score = roll("1d100", rnd);
    int range = 0;
    struct armor_or_shield const *armor_or_shield = NULL;
    for (int i = 0; i < armor_and_shield_table_count; ++i) {
        range += armor_and_shield_table[i].percent;
        if (score <= range) {
            armor_or_shield = &armor_and_shield_table[i];
            break;
        }
    }
    assert(armor_or_shield);
    
    magic_item->experience_points = armor_or_shield->experience_points;
    magic_item->true_value_in_cp = gp_to_cp(armor_or_shield->sale_value_in_gp);
    
    if (armor_or_shield->is_armor) {
        char const *armor_size;
        score = roll("1d100", rnd);
        if (score <= 65) {
            armor_size = "human sized";
        } else if (score <= 85) {
            armor_size = "elf sized";
        } else if (score <= 95) {
            armor_size = "dwarf sized";
        } else {
            armor_size = "gnome or halfling sized";
        }
        magic_item->true_description = str_alloc_formatted("%s (%s)",
                                                           armor_or_shield->name,
                                                           armor_size);
    } else {
        magic_item->true_description = str_alloc_formatted("%s",
                                                           armor_or_shield->name);
    }
}


static void
generate_artifact_or_relic(struct magic_item *magic_item, struct rnd *rnd)
{
    static struct artifact_or_relic {
        int percent;
        char const *name;
        int sale_value_in_gp;
        generate_function generate;
    } const artifacts_and_relics_table[] = {
        {   1 -  0, "axe of the dwarvish lords", 55000 },
        {   2 -  1, "Baba Yaga's hut", 90000 },
        {   4 -  2, "codex of the infinite planes", 62500 },
        {  20 -  4, "crown of might", 50000 },
        {  21 - 20, "crystal of the ebon flame", 75000 },
        {  22 - 21, "cup and talisman of Al'Akbar", 85000 },
        {  24 - 22, "eye of Vecna", 35000 },
        {  25 - 24, "hand of Vecna", 60000 },
        {  26 - 25, "Heward's mystical organ", 25000 },
        {  27 - 26, "horn of change", 20000 },
        {  29 - 27, "invulnerable coat of Arnd", 47500 },
        {  31 - 29, "iron flask of Tuerny the Merciless", 50000 },
        {  32 - 31, "jacinth of inestimable beauty", 100000 },
        {  33 - 32, "Johydee's mask", 40000 },
        {  35 - 33, "Kuroth's quill", 27500 },
        {  37 - 35, "mace of Cuthbert", 35000 },
        {  38 - 37, "machine of Lum the Mad", 72500 },
        {  40 - 38, "mighty servant of Leuk-O", 185000 },
        {  47 - 40, "orb of the dragonkind", 10000, generate_orb_of_dragonkind },
        {  63 - 47, "orb of might", 100000 },
        {  64 - 63, "Queen Ehlissa's marvelous nightengale", 112500 },
        {  66 - 64, "recorder of Ye'Cind", 80000 },
        {  68 - 66, "ring of Gaxx", 17500 },
        {  74 - 68, "rod of seven parts", 25000 },
        {  91 - 74, "sceptre of might", 150000 },
        {  92 - 91, "sword of Kas", 97000 },
        {  98 - 92, "teeth of Dahlver-Nar", 5000, generate_teeth_of_dahlver_nar },
        {  99 - 98, "throne of the gods", 0 },
        { 100 - 99, "wand of Orcus", 10000 }
    };
    size_t const artifacts_and_relics_table_count = sizeof artifacts_and_relics_table
                                                  / sizeof artifacts_and_relics_table[0];
    
    int score = roll("1d100", rnd);
    int range = 0;
    struct artifact_or_relic const *artifactOrRelic = NULL;
    for (int i = 0; i < artifacts_and_relics_table_count; ++i) {
        range += artifacts_and_relics_table[i].percent;
        if (score <= range) {
            artifactOrRelic = &artifacts_and_relics_table[i];
            break;
        }
    }
    assert(artifactOrRelic);
    
    if (artifactOrRelic->generate) {
        artifactOrRelic->generate(magic_item, rnd);
    } else {
        magic_item->experience_points = 0;
        magic_item->true_value_in_cp = gp_to_cp(artifactOrRelic->sale_value_in_gp);
        magic_item->true_description = str_alloc_formatted("%s",
                                                           artifactOrRelic->name);
    }
}


static void
generate_bracers_of_defense(struct magic_item *magic_item, struct rnd *rnd)
{
    int armor_class = 0;
    
    int score = roll("1d100", rnd);
    if (score <= 5) {
        armor_class = 8;
    } else if (score <= 15) {
        armor_class = 7;
    } else if (score <= 35) {
        armor_class = 6;
    } else if (score <= 50) {
        armor_class = 5;
    } else if (score <= 70) {
        armor_class = 4;
    } else if (score <= 85) {
        armor_class = 3;
    } else {
        armor_class = 2;
    }
    
    int level = 10 - armor_class;
    magic_item->experience_points = level * 500;
    magic_item->true_value_in_cp = gp_to_cp(level * 3000);
    magic_item->true_description = str_alloc_formatted("bracers of defense AC %i",
                                                       armor_class);
}


static void
generate_bucknards_everfull_purse(struct magic_item *magic_item,
                                  struct rnd *rnd)
{
    int type = 0;
    
    int score = roll("1d100", rnd);
    if (score <= 50) {
        type = 1;
        magic_item->experience_points = 1500;
        magic_item->true_value_in_cp = gp_to_cp(15000);
    } else if (score <= 90) {
        type = 2;
        magic_item->experience_points = 2500;
        magic_item->true_value_in_cp = gp_to_cp(25000);
    } else {
        type = 3;
        magic_item->experience_points = 4000;
        magic_item->true_value_in_cp = gp_to_cp(40000);
    }
    
    magic_item->true_description = str_alloc_formatted("Bucknards everfull purse (type %i)",
                                                       type);
}


static void
generate_cloak_of_protection(struct magic_item *magic_item, struct rnd *rnd)
{
    int plus = 0;
    
    int score = roll("1d100", rnd);
    if (score <= 35) {
        plus = 1;
    } else if (score <= 65) {
        plus = 2;
    } else if (score <= 85) {
        plus = 3;
    } else if (score <= 95) {
        plus = 4;
    } else {
        plus = 5;
    }
    
    magic_item->experience_points = 1000 * plus;
    magic_item->true_value_in_cp = gp_to_cp(10000 * plus);
    magic_item->true_description = str_alloc_formatted("cloak of protection +%i",
                                                       plus);
}


static void
generate_crystal_ball(struct magic_item *magic_item, struct rnd *rnd)
{
    char const *feature = "";
    
    int dieRoll = roll("1d100", rnd);
    if (dieRoll <= 50) {
        magic_item->experience_points = 1000;
        magic_item->true_value_in_cp = gp_to_cp(5000);
    } else if (dieRoll <= 75) {
        feature = " with clairaudience";
        magic_item->experience_points = 2000;
        magic_item->true_value_in_cp = gp_to_cp(10000);
    } else if (dieRoll <= 90) {
        feature = " with ESP";
        magic_item->experience_points = 2000;
        magic_item->true_value_in_cp = gp_to_cp(10000);
    } else {
        feature = " with telepathy (communication only)";
        magic_item->experience_points = 2000;
        magic_item->true_value_in_cp = gp_to_cp(10000);
    }
    
    magic_item->true_description = str_alloc_formatted("crystal ball%s", feature);
}


static void
generate_eyes_of_petrification(struct magic_item *magic_item, struct rnd *rnd)
{
    char const *type;
    
    int score = roll("1d100", rnd);
    if (score <= 25) {
        type = "others";
        magic_item->experience_points = 12500;
        magic_item->true_value_in_cp = gp_to_cp(50000);
    } else {
        type = "self";
        magic_item->experience_points = 0;
        magic_item->true_value_in_cp = 0;
    }
    
    magic_item->true_description = str_alloc_formatted("eyes of petrification (%s)",
                                                       type);
}


static void
generate_figurine_of_wondrous_power(struct magic_item *magic_item,
                                    struct rnd *rnd)
{
    int hit_dice;
    char *type;
    
    int score = roll("1d100", rnd);
    if (score <= 15) {
        hit_dice = 4;
        type = str_alloc_formatted("ebony fly (%i hp)", roll("4d8+4", rnd));
    } else if (score <= 30) {
        type = str_alloc_formatted("two golden lions (%i/%i hp)",
                                   roll("5d8+2", rnd), roll("5d8+2", rnd));
        hit_dice = 10;
    } else if (score <= 40) {
        type = str_alloc_formatted("three ivory goats (24/96/48 hp)");
        hit_dice = 4 + 16 + 8;
    } else if (score <= 55) {
        score = roll("1d100", rnd);
        if (score <= 50) {
            hit_dice = 10;
            type = str_alloc_formatted("marble elephant, asiatic (%i hp)",
                                       roll("10d8", rnd));
        } else if (score <= 90) {
            hit_dice = 11;
            type = str_alloc_formatted("marble elephant, african (%i hp)",
                                       roll("11d8", rnd));
        } else if (score <= 93) {
            hit_dice = 13;
            type = str_alloc_formatted("marble elephant, mammoth (%i hp)",
                                       roll("13d8", rnd));
        } else {
            hit_dice = 12;
            type = str_alloc_formatted("marble elephant, mastodon (%i hp)",
                                       roll("12d8", rnd));
        }
    } else if (score <= 65) {
        hit_dice = 6;
        type = str_alloc_formatted("obsidian steed (%i hp)",
                                   roll("6d8+6", rnd));
    } else if (score <= 85) {
        hit_dice = 2;
        type = str_alloc_formatted("onyx dog (%i hp)", roll("2d8+2", rnd));
    } else {
        hit_dice = 4;
        type = str_alloc_formatted("serpentine owl (%i hp)", roll("4d8", rnd));
    }
    
    magic_item->experience_points = hit_dice * 100;
    magic_item->true_value_in_cp = gp_to_cp(hit_dice * 1000);
    magic_item->true_description = str_alloc_formatted("figurine of wondrous power: %s",
                                                       type);
    free_or_die(type);
}


static void
generate_girdle_of_giant_strength(struct magic_item *magic_item,
                                  struct rnd *rnd)
{
    char const *type;
    
    int score = roll("1d100", rnd);
    if (score <= 30) {
        type = "hill";
    } else if (score <= 50) {
        type ="stone";
    } else if (score <= 70) {
        type ="frost";
    } else if (score <= 85) {
        type ="fire";
    } else if (score <= 95) {
        type ="cloud";
    } else {
        type ="storm";
    }
    
    magic_item->experience_points = 200;
    magic_item->true_value_in_cp = gp_to_cp(2500);
    magic_item->true_description = str_alloc_formatted("girdle of %s giant strength",
                                                     type);
}


static void generate_horn_of_valhalla(struct magic_item *magic_item,
                                      struct rnd *rnd)
{
    char const *type;
    int multiplier;
    
    int score = roll("1d20", rnd);
    if (score <= 8) {
        type = "silver";
        multiplier = 1;
    } else if (score <= 15) {
        type = "brass";
        multiplier = 1;
    } else if (score <= 18) {
        type = "bronze";
        multiplier = 2;
    } else {
        type = "iron";
        multiplier = 3;
    }
    
    magic_item->experience_points = 1000 * multiplier;
    magic_item->true_value_in_cp = gp_to_cp(15000 * multiplier);
    
    char const *alignment = NULL;
    score = roll("1d100", rnd);
    if (score <= 50) {
        score = roll("1d8", rnd);
        switch (score) {
            case 1: alignment = "good"; break;
            case 2: alignment = "lawful good"; break;
            case 3: alignment = "lawful"; break;
            case 4: alignment = "lawful evil"; break;
            case 5: alignment = "evil"; break;
            case 6: alignment = "chaotic evil"; break;
            case 7: alignment = "chaotic"; break;
            case 8: alignment = "chaotic good"; break;
            default: fail("1d8 die roll is %i", score); break;
        }
        magic_item->true_description = str_alloc_formatted("%s horn of Valhalla (%s)",
                                                           type, alignment);
    } else {
        magic_item->true_description = str_alloc_formatted("%s horn of Valhalla",
                                                           type);
    }
}


static void
generate_ioun_stones(struct magic_item *magic_item, struct rnd *rnd)
{
    int quantity = roll("1d10", rnd);
    /* TODO: generate types of stones */
    
    magic_item->experience_points = quantity * 300;
    magic_item->true_value_in_cp = gp_to_cp(quantity * 5000);
    
    char const *plural = (quantity == 1) ? "" : "s";
    magic_item->true_description = str_alloc_formatted("(%i) ioun stone%s",
                                                       quantity, plural);
}


static void
generate_instrument_of_the_bards(struct magic_item *magic_item, struct rnd *rnd)
{
    char const *name;
    int level;
    
    int score = roll("1d20", rnd);
    if (score <= 5) {
        name = "Fochlucan Bandore";
        level = 1;
    } else if (score <= 9) {
        name = "Mac-Fuirmidh Cittern";
        level = 5;
    } else if (score <= 12) {
        name = "Doss Lute";
        level = 8;
    } else if (score <= 15) {
        name = "Canaith Mandolin";
        level = 11;
    } else if (score <= 17) {
        name = "Cli Lyre";
        level = 14;
    } else if (score <= 19) {
        name = "Anstruth Harp";
        level = 17;
    } else {
        name = "Ollamh Harp";
        level = 20;
    }
    
    magic_item->experience_points = level * 1000;
    magic_item->true_value_in_cp = gp_to_cp(level * 5000);
    magic_item->true_description = str_alloc_formatted("instrument of the bards: %s",
                                                       name);
}


static void
generate_jewel_of_flawlessness(struct magic_item *magic_item, struct rnd *rnd)
{
    int facets = roll("10d10", rnd);
    
    magic_item->experience_points = 0;
    magic_item->true_value_in_cp = gp_to_cp(facets * 1000);
    
    char const *plural = (facets == 1) ? "" : "s";
    magic_item->true_description = str_alloc_formatted("jewel of flawlessness with %i facet%s",
                                                       facets, plural);
}


void magic_item_generate(struct magic_item *magic_item,
                         struct rnd *rnd,
                         possible_magic_items_t possible_magic_items)
{
    struct {
        int percent;
        possible_magic_items_t possible_magic_items;
        enum magic_item_type type;
        generate_function generate;
    } const magic_items_table[] = {
        {  20 -  0, POTION, magic_item_type_potion, generate_potion },
        {  35 - 20, SCROLL, magic_item_type_scroll, generate_scroll },
        {  40 - 35, RING, magic_item_type_ring, generate_ring },
        {  45 - 40, ROD_STAFF_WAND, magic_item_type_rod_staff_or_wand, generate_rod_staff_or_wand },
        {  48 - 45, MISC_MAGIC, magic_item_type_misc, generate_misc_magic_item_table1 },
        {  51 - 48, MISC_MAGIC, magic_item_type_misc, generate_misc_magic_item_table2 },
        {  54 - 51, MISC_MAGIC, magic_item_type_misc, generate_misc_magic_item_table3 },
        {  57 - 54, MISC_MAGIC, magic_item_type_misc, generate_misc_magic_item_table4 },
        {  60 - 57, MISC_MAGIC, magic_item_type_misc, generate_misc_magic_item_table5 },
        {  75 - 60, ARMOR_SHIELD, magic_item_type_armor, generate_armor_or_shield },
        {  86 - 75, SWORD, magic_item_type_sword, generate_sword },
        { 100 - 86, MISC_WEAPON, magic_item_type_misc_weapon, generate_misc_weapon }
    };
    size_t const magic_items_table_count = sizeof magic_items_table
                                         / sizeof magic_items_table[0];
    
    int total = 0;
    for (int i = 0; i < magic_items_table_count; ++i) {
        if (possible_magic_items & magic_items_table[i].possible_magic_items) {
            total += magic_items_table[i].percent;
        }
    }
    
    int score = dice_roll(dice_make(1, total), rnd, NULL);
    int range = 0;
    for (int i = 0; i < magic_items_table_count; ++i) {
        if (possible_magic_items & magic_items_table[i].possible_magic_items) {
            range += magic_items_table[i].percent;
            if (score <= range) {
                magic_item->type = magic_items_table[i].type;
                magic_items_table[i].generate(magic_item, rnd);
                return;
            }
        }
    }
    fail("Did not match table entry (score=%i, range=%i)", score, range);
}


static void
generate_medallion_of_ESP(struct magic_item *magic_item, struct rnd *rnd)
{
    char const *type;
    
    int score = roll("1d20", rnd);
    if (15 <= score) {
        type = "30 ft range";
        magic_item->experience_points = 1000;
        magic_item->true_value_in_cp = gp_to_cp(10000);
    } else if (18 <= score) {
        type = "30 ft range with empathy";
        magic_item->experience_points = 1000;
        magic_item->true_value_in_cp = gp_to_cp(10000);
    } else if (19 <= score) {
        type = "60 ft range";
        magic_item->experience_points = 3000;
        magic_item->true_value_in_cp = gp_to_cp(30000);
    } else {
        type = "90 ft range";
        magic_item->experience_points = 3000;
        magic_item->true_value_in_cp = gp_to_cp(30000);
    }
    
    magic_item->true_description = str_alloc_formatted("medallion of ESP, %s",
                                                       type);
}


static void
generate_misc_magic_item_table1(struct magic_item *magic_item, struct rnd *rnd)
{
    static struct misc_magic_item const misc_magic_items_table[] = {
        {   2 -  0, "alchemy jug", 3000, 12000 },
        {   4 -  2, "amulet of inescapable location", 0, 1000 },
        {   5 -  4, "amulet of life protection", 5000, 20000 },
        {   7 -  5, "amulet of the planes", 6000, 30000 },
        {  11 -  7, "amulet of proof against detection and location", 4000, 15000 },
        {  13 - 11, "apparatus of kwalish", 8000, 35000 },
        {  16 - 13, "arrow of direction", 2500, 17500 },
        {  17 - 16, "artifact or relic", 0, 0, NO_CLASS_RESTRICTIONS, generate_artifact_or_relic },
        {  20 - 17, "bag of beans", 1000, 5000 },
        {  21 - 20, "bag of devouring", 0, 1500 },
        {  26 - 21, "bag of holding", 5000, 25000 },
        {  27 - 26, "bag of transmuting", 0, 500 },
        {  29 - 27, "bag of tricks", 2500, 15000 },
        {  31 - 29, "beaker of plentiful potions", 1500, 12500 },
        {  32 - 31, "folding boat", 10000, 25000 },
        {  33 - 32, "book of exalted deeds", 8000, 40000, CLERICS },
        {  34 - 33, "book of infinite spells", 9000, 50000 },
        {  35 - 34, "book of vile darkness", 8000, 40000, CLERICS },
        {  36 - 35, "boots of dancing", 0, 5000 },
        {  42 - 36, "boots of elvenkind", 1000, 5000 },
        {  47 - 42, "boots of levitation", 2000, 15000 },
        {  51 - 47, "boots of speed", 2500, 20000 },
        {  55 - 51, "boots of striding and springing", 2500, 20000 },
        {  58 - 55, "bowl commanding water elementals", 4000, 25000, MAGIC_USERS },
        {  59 - 58, "bowl of watery death", 0, 1000 },
        {  79 - 59, "bracers of defense", 500, 3000, NO_CLASS_RESTRICTIONS, generate_bracers_of_defense },
        {  81 - 79, "bracers of defenselessness", 0, 2000 },
        {  84 - 81, "brazier commanding fire elementals", 4000, 25000, MAGIC_USERS },
        {  85 - 84, "brazier of sleep smoke", 0, 1000, MAGIC_USERS },
        {  92 - 85, "brooch of shielding", 1000, 10000 },
        {  93 - 92, "broom of animated attack", 0, 3000 },
        {  98 - 93, "broom of flying", 2000, 10000 },
        { 100 - 98, "Bucknards everfull purse", 1500, 15000, NO_CLASS_RESTRICTIONS, generate_bucknards_everfull_purse }
    };
    size_t const misc_magic_items_table_count = sizeof misc_magic_items_table
                                              / sizeof misc_magic_items_table[0];
    
    int score = roll("1d100", rnd);
    int range = 0;
    struct misc_magic_item const *misc_magic_item = NULL;
    for (int i = 0; i < misc_magic_items_table_count; ++i) {
        range += misc_magic_items_table[i].percent;
        if (score <= range) {
            misc_magic_item = &misc_magic_items_table[i];
            break;
        }
    }
    assert(misc_magic_item);
    
    if (misc_magic_item->generate) {
        misc_magic_item->generate(magic_item, rnd);
    } else {
        magic_item->experience_points = misc_magic_item->experience_points;
        magic_item->true_value_in_cp = gp_to_cp(misc_magic_item->sale_value_in_gp);
        magic_item->true_description = str_alloc_formatted("%s",
                                                           misc_magic_item->name);
    }
}


static void
generate_misc_magic_item_table2(struct magic_item *magic_item, struct rnd *rnd)
{
    static struct misc_magic_item const misc_magic_items_table[] = {
        {   6 -  0, "candle of invocation", 1000, 5000, CLERICS },
        {   8 -  6, "carpet of flying", 7500, 25000 },
        {  10 -  8, "censer controlling air elementals", 4000, 25000, MAGIC_USERS },
        {  11 - 10, "censer of summoning hostile air elementals", 0, 1000, MAGIC_USERS },
        {  13 - 11, "chime of opening", 3500, 20000 },
        {  14 - 13, "chime of hunger", 0, 0 },
        {  18 - 14, "cloak of displacement", 3000, 17500 },
        {  27 - 18, "cloak of elvenkind", 1000, 6000 },
        {  30 - 27, "cloak of manta ray", 2000, 12500 },
        {  32 - 30, "cloak of poisonousness", 0, 2500 },
        {  55 - 32, "cloak of protection", 1000, 10000, NO_CLASS_RESTRICTIONS, generate_cloak_of_protection },
        {  60 - 55, "crystal ball", 1000, 5000, MAGIC_USERS, generate_crystal_ball },
        {  61 - 60, "crystal hypnosis ball", 0, 3000, MAGIC_USERS },
        {  63 - 61, "cube of force", 3000, 20000 },
        {  65 - 63, "cube of frost resistance", 2000, 14000 },
        {  67 - 65, "cubic gate", 5000, 17500 },
        {  69 - 67, "Daern's instant fortress", 7000, 27500 },
        {  72 - 69, "decanter of endless water", 1000, 3000 },
        {  76 - 72, "deck of many things", 0, 10000 },
        {  77 - 76, "drums of deafening", 0, 500 },
        {  79 - 77, "drums of panic", 6500, 35000 },
        {  85 - 79, "dust of appearance", 1000, 4000 },
        {  91 - 85, "dust of disappearance", 2000, 8000 },
        {  92 - 91, "dust of sneezing and choking", 0, 1000 },
        {  93 - 92, "efreeti bottle", 9000, 45000 },
        {  94 - 93, "eversmoking bottle", 500, 2500 },
        {  95 - 94, "eyes of charming", 4000, 24000, MAGIC_USERS },
        {  97 - 95, "eyes of the eagle", 3500, 18000 },
        {  99 - 97, "eyes of minute seeing", 2000, 12500 },
        { 100 - 99, "eyes of petrification", 0, 0, NO_CLASS_RESTRICTIONS, generate_eyes_of_petrification }
    };
    size_t const misc_magic_items_table_count = sizeof misc_magic_items_table
                                              / sizeof misc_magic_items_table[0];
    
    int score = roll("1d100", rnd);
    int range = 0;
    struct misc_magic_item const *misc_magic_item = NULL;
    for (int i = 0; i < misc_magic_items_table_count; ++i) {
        range += misc_magic_items_table[i].percent;
        if (score <= range) {
            misc_magic_item = &misc_magic_items_table[i];
            break;
        }
    }
    assert(misc_magic_item);
    
    if (misc_magic_item->generate) {
        misc_magic_item->generate(magic_item, rnd);
    } else {
        magic_item->experience_points = misc_magic_item->experience_points;
        magic_item->true_value_in_cp = gp_to_cp(misc_magic_item->sale_value_in_gp);
        magic_item->true_description = str_alloc_formatted("%s",
                                                           misc_magic_item->name);
    }
}


static void
generate_misc_magic_item_table3(struct magic_item *magic_item, struct rnd *rnd)
{
    static struct misc_magic_item const misc_magic_items_table[] = {
        {  15 -  0, "figurine of wondrous power", 100, 1000, NO_CLASS_RESTRICTIONS, generate_figurine_of_wondrous_power },
        {  16 - 15, "flask of curses", 0, 1000 },
        {  18 - 16, "gauntlets of dexterity", 1000, 10000 },
        {  20 - 18, "gauntlets of fumbling", 0, 1000 },
        {  22 - 20, "gauntlets of ogre power", 1000, 15000, CLERICS | FIGHTERS | THIEVES },
        {  25 - 22, "gauntlets of swimming and climbing", 1000, 10000, CLERICS | FIGHTERS | THIEVES },
        {  26 - 25, "gem of brightness", 2000, 17500 },
        {  27 - 26, "gem of seeing", 2000, 25000 },
        {  28 - 27, "girdle of femininity/masculinity", 0, 1000, CLERICS | FIGHTERS | THIEVES },
        {  29 - 28, "girdle of giant strength", 200, 2500, CLERICS | FIGHTERS | THIEVES, generate_girdle_of_giant_strength },
        {  30 - 29, "helm of brilliance", 2500, 60000 },
        {  35 - 30, "helm of comprehending languages & reading magic", 1000, 12500 },
        {  37 - 35, "helm of opposite alignment", 0, 1000 },
        {  39 - 37, "helm of telepathy", 3000, 35000 },
        {  40 - 39, "helm of teleportation", 2500, 30000 },
        {  45 - 40, "helm of underwater action", 1000, 10000 },
        {  46 - 45, "horn of blasting", 5000, 55000 },
        {  48 - 46, "horn of bubbles", 0, 0 },
        {  49 - 48, "horn of collapsing", 1500, 25000 },
        {  53 - 49, "horn of the tritons", 2000, 17500, CLERICS | FIGHTERS },
        {  60 - 53, "horn of Valhalla", 1000, 15000, NO_CLASS_RESTRICTIONS, generate_horn_of_valhalla },
        {  63 - 60, "horseshoes of speed", 2000, 10000 },
        {  65 - 63, "horseshoes of a zephyr", 1500, 7500 },
        {  70 - 65, "incense of meditation", 500, 7500, CLERICS },
        {  71 - 70, "incense of obsession", 0, 500, CLERICS },
        {  72 - 71, "ioun stones", 300, 5000, NO_CLASS_RESTRICTIONS, generate_ioun_stones },
        {  78 - 72, "instrument of the bards", 1000, 5000, NO_CLASS_RESTRICTIONS, generate_instrument_of_the_bards },
        {  80 - 78, "iron flask", 0, 0 },
        {  85 - 80, "javelin of lightning", 250, 3000, FIGHTERS },
        {  90 - 85, "javelin of piercing", 250, 3000, FIGHTERS },
        {  91 - 90, "jewel of attacks", 0, 1000 },
        {  92 - 91, "jewel of flawlessness", 0, 1000, NO_CLASS_RESTRICTIONS, generate_jewel_of_flawlessness },
        { 100 - 92, "Keoghtom's ointment", 500, 10000 }
    };
    size_t const misc_magic_items_table_count = sizeof misc_magic_items_table
                                              / sizeof misc_magic_items_table[0];
    
    int score = roll("1d100", rnd);
    int range = 0;
    struct misc_magic_item const *misc_magic_item = NULL;
    for (int i = 0; i < misc_magic_items_table_count; ++i) {
        range += misc_magic_items_table[i].percent;
        if (score <= range) {
            misc_magic_item = &misc_magic_items_table[i];
            break;
        }
    }
    assert(misc_magic_item);
    
    if (misc_magic_item->generate) {
        misc_magic_item->generate(magic_item, rnd);
    } else {
        magic_item->experience_points = misc_magic_item->experience_points;
        magic_item->true_value_in_cp = gp_to_cp(misc_magic_item->sale_value_in_gp);
        magic_item->true_description = str_alloc_formatted("%s",
                                                           misc_magic_item->name);
    }
}


static void
generate_misc_magic_item_table4(struct magic_item *magic_item, struct rnd *rnd)
{
    static struct misc_magic_item const misc_magic_items_table[] = {
        {   1 -  0, "libram of gainful conjuration", 8000, 40000, MAGIC_USERS },
        {   2 -  1, "libram of ineffable damnation", 8000, 40000, MAGIC_USERS },
        {   3 -  2, "libram of silver magic", 8000, 40000, MAGIC_USERS },
        {   4 -  3, "lyre of building", 5000, 30000 },
        {   5 -  4, "manual of bodily health", 5000, 50000 },
        {   6 -  5, "manual of gainful exercise", 5000, 50000 },
        {   7 -  6, "manual of golems", 3000, 30000, CLERICS | MAGIC_USERS },
        {   8 -  7, "manual of puissant skill at arms", 8000, 40000, FIGHTERS },
        {   9 -  8, "manual of quickness of action", 5000, 50000 },
        {  10 -  9, "manual of stealthy pilfering", 8000, 40000, THIEVES },
        {  11 - 10, "mattock of the titans", 3500, 7000, FIGHTERS },
        {  12 - 11, "maul of the titans", 4000, 12000 },
        {  15 - 12, "medallion of ESP", 1000, 10000, NO_CLASS_RESTRICTIONS, generate_medallion_of_ESP },
        {  17 - 15, "medallion of thought projection", 0, 1000 },
        {  18 - 17, "mirror of life trapping", 2500, 25000, MAGIC_USERS },
        {  19 - 18, "mirror of mental prowess", 5000, 50000 },
        {  20 - 19, "mirror of opposition", 0, 2000 },
        {  23 - 20, "necklace of adaptation", 1000, 10000 },
        {  27 - 23, "necklace of missiles", 50, 200, NO_CLASS_RESTRICTIONS, generate_necklace_of_missiles },
        {  33 - 27, "necklace of prayer beads", 500, 3000, CLERICS, generate_necklace_of_prayer_beads },
        {  35 - 33, "necklace of strangulation", 0, 1000 },
        {  38 - 35, "net of entrapment", 1000, 7500, CLERICS | FIGHTERS | THIEVES },
        {  42 - 38, "net of snaring", 1000, 6000, CLERICS | FIGHTERS | THIEVES },
        {  44 - 42, "Nolzurs' marvelous pigments", 500, 3000, NO_CLASS_RESTRICTIONS, generate_nolzurs_marvelous_pigments },
        {  46 - 44, "pearl of power", 200, 2000, MAGIC_USERS, generate_pearl_of_power },
        {  48 - 46, "pearl of wisdom", 500, 5000, CLERICS },
        {  50 - 48, "periapt of foul rotting", 0, 1000 },
        {  53 - 50, "periapt of health", 1000, 10000 },
        {  60 - 53, "periapt of proof against poison", 1500, 12500 },
        {  64 - 60, "periapt of wound closure", 1000, 10000 },
        {  70 - 64, "phylactery of faithfulness", 1000, 7500, CLERICS },
        {  74 - 70, "phylactery of long years", 3000, 25000, CLERICS },
        {  76 - 74, "phylactery of monstrous attention", 0, 2000, CLERICS },
        {  84 - 76, "pipes of the sewers", 1750, 8500 },
        {  85 - 84, "portable hole", 5000, 50000 },
        { 100 - 85, "Quaal's feather token", 500, 2000, NO_CLASS_RESTRICTIONS, generate_quaals_feather_token },
    };
    size_t const misc_magic_items_table_count = sizeof misc_magic_items_table
                                              / sizeof misc_magic_items_table[0];
    
    int score = roll("1d100", rnd);
    int range = 0;
    struct misc_magic_item const *misc_magic_item = NULL;
    for (int i = 0; i < misc_magic_items_table_count; ++i) {
        range += misc_magic_items_table[i].percent;
        if (score <= range) {
            misc_magic_item = &misc_magic_items_table[i];
            break;
        }
    }
    assert(misc_magic_item);
    
    if (misc_magic_item->generate) {
        misc_magic_item->generate(magic_item, rnd);
    } else {
        magic_item->experience_points = misc_magic_item->experience_points;
        magic_item->true_value_in_cp = gp_to_cp(misc_magic_item->sale_value_in_gp);
        magic_item->true_description = str_alloc_formatted("%s",
                                                           misc_magic_item->name);
    }
}


static void
generate_misc_magic_item_table5(struct magic_item *magic_item, struct rnd *rnd)
{
    static struct misc_magic_item const misc_magic_items_table[] = {
        {   1 -  0, "robe of the archmagi", 6000, 65000, MAGIC_USERS },
        {   8 -  1, "robe of blending", 3500, 35000 },
        {   9 -  8, "robe of eyes", 4500, 50000, MAGIC_USERS },
        {  10 -  9, "robe of powerlessness", 0, 1000 },
        {  11 - 10, "robe of scintillating colors", 2750, 25000, CLERICS | MAGIC_USERS },
        {  19 - 11, "robe of useful items", 1500, 15000, MAGIC_USERS },
        {  25 - 19, "rope of climbing", 1000, 10000 },
        {  27 - 25, "rope of construction", 0, 1000 },
        {  31 - 27, "rope of entanglement", 1250, 12000 },
        {  32 - 31, "rug of smothering", 0, 1500 },
        {  33 - 32, "rug of welcome", 6500, 45000, MAGIC_USERS },
        {  34 - 33, "saw of mighty cutting", 1750, 12500, FIGHTERS },
        {  35 - 34, "scarab of death", 0, 2500 },
        {  38 - 35, "scarab of enraging enemies", 1000, 8000 },
        {  40 - 38, "scarab of insanity", 1500, 11000 },
        {  46 - 40, "scarab of protection", 2500, 25000 },
        {  47 - 46, "spade of colossal excavation", 1000, 6500, FIGHTERS },
        {  48 - 47, "sphere of annihilation", 3750, 30000, MAGIC_USERS },
        {  50 - 48, "stone of controlling earth elementals", 1500, 12500 },
        {  52 - 50, "stone of good luck (luckstone)", 3000, 25000 },
        {  54 - 52, "stone of weight (loadstone)", 0, 1000 },
        {  57 - 54, "talisman of pure good", 3500, 27500, CLERICS },
        {  58 - 57, "talisman of the sphere", 100, 10000, MAGIC_USERS },
        {  60 - 58, "talisman of ultimate evil", 3500, 32500, CLERICS },
        {  66 - 60, "talisman of Zagy", 1000, 10000 },
        {  67 - 66, "tome of clear thought", 8000, 48000 },
        {  68 - 67, "tome of leadership and influence", 7500, 40000 },
        {  69 - 68, "tome of understanding", 8000, 43500 },
        {  76 - 69, "trident of fish command", 500, 4000, CLERICS | FIGHTERS | THIEVES },
        {  78 - 76, "trident of submission", 1250, 12500, FIGHTERS },
        {  83 - 78, "trident of warning", 1000, 10000, CLERICS | FIGHTERS | THIEVES },
        {  85 - 83, "trident of yearning", 0, 1000 },
        {  87 - 85, "vacuous grimoire", 0, 1000 },
        {  90 - 87, "well of many worlds", 6000, 12000 },
        { 100 - 90, "wings of flying", 750, 7500 }
    };
    size_t const misc_magic_items_table_count = sizeof misc_magic_items_table
                                              / sizeof misc_magic_items_table[0];
    
    int score = roll("1d100", rnd);
    int range = 0;
    struct misc_magic_item const *misc_magic_item = NULL;
    for (int i = 0; i < misc_magic_items_table_count; ++i) {
        range += misc_magic_items_table[i].percent;
        if (score <= range) {
            misc_magic_item = &misc_magic_items_table[i];
            break;
        }
    }
    assert(misc_magic_item);
    
    if (misc_magic_item->generate) {
        misc_magic_item->generate(magic_item, rnd);
    } else {
        magic_item->experience_points = misc_magic_item->experience_points;
        magic_item->true_value_in_cp = gp_to_cp(misc_magic_item->sale_value_in_gp);
        magic_item->true_description = str_alloc_formatted("%s",
                                                           misc_magic_item->name);
    }
}


static void
generate_misc_weapon(struct magic_item *magic_item, struct rnd *rnd)
{
    
    static struct misc_weapon {
        int percent;
        char const *name;
        char const *quantity;
        int experience_points;
        int sale_value_in_gp;
    } const misc_weapons_table[] = {
        {   8 -  0, "arrow +1", "2d12", 20, 120 },
        {  12 -  8, "arrow +2", "2d8", 50, 300 },
        {  14 - 12, "arrow +3", "2d6", 75, 450 },
        {  15 - 14, "arrow of slaying", "1", 250, 2500 },
        {  20 - 15, "axe +1", "1", 300, 1750 },
        {  22 - 20, "axe +2", "1", 600, 3750 },
        {  23 - 22, "axe +2, throwing", "1", 750, 4500 },
        {  24 - 23, "axe +3", "1", 1000, 7000 },
        {  27 - 24, "battle axe +1", "1", 400, 2500 },
        {  32 - 27, "bolt +2", "2d10", 50, 300 },
        {  35 - 32, "bow +1", "1", 500, 3500 },
        {  36 - 35, "crossbow of accuracy +3", "1", 2000, 12000 },
        {  37 - 36, "crossbow of distance", "1", 1500, 7500 },
        {  38 - 37, "crossbow of speed", "1", 1500, 7500 },
        {  46 - 38, "dagger +1, +2 vs creatures smaller than man-sized", "1", 100, 750 },
        {  50 - 46, "dagger +2, +3 vs creatures larger than man-sized", "1", 250, 2000 },
        {  51 - 50, "dagger of venom", "1", 350, 3000 },
        {  56 - 51, "flail +1", "1", 450, 4000 },
        {  60 - 56, "hammer +1", "1", 300, 2500 },
        {  62 - 60, "hammer +2", "1", 650, 6000 },
        {  63 - 62, "hammer +3, dwarven thrower", "1", 1500, 15000 },
        {  64 - 63, "hammer of thunderbolts", "1", 2500, 25000 },
        {  67 - 64, "javelin +2", "1", 750, 5000 },
        {  72 - 67, "mace +1", "1", 350, 3000 },
        {  75 - 72, "mace +2", "1", 700, 4500 },
        {  76 - 75, "mace of disruption", "1", 1750, 17500 },
        {  77 - 76, "mace +4", "1", 1500, 15000 },
        {  80 - 77, "military pick +1", "1", 350, 2500 },
        {  83 - 80, "morning star +1", "1", 400, 3000 },
        {  88 - 83, "scimitar +2", "1", 750, 6000 },
        {  89 - 88, "sling of seeking +2", "1", 700, 7000 },
        {  94 - 89, "spear +1", "1", 500, 3000 },
        {  96 - 94, "spear +2", "1", 1000, 6500 },
        {  97 - 96, "spear +3", "1", 1750, 15000 },
        {  99 - 97, "spear, cursed backbiter", "1", 0, 1000 },
        { 100 - 99, "trident (military fork) +3", "1", 1500, 12500 }
    };
    size_t const misc_weapons_table_count = sizeof misc_weapons_table
                                          / sizeof misc_weapons_table[0];
    
    int score = roll("1d100", rnd);
    int range = 0;
    struct misc_weapon const *misc_weapon = NULL;
    for (int i = 0; i < misc_weapons_table_count; ++i) {
        range += misc_weapons_table[i].percent;
        if (score <= range) {
            misc_weapon = &misc_weapons_table[i];
            break;
        }
    }
    assert(misc_weapon);
    
    int quantity = roll(misc_weapon->quantity, rnd);
    magic_item->experience_points = misc_weapon->experience_points * quantity;
    magic_item->true_value_in_cp = gp_to_cp(misc_weapon->sale_value_in_gp * quantity);
    
    if (1 == quantity) {
        magic_item->true_description = str_alloc_formatted("%s",
                                                           misc_weapon->name);
    } else {
        magic_item->true_description = str_alloc_formatted("(%i) %s",
                                                           quantity,
                                                           misc_weapon->name);
    }
}


static void
generate_necklace_of_missiles(struct magic_item *magic_item, struct rnd *rnd)
{
    char const *type;
    int total_hit_dice;
    
    int score = roll("1d20", rnd);
    if (score <= 4) {
        type = "(1) 5 hd, (2) 3 hd";
        total_hit_dice = (1 * 5) + (2 * 3);
    } else if (score <= 8) {
        type = "(1) 6 hd, (2) 4 hd, (2) 2 hd";
        total_hit_dice = (1 * 6) + (2 * 4) + (2 * 2);
    } else if (score <= 12) {
        type = "(1) 7 hd, (2) 5 hd, (4) 3 hd";
        total_hit_dice = (1 * 7) + (2 * 5) + (4 * 3);
    } else if (score <= 16) {
        type = "(1) 8 hd, (2) 6 hd, (2) 4 hd, (4) 2 hd";
        total_hit_dice = (1 * 8) + (2 * 6) + (2 * 4) + (4 * 2);
    } else if (score <= 18) {
        type = "(1) 9 hd, (2) 7 hd, (2) 5 hd, (2) 3 hd";
        total_hit_dice = (1 * 9) + (2 * 7) + (2 * 5) + (2 * 3);
    } else if (score <= 19) {
        type = "(1) 10 hd, (2) 8 hd, (2) 6 hd, (4) 4 hd";
        total_hit_dice = (1 * 10) + (2 * 8) + (2 * 6) + (4 * 4);
    } else {
        type = "(1) 11 hd, (2) 9 hd, (2) 7 hd, (2) 5 hd, (2) 3 hd";
        total_hit_dice = (1 * 11) + (2 * 9) + (2 * 7) + (2 * 5) + (2 * 3);
    }
    
    magic_item->experience_points = total_hit_dice * 50;
    magic_item->true_value_in_cp = gp_to_cp(total_hit_dice * 200);
    magic_item->true_description = str_alloc_formatted("necklace of missiles (with %s missiles)",
                                                       type);
}


static void
generate_necklace_of_prayer_beads(struct magic_item *magic_item, struct rnd *rnd)
{
    int const types_count = 6;
    int special_bead_counts[types_count];
    memset(special_bead_counts, 0, sizeof special_bead_counts);
    int special_bead_total = roll("1d4+2", rnd);
    for (int i = 0; i < special_bead_total; ++i) {
        int score = roll("1d20", rnd);
        if (score <= 5) {
            ++special_bead_counts[0];
        } else if (score <= 10) {
            ++special_bead_counts[1];
        } else if (score <= 15) {
            ++special_bead_counts[2];
        } else if (score <= 17) {
            ++special_bead_counts[3];
        } else if (score <= 18) {
            ++special_bead_counts[4];
        } else {
            ++special_bead_counts[5];
        }
    }
    
    char const *types[] = {
        "atonement", "blessing", "curing", "karma", "summons", "wind walking"
    };
    char *description = str_alloc_formatted("necklace of prayer beads with ");
    bool first = true;
    for (int i = 0; i < types_count; ++i) {
        if (!special_bead_counts[i]) continue;
        if (!first) str_realloc_append_formatted(&description, ", ");
        first = false;
        if (special_bead_counts[i] > 1) {
            str_realloc_append_formatted(&description,"(%i) ",
                                         special_bead_counts[i]);
        }
        str_realloc_append_formatted(&description, "%s", types[i]);
    }

    magic_item->experience_points = special_bead_total * 500;
    magic_item->true_value_in_cp = gp_to_cp(special_bead_total * 3000);
    magic_item->true_description = description;
}


static void
generate_nolzurs_marvelous_pigments(struct magic_item *magic_item,
                                    struct rnd *rnd)
{
    int quantity = roll("1d4", rnd);
    char const *plural = 1 == quantity ? "" : "s";
    magic_item->experience_points = quantity * 500;
    magic_item->true_value_in_cp = gp_to_cp(quantity * 3000);
    magic_item->true_description = str_alloc_formatted("Nolzurs' marvelous pigments (%i pot%s)",
                                                       quantity, plural);
}


static void
generate_orb_of_dragonkind(struct magic_item *magic_item, struct rnd *rnd)
{
    int score = roll("1d8", rnd);
    char const *type = NULL;
    switch (score) {
        case 1: type = "hatchling"; break;
        case 2: type = "wyrmkin"; break;
        case 3: type = "dragonette"; break;
        case 4: type = "dragon"; break;
        case 5: type = "great serpent"; break;
        case 6: type = "fire drake"; break;
        case 7: type = "elder wyrm"; break;
        case 8: type = "eternal grand dragon"; break;
        default: fail("1d8 die roll is %i", score); break;
    }
    magic_item->experience_points = 0;
    magic_item->true_value_in_cp = gp_to_cp(score * 10000);
    magic_item->true_description = str_alloc_formatted("orb of dragonkind: orb of the %s",
                                                       type);
}


static void
generate_pearl_of_power(struct magic_item *magic_item, struct rnd *rnd)
{
    int spell_quantity = 1;
    int spell_level;
    
    int score = roll("1d100", rnd);
    if (score <= 25) {
        spell_level = 1;
    } else if (score <= 45) {
        spell_level = 2;
    } else if (score <= 60) {
        spell_level = 3;
    } else if (score <= 75) {
        spell_level = 4;
    } else if (score <= 85) {
        spell_level = 5;
    } else if (score <= 92) {
        spell_level = 6;
    } else if (score <= 96) {
        spell_level = 7;
    } else if (score <= 98) {
        spell_level = 8;
    } else if (score <= 99) {
        spell_level = 9;
    } else {
        spell_quantity = 2;
        spell_level = roll("1d6", rnd);
    }
    
    char const *effect;
    if (1 == roll("1d20", rnd)) {
        effect = "(cursed) forget";
    } else {
        effect = "recall";
    }
    
    magic_item->experience_points = spell_level * spell_quantity * 200;
    magic_item->true_value_in_cp = spell_level * spell_quantity * gp_to_cp(2000);
    
    char const *plural = (spell_quantity == 1) ? "" : "s";
    magic_item->true_description = str_alloc_formatted("pearl of power: %s %i level %i spell%s",
                                                       effect, spell_quantity,
                                                       spell_level, plural);
}


static void
generate_potion(struct magic_item *magic_item, struct rnd *rnd)
{
    static struct potion {
        int percent;
        char const *name;
        int experience_points;
        int sale_value_in_gp;
        class_restrictions_t class_restrictions;
        generate_function generate;
    } const potions_table[] = {
        {   3 -  0, "animal control", 250, 400 },
        {   6 -  3, "clairaudience", 250, 400 },
        {   9 -  6, "clairvoyance", 300, 500 },
        {  12 -  9, "climbing", 300, 500 },
        {  15 - 12, "delusion", 0, 150 },
        {  18 - 15, "diminution", 300, 500 },
        {  20 - 18, "dragon control", 500, 5000, NO_CLASS_RESTRICTIONS, generate_potion_of_dragon_control },
        {  23 - 20, "ESP", 500, 850 },
        {  26 - 23, "extra-healing", 400, 800 },
        {  29 - 26, "fire resistance", 250, 400 },
        {  32 - 29, "flying", 500, 750 },
        {  34 - 32, "gaseous form", 300, 400 },
        {  36 - 34, "giant control", 400, 1000, NO_CLASS_RESTRICTIONS, generate_potion_of_giant_control },
        {  39 - 36, "giant strength", 500, 900, FIGHTERS, generate_potion_of_giant_strength },
        {  41 - 39, "growth", 250, 300 },
        {  47 - 41, "healing", 200, 400 },
        {  49 - 47, "heroism", 300, 500, FIGHTERS },
        {  51 - 49, "human control", 500, 900 },
        {  54 - 51, "invisibility", 250, 500 },
        {  57 - 54, "invulnerability", 350, 500, FIGHTERS },
        {  60 - 57, "levitation", 250, 400 },
        {  63 - 60, "longevity", 500, 1000 },
        {  66 - 63, "oil of etherealness", 600, 1500 },
        {  69 - 66, "oil of slipperiness", 400, 750 },
        {  72 - 69, "philter of love", 200, 300 },
        {  75 - 72, "philter of persuasiveness", 400, 850 },
        {  78 - 75, "plant control", 250, 300 },
        {  81 - 78, "polymorph (self)", 200, 350 },
        {  84 - 81, "poison", 0, 0 },
        {  87 - 84, "speed", 200, 450 },
        {  90 - 87, "super-heroism", 450, 750, FIGHTERS },
        {  93 - 90, "sweet water", 200, 250 },
        {  96 - 93, "treasure finding", 600, 2000 },
        {  97 - 96, "undead control", 700, 2500 },
        { 100 - 97, "water breathing", 400, 900 }
    };
    size_t const potions_table_count = sizeof potions_table
                                     / sizeof potions_table[0];
    
    int score = roll("1d100", rnd);
    int range = 0;
    struct potion const *potion = NULL;
    for (int i = 0; i < potions_table_count; ++i) {
        range += potions_table[i].percent;
        if (score <= range) {
            potion = &potions_table[i];
            break;
        }
    }
    assert(potion);
    
    if (potion->generate) {
        potion->generate(magic_item, rnd);
    } else {
        magic_item->experience_points = potion->experience_points;
        magic_item->true_value_in_cp = gp_to_cp(potion->sale_value_in_gp);
        magic_item->true_description = str_alloc_formatted("%s potion",
                                                           potion->name);
    }
}


static void
generate_potion_of_dragon_control(struct magic_item *magic_item,
                                  struct rnd *rnd)
{
    char const *type;
    
    int score = roll("1d20", rnd);
    if (score <= 2) {
        type = "white";
        magic_item->experience_points = 500;
        magic_item->true_value_in_cp = gp_to_cp(5000);
    } else if (score <= 4) {
        type = "black";
        magic_item->experience_points = 500;
        magic_item->true_value_in_cp = gp_to_cp(5000);
    } else if (score <= 7) {
        type = "green";
        magic_item->experience_points = 500;
        magic_item->true_value_in_cp = gp_to_cp(5000);
    } else if (score <= 9) {
        type = "blue";
        magic_item->experience_points = 500;
        magic_item->true_value_in_cp = gp_to_cp(5000);
    } else if (score <= 10) {
        type = "red";
        magic_item->experience_points = 500;
        magic_item->true_value_in_cp = gp_to_cp(5000);
    } else if (score <= 12) {
        type = "brass";
        magic_item->experience_points = 500;
        magic_item->true_value_in_cp = gp_to_cp(5000);
    } else if (score <= 14) {
        type = "copper";
        magic_item->experience_points = 500;
        magic_item->true_value_in_cp = gp_to_cp(5000);
    } else if (score <= 15) {
        type = "bronze";
        magic_item->experience_points = 500;
        magic_item->true_value_in_cp = gp_to_cp(5000);
    } else if (score <= 16) {
        type = "silver";
        magic_item->experience_points = 500;
        magic_item->true_value_in_cp = gp_to_cp(5000);
    } else if (score <= 17) {
        type = "gold";
        magic_item->experience_points = 500;
        magic_item->true_value_in_cp = gp_to_cp(5000);
    } else if (score <= 19) {
        type = "evil";
        magic_item->experience_points = 1000;
        magic_item->true_value_in_cp = gp_to_cp(9000);
    } else {
        type = "good";
        magic_item->experience_points = 1000;
        magic_item->true_value_in_cp = gp_to_cp(9000);
    }
    
    magic_item->true_description = str_alloc_formatted("potion of %s dragon control",
                                                       type);
}


static void
generate_potion_of_giant_control(struct magic_item *magic_item, struct rnd *rnd)
{
    char const *type;
    int level;
    
    int score = roll("1d20", rnd);
    if (score <= 5) {
        type = "hill";
        level = 1;
    } else if (score <= 9) {
        type = "stone";
        level = 2;
    } else if (score <= 13) {
        type = "frost";
        level = 3;
    } else if (score <= 17) {
        type = "fire";
        level = 4;
    } else if (score <= 19) {
        type = "cloud";
        level = 5;
    } else {
        type = "storm";
        level = 6;
    }
    
    magic_item->experience_points = level * 100 + 300;
    magic_item->true_value_in_cp = gp_to_cp(level * 1000);
    magic_item->true_description = str_alloc_formatted("potion of %s giant control",
                                                       type);
}


static void
generate_potion_of_giant_strength(struct magic_item *magic_item,
                                  struct rnd *rnd)
{
    char const *type;
    int level;
    
    int score = roll("1d20", rnd);
    if (score <= 6) {
        type ="hill";
        level = 1;
    } else if (score <= 10) {
        type = "stone";
        level = 2;
    } else if (score <= 14) {
        type = "frost";
        level = 3;
    } else if (score <= 17) {
        type = "fire";
        level = 4;
    } else if (score <= 19) {
        type = "cloud";
        level = 5;
    } else {
        type = "storm";
        level = 6;
    }
    
    magic_item->experience_points = level * 50 + 450;
    magic_item->true_value_in_cp = gp_to_cp(level * 100 + 800);
    magic_item->true_description = str_alloc_formatted("potion of %s giant strength",
                                                       type);
}


static void
generate_quaals_feather_token(struct magic_item *magic_item, struct rnd *rnd)
{
    char const *type;
    
    int score = roll("1d20", rnd);
    if (score <= 4) {
        type = "anchor";
        magic_item->experience_points = 500;
        magic_item->true_value_in_cp = gp_to_cp(2000);
    } else if (score <= 7) {
        type = "bird";
        magic_item->experience_points = 500;
        magic_item->true_value_in_cp = gp_to_cp(2000);
    } else if (score <= 10) {
        type = "fan";
        magic_item->experience_points = 500;
        magic_item->true_value_in_cp = gp_to_cp(2000);
    } else if (score <= 13) {
        type = "swan boat";
        magic_item->experience_points = 500;
        magic_item->true_value_in_cp = gp_to_cp(2000);
    } else if (score <= 18) {
        type = "tree";
        magic_item->experience_points = 500;
        magic_item->true_value_in_cp = gp_to_cp(2000);
    } else {
        type = "whip";
        magic_item->experience_points = 1000;
        magic_item->true_value_in_cp = gp_to_cp(7000);
    }
    
    magic_item->true_description = str_alloc_formatted("Quaal's feather token: %s",
                                                       type);
}


static void
generate_ring(struct magic_item *magic_item, struct rnd *rnd)
{
    static struct ring {
        int percent;
        char const *name;
        bool limited_charges;
        int experience_points;
        int sale_value_in_gp;
        class_restrictions_t class_restrictions;
        generate_function generate;
    } const rings_table[] = {
        {   6 -  0, "contrariness", false, 0, 1000 },
        {  12 -  6, "delusion", false, 0, 2000 },
        {  14 - 12, "djinni summoning", true, 3000, 20000 },
        {  15 - 14, "elemental control", false, 5000, 25000 },
        {  21 - 15, "feather falling", false, 1000, 5000 },
        {  27 - 21, "fire resistance", false, 1000, 5000 },
        {  30 - 27, "free action", false, 1000, 5000 },
        {  33 - 30, "human influence", true, 2000, 10000 },
        {  40 - 33, "invisibility", false, 1500, 7500 },
        {  43 - 40, "mammal control", true, 1000, 5000 },
        {  44 - 43, "multiple wishes", true, 5000, 25000 },
        {  60 - 44, "protection", false, 2000, 10000, NO_CLASS_RESTRICTIONS, generate_ring_of_protection },
        {  61 - 60, "regeneration", false, 5000, 40000 },
        {  63 - 61, "shooting stars", false, 3000, 15000 },
        {  65 - 63, "spell storing", false, 2500, 22500 },
        {  69 - 65, "spell turning", false, 2000, 17500 },
        {  75 - 69, "swimming", false, 1000, 5000 },
        {  77 - 75, "telekinesis", true, 2000, 10000 },
        {  79 - 77, "three wishes", true, 3000, 15000 },
        {  85 - 79, "warmth", false, 1000, 5000 },
        {  90 - 85, "water walking", false, 1000, 5000 },
        {  98 - 90, "weakness", false, 0, 1000 },
        {  99 - 98, "wizardry", true, 4000, 50000, MAGIC_USERS },
        { 100 - 99, "x-ray vision", false, 4000, 35000 }
    };
    size_t const rings_table_count = sizeof rings_table / sizeof rings_table[0];
    
    int score = roll("1d100", rnd);
    int range = 0;
    struct ring const *ring = NULL;
    for (int i = 0; i < rings_table_count; ++i) {
        range += rings_table[i].percent;
        if (score <= range) {
            ring = &rings_table[i];
            break;
        }
    }
    assert(ring);
    
    if (ring->generate) {
        ring->generate(magic_item, rnd);
    } else {
        magic_item->experience_points = ring->experience_points;
        magic_item->true_value_in_cp = gp_to_cp(ring->sale_value_in_gp);
        magic_item->true_description = str_alloc_formatted("ring of %s",
                                                           ring->name);
    }
}


static void
generate_ring_of_protection(struct magic_item *magic_item, struct rnd *rnd)
{
    char const *type;
    
    int score = roll("1d100", rnd);
    if (score <= 70) {
        type = "+1";
        magic_item->experience_points = 2000;
    } else if (score <= 82) {
        type = "+2";
        magic_item->experience_points = 2500;
    } else if (score <= 83) {
        type = "+2, 5 ft radius";
        magic_item->experience_points = 2500;
    } else if (score <= 90) {
        type = "+3";
        magic_item->experience_points = 3000;
    } else if (score <= 91) {
        type = "+3, 5 ft radius";
        magic_item->experience_points = 3500;
    } else if (score <= 97) {
        type = "+4 on AC, +2 saving throws";
        magic_item->experience_points = 3500;
    } else {
        type = "+6 on AC, +1 saving throws";
        magic_item->experience_points = 4000;
    }
    
    magic_item->true_value_in_cp = magic_item->experience_points * gp_to_cp(5);
    magic_item->true_description = str_alloc_formatted("ring of protection %s",
                                                       type);
}


static void
generate_rod_staff_or_wand(struct magic_item *magic_item, struct rnd *rnd)
{
    static struct rod_staff_or_wand {
        int percent;
        char const *name;
        int experience_points;
        int sale_value_in_gp;
        class_restrictions_t class_restrictions;
    } const rods_staves_and_wands_table[] = {
        {   3 -  0, "rod of absorption", 7500, 40000, CLERICS | MAGIC_USERS },
        {   4 -  3, "rod of beguiling", 5000, 30000, CLERICS | MAGIC_USERS | THIEVES },
        {  14 -  4, "rod of cancellation", 10000, 15000 },
        {  16 - 14, "rod of lordly might", 6000, 20000, FIGHTERS },
        {  17 - 16, "rod of resurrection", 10000, 35000, CLERICS },
        {  18 - 17, "rod of rulership", 8000, 35000 },
        {  19 - 18, "rod of smiting", 4000, 15000, CLERICS | FIGHTERS },
        {  20 - 19, "staff of command", 5000, 25000, CLERICS | MAGIC_USERS },
        {  22 - 20, "staff of curing", 6000, 25000, CLERICS },
        {  23 - 22, "staff of the magi", 15000, 75000, MAGIC_USERS },
        {  24 - 23, "staff of power", 12000, 60000, MAGIC_USERS },
        {  27 - 24, "staff of the serpent", 7000, 35000, CLERICS },
        {  31 - 27, "staff of striking", 6000, 15000, CLERICS | MAGIC_USERS },
        {  33 - 31, "staff of withering", 8000, 35000, CLERICS },
        {  34 - 33, "wand of conjuration", 7000, 35000, MAGIC_USERS },
        {  38 - 34, "wand of enemy detection", 2000, 10000 },
        {  41 - 38, "wand of fear", 3000, 15000, CLERICS | MAGIC_USERS },
        {  44 - 41, "wand of fire", 4500, 25000, MAGIC_USERS },
        {  47 - 44, "wand of frost", 6000, 50000, MAGIC_USERS },
        {  52 - 47, "wand of illumination", 2000, 10000 },
        {  56 - 52, "wand of illusion", 3000, 20000, MAGIC_USERS },
        {  59 - 56, "wand of lightning", 4000, 30000, MAGIC_USERS },
        {  68 - 59, "wand of magic detection", 2500, 25000 },
        {  73 - 68, "wand of metal & mineral detection", 1500, 7500 },
        {  78 - 73, "wand of magic missiles", 4000, 35000 },
        {  86 - 78, "wand of negation", 3500, 15000 },
        {  89 - 86, "wand of paralyzation", 3500, 25000, MAGIC_USERS },
        {  92 - 89, "wand of polymorphing", 3500, 25000, MAGIC_USERS },
        {  94 - 92, "wand of secret door & trap location", 5000, 40000 },
        { 100 - 94, "wand of wonder", 6000, 10000 }
    };
    size_t const rods_staves_and_wands_table_count = sizeof rods_staves_and_wands_table
                                                   / sizeof rods_staves_and_wands_table[0];
    
    int score = roll("1d100", rnd);
    int range = 0;
    struct rod_staff_or_wand const *rod_staff_or_wand = NULL;
    for (int i = 0; i < rods_staves_and_wands_table_count; ++i) {
        range += rods_staves_and_wands_table[i].percent;
        if (score <= range) {
            rod_staff_or_wand = &rods_staves_and_wands_table[i];
            break;
        }
    }
    
    magic_item->experience_points = rod_staff_or_wand->experience_points;
    magic_item->true_value_in_cp = gp_to_cp(rod_staff_or_wand->sale_value_in_gp);
    magic_item->true_description = str_alloc_formatted("%s",
                                                       rod_staff_or_wand->name);
}


static void
generate_scroll(struct magic_item *magic_item, struct rnd *rnd)
{
    static struct scroll {
        int percent;
        char const *name;
        bool is_spell_scroll;
        int spell_count;
        char const *magic_user_spell_level_range;
        char const *clerical_spell_level_range;
        int experience_points;
        int sale_value_in_gp;
    } const scrolls_table[] = {
        {  10 - 00, "(spell scroll)", true, 1, "1d4", "1d4", 100, 300 },
        {  16 - 10, "(spell scroll)", true, 1, "1d6", "1d6", 100, 300 },
        {  19 - 16, "(spell scroll)", true, 1, "1d8+1", "1d6+1", 100, 300 },
        {  24 - 19, "(spell scroll)", true, 2, "1d4", "1d4", 100, 300 },
        {  27 - 24, "(spell scroll)", true, 2, "1d8", "1d6", 100, 300 },
        {  32 - 27, "(spell scroll)", true, 3, "1d4", "1d4", 100, 300 },
        {  35 - 32, "(spell scroll)", true, 3, "1d8+1", "1d6+1", 100, 300 },
        {  39 - 35, "(spell scroll)", true, 4, "1d6", "1d6", 100, 300 },
        {  42 - 39, "(spell scroll)", true, 4, "1d8", "1d6", 100, 300 },
        {  46 - 42, "(spell scroll)", true, 5, "1d6", "1d6", 100, 300 },
        {  49 - 46, "(spell scroll)", true, 5, "1d8", "1d6", 100, 300 },
        {  52 - 49, "(spell scroll)", true, 6, "1d6", "1d6", 100, 300 },
        {  54 - 52, "(spell scroll)", true, 6, "1d6+2", "1d4+2", 100, 300 },
        {  57 - 54, "(spell scroll)", true, 7, "1d8", "1d6", 100, 300 },
        {  59 - 57, "(spell scroll)", true, 7, "1d8+1", "1d6+1", 100, 300 },
        {  60 - 59, "(spell scroll)", true, 7, "1d6+3", "1d4+3", 100, 300 },
        {  62 - 60, "protection from demons", false, 1, "0", "0", 2500, 2500 * 5 },
        {  64 - 62, "protection from devils", false, 1, "0", "0", 2500, 2500 * 5 },
        {  70 - 64, "protection from elementals", false, 1, "0", "0", 1500, 1500 * 5 },
        {  76 - 70, "protection from lycanthropes", false, 1, "0", "0", 1000, 1000 * 5 },
        {  82 - 76, "protection from magic", false, 1, "0", "0", 1500, 1500 * 5 },
        {  87 - 82, "protection from petrification", false, 1, "0", "0", 2000, 2000 * 5 },
        {  92 - 87, "protection from possession", false, 1, "0", "0", 2000, 2000 * 5 },
        {  97 - 92, "protection from undead", false, 1, "0", "0", 1500, 1500 * 5 },
        { 100 - 97, "cursed", false, 1, "0", "0" }
    };
    size_t const scrolls_table_count = sizeof scrolls_table
                                     / sizeof scrolls_table[0];
    
    int score = roll("1d100", rnd);
    int range = 0;
    struct scroll const *scroll = NULL;
    for (int i = 0; i < scrolls_table_count; ++i) {
        range += scrolls_table[i].percent;
        if (score <= range) {
            scroll = &scrolls_table[i];
            break;
        }
    }
    assert(scroll);
    
    if (scroll->is_spell_scroll) {
        magic_item->true_details = calloc_or_die(scroll->spell_count + 1,
                                                 sizeof(char *));
        
        enum spell_type spell_type;
        char const *spell_type_name;
        char const *spell_level_range = scroll->clerical_spell_level_range;
        score = roll("1d100", rnd);
        if (score <= 70) {
            if (roll("1d100", rnd) <= 10) {
                spell_type = spell_type_illusionist;
                spell_type_name = "illusionist";
            } else {
                spell_type = spell_type_magic_user;
                spell_type_name = "magic-user";
                spell_level_range = scroll->magic_user_spell_level_range;
            }
        } else {
            if (roll("1d100", rnd) <= 25) {
                spell_type = spell_type_drudical;
                spell_type_name = "druidical";
            } else {
                spell_type = spell_type_clerical;
                spell_type_name = "clerical";
            }
        }
        
        int spell_levels = 0;
        for (int i = 0; i < scroll->spell_count; ++i) {
            int spell_level = roll(spell_level_range, rnd);
            char const *spell_name = determine_spell(rnd, spell_type, spell_level);
            magic_item->true_details[i] = str_alloc_formatted("level %i: %s",
                                                              spell_level,
                                                              spell_name);
            spell_levels += spell_level;
        }
        
        magic_item->experience_points = scroll->experience_points * spell_levels;
        magic_item->true_value_in_cp = gp_to_cp(scroll->sale_value_in_gp) * spell_levels;
        
        char const *plural = scroll->spell_count == 1 ? "" : "s";
        magic_item->true_description = str_alloc_formatted("%s scroll (%i spell%s)",
                                                           spell_type_name,
                                                           scroll->spell_count,
                                                           plural);
    } else {
        magic_item->experience_points = scroll->experience_points;
        magic_item->true_value_in_cp = gp_to_cp(scroll->sale_value_in_gp);
        magic_item->true_description = str_alloc_formatted("%s scroll",
                                                           scroll->name);
    }
}


static void
generate_sword(struct magic_item *magic_item, struct rnd *rnd)
{
    static struct sword {
        int percent;
        char const *name_format;
        char const *details[4];
        int base_ego;
        int experience_points;
        int sale_value_in_gp;
    } const swords_table[] = {
        {  25 -  0, "%s +1", {}, 1, 400, 2000 },
        {  30 - 25, "%s +1",
            { "+2 vs magic-using & enchanted creatures" }, 3, 600, 3000 },
        {  35 - 30, "%s +1",
            { "+3 vs lycanthropes & shape changers" }, 4, 700, 3500 },
        {  40 - 35, "%s +1",
            { "+3 vs regenerating creatures" }, 4, 800, 4000 },
        {  45 - 40, "%s +1",
            { "+4 vs reptiles" }, 5, 800, 4000 },
        {  49 - 45, "%s +1, flame tongue",
            { "+2 vs regenerating creatures",
                "+3 vs cold-using, inflammable or avian creatures",
                "+4 vs undead" }, 5, 900, 4500 },
        {  50 - 49, "%s +1, luck blade", {}, 2, 1000, 5000 },
        {  58 - 50, "%s +2", {}, 2, 800, 4000 },
        {  62 - 58, "%s +2, giant slayer", {}, 4, 900, 4500 },
        {  66 - 62, "%s +2, dragon slayer", {}, 4, 900, 4500 },
        {  67 - 66, "%s +2, nine lives stealer", {}, 4, 1600, 8000 },
        {  71 - 67, "%s +3", {}, 3, 1400, 7000 },
        {  74 - 71, "%s +3, frost brand",
            { "+6 vs fire using/dwelling creatures" }, 9, 1600, 8000 },
        {  76 - 74, "%s +4", {}, 4, 2000, 10000 },
        {  77 - 76, "%s +4, defender", {}, 8, 3000, 15000 },
        {  78 - 77, "%s +5", {}, 5, 3000, 15000 },
        {  79 - 78, "%s +5, defender", {}, 10, 3600, 18000 },
        {  80 - 79, "%s +5, holy avenger", {}, 10, 4000, 20000 },
        {  81 - 80, "%s of dancing", {}, 8, 4400, 22000 },
        {  82 - 81, "%s of wounding", {}, 2, 4400, 22000 },
        {  83 - 82, "%s of life stealing", {}, 4, 5000, 25000 },
        {  84 - 83, "%s of sharpness", {}, 6, 7000, 35000 },
        {  85 - 84, "%s, vorpal weapon", {}, 6, 10000, 50000 },
        {  90 - 85, "%s +1, cursed", {}, 1, 400, 0 },
        {  95 - 90, "%s -2, cursed", {}, 2, 600, 0 },
        { 100 - 95, "%s, cursed berserking", {}, 4, 900, 0 }
    };
    size_t const swords_table_count = sizeof swords_table
                                    / sizeof swords_table[0];
    
    int score = roll("1d100", rnd);
    int range = 0;
    struct sword const *sword = NULL;
    for (int i = 0; i < swords_table_count; ++i) {
        range += swords_table[i].percent;
        if (score <= range) {
            sword = &swords_table[i];
            break;
        }
    }
    assert(sword);
    
    magic_item->experience_points = sword->experience_points;
    magic_item->true_value_in_cp = gp_to_cp(sword->sale_value_in_gp);
    
    char const *sword_type;
    score = roll("1d100", rnd);
    if (score <= 70) {
        sword_type = "longsword";
    } else if (score <= 90) {
        sword_type = "broadsword";
    } else if (score <= 95) {
        sword_type = "short sword";
    } else if (score <= 99) {
        sword_type = "bastard sword";
    } else {
        sword_type = "two-handed sword";
    }
    magic_item->true_description = str_alloc_formatted(sword->name_format,
                                                       sword_type);
    
    int detail_count = 0;
    while (sword->details[detail_count]) {
        ++detail_count;
    }
    if (detail_count) {
        magic_item->true_details = calloc_or_die(detail_count + 1, sizeof(char *));
        for (int i = 0; i < detail_count; ++i) {
            magic_item->true_details[i] = strdup_or_die(sword->details[i]);
        }
    }
    
    int ego = sword->base_ego;
    int intelligence = 0;
    int primary_ability_count = 0;
    int extraordinary_power_count = 0;
    char const *communication = NULL;
    
    score = roll("1d100", rnd);
    if (score <= 75) {
        /* not an unusual magic sword */
        return;
    } else if (score <= 83) {
        intelligence = 12;
        primary_ability_count = 1;
        communication = "semi-empathy";
    } else if (score <= 89) {
        intelligence = 13;
        primary_ability_count = 2;
        communication = "empathy";
    } else if (score <= 94) {
        intelligence = 14;
        primary_ability_count = 2;
        communication = "speech";
    } else if (score <= 97) {
        intelligence = 15;
        primary_ability_count = 3;
        communication = "speech";
    } else if (score <= 99) {
        intelligence = 16;
        primary_ability_count = 3;
        communication = "speech";
        ego += 1; /* read languages */
    } else {
        intelligence = 17;
        primary_ability_count = 3;
        extraordinary_power_count = 1;
        communication = "speech and telepathy";
        ego += 1; /* read languages */
        ego += 2; /* read magic */
        ego += 2; /* telepathy */
    }
    assert(intelligence);
    
    char *trueDescription = magic_item->true_description;
    magic_item->true_description = str_alloc_formatted("%s (unusual)",
                                                       trueDescription);
    free_or_die(trueDescription);
    
    int detail_capacity = detail_count +
                        + 2 /* intelligence, alignment */
                        + 6 /* up to 6 primary abilities */
                        + 3 /* 2 extraordinary powers or 1 power + special purpose & power */
                        + 2 /* languages list, ego points */
                        + 1 /* NULL */;
    if (!magic_item->true_details) {
        magic_item->true_details = calloc_or_die(detail_capacity, sizeof(char *));
    } else {
        magic_item->true_details = reallocarray_or_die(magic_item->true_details,
                                                       detail_capacity,
                                                       sizeof(char *));
        char **start_of_unused = magic_item->true_details + detail_count;
        size_t size_of_unused = (detail_capacity - detail_count)
                              * sizeof(char *);
        memset(start_of_unused, 0, size_of_unused);
    }
    
    magic_item->true_details[detail_count] = str_alloc_formatted("intelligence %i (%s)",
                                                                 intelligence,
                                                                 communication);
    ++detail_count;
    assert(detail_count < detail_capacity);
    
    char const *alignment;
    score = roll("1d100", rnd);
    if (score <= 5) {
        alignment = "chaotic good";
    } else if (score <= 15) {
        alignment = "chaotic neutral";
    } else if (score <= 20) {
        alignment = "chaotic evil";
    } else if (score <= 25) {
        alignment = "neutral evil";
    } else if (score <= 30) {
        alignment = "lawful evil";
    } else if (score <= 55) {
        alignment = "lawful good";
    } else if (score <= 60) {
        alignment = "lawful neutral";
    } else if (score <= 80) {
        alignment = "neutral";
    } else {
        alignment = "neutral good";
    }
    magic_item->true_details[detail_count] = str_alloc_formatted("%s alignment",
                                                                 alignment);
    ++detail_count;
    assert(detail_count < detail_capacity);
    
    struct primary_abilities {
        int count;
        double detect_elevators;
        double detect_sloping_passages;
        double detect_traps;
        double detect_evil_good;
        double detect_precious_metals;
        double detect_gems;
        double detect_magic;
        double detect_secret_doors;
        double detect_invisible;
        double locate_object;
    } primary_abilities = {};
    for (int i = 0; i < primary_ability_count; ++i) {
        int max_score = 100;
        int rolls = 1;
        do {
            score = dice_roll(dice_make(1, max_score), rnd, NULL);
            --rolls;
            if (score <= 11) {
                ++primary_abilities.count;
                primary_abilities.detect_elevators += 1;
            } else if (score <= 22) {
                ++primary_abilities.count;
                primary_abilities.detect_sloping_passages += 1;
            } else if (score <= 33) {
                ++primary_abilities.count;
                primary_abilities.detect_traps += 1;
            } else if (score <= 44) {
                ++primary_abilities.count;
                primary_abilities.detect_evil_good += 1;
            } else if (score <= 55) {
                ++primary_abilities.count;
                primary_abilities.detect_precious_metals += 2;
            } else if (score <= 66) {
                ++primary_abilities.count;
                primary_abilities.detect_gems += 0.5;
            } else if (score <= 77) {
                ++primary_abilities.count;
                primary_abilities.detect_magic += 1;
            } else if (score <= 82) {
                ++primary_abilities.count;
                primary_abilities.detect_secret_doors += 0.5;
            } else if (score <= 87) {
                ++primary_abilities.count;
                primary_abilities.detect_invisible += 1;
            } else if (score <= 92) {
                ++primary_abilities.count;
                primary_abilities.locate_object += 12;
            } else if (score <= 98) {
                rolls = 2;
                max_score = 92;
            } else {
                ++extraordinary_power_count;
            }
        } while (rolls > 0);
    }
    ego += primary_abilities.count;
    
    if (primary_abilities.detect_elevators) {
        magic_item->true_details[detail_count] = str_alloc_formatted("detect \"elevator\"/shifting rooms/walls in a %.0f\" radius",
                                                                     primary_abilities.detect_elevators);
        ++detail_count;
    }
    if (primary_abilities.detect_sloping_passages) {
        magic_item->true_details[detail_count] = str_alloc_formatted("detect sloping passages in a %.0f\" radius",
                                                                     primary_abilities.detect_sloping_passages);
        ++detail_count;
    }
    if (primary_abilities.detect_traps) {
        magic_item->true_details[detail_count] = str_alloc_formatted("detect traps of large size in a %.0f\" radius",
                                                                     primary_abilities.detect_traps);
        ++detail_count;
    }
    if (primary_abilities.detect_evil_good) {
        magic_item->true_details[detail_count] = str_alloc_formatted("detect evil/good in a %.0f\" radius",
                                                                     primary_abilities.detect_evil_good);
        ++detail_count;
    }
    if (primary_abilities.detect_precious_metals) {
        magic_item->true_details[detail_count] = str_alloc_formatted("detect precious metals, kind and amount in a %.0f\" radius",
                                                                     primary_abilities.detect_precious_metals);
        ++detail_count;
    }
    if (primary_abilities.detect_gems) {
        magic_item->true_details[detail_count] = str_alloc_formatted("detect gems, kind and number in a %.1f\" radius",
                                                                     primary_abilities.detect_gems);
        ++detail_count;
    }
    if (primary_abilities.detect_magic) {
        magic_item->true_details[detail_count] = str_alloc_formatted("detect magic in a %.0f\" radius",
                                                                     primary_abilities.detect_magic);
        ++detail_count;
    }
    if (primary_abilities.detect_secret_doors) {
        magic_item->true_details[detail_count] = str_alloc_formatted("detect secret doors in a %.1f\" radius",
                                                                     primary_abilities.detect_secret_doors);
        ++detail_count;
    }
    if (primary_abilities.detect_invisible) {
        magic_item->true_details[detail_count] = str_alloc_formatted("detect invisible objects in a %.0f\" radius",
                                                                     primary_abilities.detect_invisible);
        ++detail_count;
    }
    if (primary_abilities.locate_object) {
        magic_item->true_details[detail_count] = str_alloc_formatted("locate object in a %.0f\" radius",
                                                                     primary_abilities.locate_object);
        ++detail_count;
    }
    assert(detail_count < detail_capacity);
    
    struct extraordinary_powers {
        int count;
        int charm_person;
        int clairaudience;
        int clairvoyance;
        int determine_directions;
        int esp;
        int flying;
        int heal;
        int illusion;
        int levitation;
        int strength;
        int telekinesis;
        int telepathy;
        int teleportation;
        int xray_vision;
    } extraordinary_powers = {};
    bool has_special_purpose = false;
    for (int i = 0; i < extraordinary_power_count; ++i) {
        int max_score = 100;
        int rolls = 1;
        bool roll_twice_possible = true;
        bool choose_one_possible = true;
        bool special_purpose_possible = true;
        do {
            score = dice_roll(dice_make(1, max_score), rnd, NULL);
            --rolls;
            if (score <= 7) {
                ++extraordinary_powers.count;
                extraordinary_powers.charm_person += 3;
            } else if (score <= 15) {
                ++extraordinary_powers.count;
                extraordinary_powers.clairaudience += 3;
            } else if (score <= 22) {
                ++extraordinary_powers.count;
                extraordinary_powers.clairvoyance += 3;
            } else if (score <= 28) {
                ++extraordinary_powers.count;
                extraordinary_powers.determine_directions += 2;
            } else if (score <= 34) {
                ++extraordinary_powers.count;
                extraordinary_powers.esp += 3;
            } else if (score <= 41) {
                ++extraordinary_powers.count;
                extraordinary_powers.flying += 1;
            } else if (score <= 47) {
                ++extraordinary_powers.count;
                extraordinary_powers.heal += 1;
            } else if (score <= 54) {
                ++extraordinary_powers.count;
                extraordinary_powers.illusion += 2;
            } else if (score <= 61) {
                ++extraordinary_powers.count;
                extraordinary_powers.levitation += 3;
            } else if (score <= 67) {
                ++extraordinary_powers.count;
                extraordinary_powers.strength += 1;
            } else if (score <= 75) {
                ++extraordinary_powers.count;
                extraordinary_powers.telekinesis += 2;
            } else if (score <= 81) {
                ++extraordinary_powers.count;
                extraordinary_powers.telepathy += 2;
            } else if (score <= 88) {
                ++extraordinary_powers.count;
                extraordinary_powers.teleportation += 1;
            } else if (score <= 94) {
                ++extraordinary_powers.count;
                extraordinary_powers.xray_vision += 2;
            } else {
                int range = 94;
                
                int const roll_twice_range = 3;
                if (roll_twice_possible) {
                    range += roll_twice_range;
                }
                if (score <= range) {
                    roll_twice_possible = false;
                    rolls = 2;
                    max_score -= roll_twice_range;
                }
                
                int const choose_one_range = 2;
                if (choose_one_possible) {
                    range += choose_one_range;
                }
                if (score <= range) {
                    choose_one_possible = false;
                    rolls = 1;
                    max_score -= choose_one_range;
                }
                
                int const special_purpose_range = 1;
                if (special_purpose_possible) {
                    range += special_purpose_range;
                }
                if (score <= range) {
                    special_purpose_possible = false;
                    has_special_purpose = true;
                    rolls = 1;
                    max_score -= special_purpose_range;
                }
                
            }
        } while (rolls > 0);
    }
    ego += extraordinary_powers.count * 2;
    
    if (extraordinary_powers.charm_person) {
        magic_item->true_details[detail_count] = str_alloc_formatted("charm person on contact - %i times/day",
                                                                     extraordinary_powers.charm_person);
        ++detail_count;
    }
    if (extraordinary_powers.clairaudience) {
        magic_item->true_details[detail_count] = str_alloc_formatted("clairaudience, 3\" range - %i times/day, 1 round per use",
                                                                     extraordinary_powers.clairaudience);
        ++detail_count;
    }
    if (extraordinary_powers.clairvoyance) {
        magic_item->true_details[detail_count] = str_alloc_formatted("clairvoyance, 3\" range - %i times/day, 1 round per use",
                                                                     extraordinary_powers.clairvoyance);
        ++detail_count;
    }
    if (extraordinary_powers.determine_directions) {
        magic_item->true_details[detail_count] = str_alloc_formatted("determine directions and depth - %i times/day",
                                                                     extraordinary_powers.determine_directions);
        ++detail_count;
    }
    if (extraordinary_powers.esp) {
        magic_item->true_details[detail_count] = str_alloc_formatted("ESP, 3\" range - %i times/day, 1 round per use",
                                                                     extraordinary_powers.esp);
        ++detail_count;
    }
    if (extraordinary_powers.flying) {
        magic_item->true_details[detail_count] = str_alloc_formatted("flying, 12\"/turn - %i hours/day",
                                                                     extraordinary_powers.flying);
        ++detail_count;
    }
    if (extraordinary_powers.heal) {
        magic_item->true_details[detail_count] = str_alloc_formatted("heal - %i times/day",
                                                                     extraordinary_powers.heal);
        ++detail_count;
    }
    if (extraordinary_powers.illusion) {
        magic_item->true_details[detail_count] = str_alloc_formatted("illusion, 12\" range - %i times/day, as the wand",
                                                                     extraordinary_powers.illusion);
        ++detail_count;
    }
    if (extraordinary_powers.levitation) {
        magic_item->true_details[detail_count] = str_alloc_formatted("levitation, 1 turn duration - %i times/day, at 6th level of magic use ability",
                                                                     extraordinary_powers.levitation);
        ++detail_count;
    }
    if (extraordinary_powers.strength) {
        magic_item->true_details[detail_count] = str_alloc_formatted("strength - %i times/day (upon wielder only)",
                                                                     extraordinary_powers.strength);
        ++detail_count;
    }
    if (extraordinary_powers.telekinesis) {
        magic_item->true_details[detail_count] = str_alloc_formatted("telekinesis, 2,500 gp wt maximum - %i times/day, 1 round each use",
                                                                     extraordinary_powers.telekinesis);
        ++detail_count;
    }
    if (extraordinary_powers.telepathy) {
        magic_item->true_details[detail_count] = str_alloc_formatted("telepathy, 6\" range - %i times/day",
                                                                     extraordinary_powers.telepathy);
        ++detail_count;
    }
    if (extraordinary_powers.teleportation) {
        magic_item->true_details[detail_count] = str_alloc_formatted("teleportation - %i times/day, 6,000 gp wt maximum, 2 segments to activate",
                                                                     extraordinary_powers.teleportation);
        ++detail_count;
    }
    if (extraordinary_powers.xray_vision) {
        magic_item->true_details[detail_count] = str_alloc_formatted("X-ray vision, 4\" range - %i times/day, 1 turn per use",
                                                                     extraordinary_powers.xray_vision);
        ++detail_count;
    }
    assert(detail_count < detail_capacity);
    
    if (has_special_purpose) {
        ego += 5;
        char const *special_purpose;
        score = roll("1d100", rnd);
        if (score <= 10) {
            if (0 == strcmp(alignment, "neutral")) {
                special_purpose = "preserve the balance by defeating/slaying powerful beings of extreme alignment (LG, LE, CG, CE)";
            } else {
                special_purpose = "defeat/slay diametrically opposed alignment";
            }
        } else if (score <= 20) {
            special_purpose = "kill clerics";
        } else if (score <= 30) {
            special_purpose = "kill fighters";
        } else if (score <= 40) {
            special_purpose = "kill magic-users";
        } else if (score <= 50) {
            special_purpose = "kill thieves";
        } else if (score <= 55) {
            special_purpose = "kill bards/monks";
        } else if (score <= 65) {
            special_purpose = "overthrow law and/or chaos";
        } else if (score <= 75) {
            special_purpose = "slay good and/or evil";
        } else {
            special_purpose = "slay non-human monsters";
        }
        magic_item->true_details[detail_count] = str_alloc_formatted("special purpose: %s",
                                                                     special_purpose);
        ++detail_count;
        assert(detail_count < detail_capacity);
        
        char const *special_purpose_power;
        char const *format = "special purpose power: %s on a hit unless save vs magic";
        score = roll("1d100", rnd);
        if (score <= 10) {
            special_purpose_power = "blindness for 2-12 rounds";
        } else if (score <= 20) {
            special_purpose_power = "confusion for 2-12 rounds";
        } else if (score <= 25) {
            special_purpose_power = "disintegrate";
        } else if (score <= 55) {
            special_purpose_power = "fear for 1-4 rounds";
        } else if (score <= 65) {
            special_purpose_power = "insanity for 1-4 rounds";
        } else if (score <= 80) {
            special_purpose_power = "paralysis for 1-4 rounds";
        } else {
            special_purpose_power = "+2 on all saving throws, -1 on each die of damage sustained";
            format = "special purpose power: %s";
        }
        magic_item->true_details[detail_count] = str_alloc_formatted(format,
                                                                     special_purpose_power);
        ++detail_count;
        assert(detail_count < detail_capacity);
    }
    
    // languages
    if (intelligence >= 14) {
        int max_score = 100;
        int rolls = 1;
        int language_count = 0;
        int min_language_count = 0;
        do {
            score = dice_roll(dice_make(1, max_score), rnd, NULL);
            --rolls;
            if (score <= 40) {
                language_count += 1;
            } else if (score <= 70) {
                language_count += 2;
            } else if (score <= 85) {
                language_count += 3;
            } else if (score <= 95) {
                language_count += 4;
            } else if (score <= 99) {
                language_count += 5;
            } else {
                language_count = 0;
                min_language_count = 6;
                max_score = 99;
                rolls = 2;
            }
        } while (rolls > 0);
        if (language_count < min_language_count) {
            language_count = min_language_count;
        }
        ego += (language_count + 1 /* alignment language */ + 1) / 2;
        
        char const *languages[10];
        char const *prefix = "speaks ";
        size_t languages_size = strlen(prefix);
        for (int i = 0; i < language_count; ++i) {
            languages[i] = determine_language(rnd, languages, i);
            languages_size += strlen(languages[i]);
        }
        char const *separator = ", ";
        languages_size += (strlen(separator) * language_count);
        languages_size += strlen(alignment);
        languages_size += sizeof('\0');
        char *languages_detail = calloc_or_die(sizeof(char), languages_size);
        strcat(languages_detail, prefix);
        for (int i = 0; i < language_count; ++i) {
            if (i) {
                strcat(languages_detail, separator);
            }
            strcat(languages_detail, languages[i]);
        }
        strcat(languages_detail, separator);
        strcat(languages_detail, alignment);
        magic_item->true_details[detail_count] = languages_detail;
        ++detail_count;
        assert(detail_count < detail_capacity);
    }
    
    magic_item->true_details[detail_count] = str_alloc_formatted("personality strength %i (ego %i)",
                                                                 (intelligence + ego), ego);
    ++detail_count;
    assert(detail_count < detail_capacity);
}


static void
generate_teeth_of_dahlver_nar(struct magic_item *magic_item, struct rnd *rnd)
{
    int tooth_number = roll("1d32", rnd);
    
    magic_item->experience_points = 0;
    magic_item->true_value_in_cp = gp_to_cp(5000);
    magic_item->true_description = str_alloc_formatted("tooth of Dahlver-Nar (#%i)",
                                                       tooth_number);
}


void
magic_item_initialize(struct magic_item *magic_item)
{
    memset(magic_item, 0, sizeof(struct magic_item));
}
