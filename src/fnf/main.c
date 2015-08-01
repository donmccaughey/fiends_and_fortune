#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/alloc_or_die.h"
#include "common/dice.h"
#include "common/rnd.h"

#include "character/character.h"

#include "treasure/coins.h"
#include "treasure/gem.h"
#include "treasure/jewelry.h"
#include "treasure/magic_item.h"
#include "treasure/treasure.h"
#include "treasure/treasure_map.h"
#include "treasure/treasure_type.h"

#include "dungeon.h"
#include "text_graph.h"


static void check(FILE *out, char const *constantNumber);
static void enumerateTreasureItems(struct treasure *treasure, FILE *out);
static void generateCharacter(struct rnd *rnd,
                              FILE *out,
                              char const *methodName);
static void generateEachTreasure(struct rnd *rnd, FILE *out);
static void generateMap(struct rnd *rnd, FILE *out);
static void generateMagicItems(struct rnd *rnd, FILE *out, int count);
static void generateRandomDungeon(struct rnd *rnd, FILE *out);
static void generateSampleDungeon(struct rnd *rnd, FILE *out);
static void generateTreasureType(struct rnd *rnd, FILE *out, char letter);
static void generateTreasureTypeTable(FILE *out);
static void usage(int argc, char *argv[]);


static void check(FILE *out, char const *constantNumber)
{
    uint32_t fixedValue = (uint32_t)strtoul(constantNumber, NULL, 10);
    struct rnd *fake_rnd = rnd_alloc_fake_fixed(fixedValue);
    
    generateTreasureTypeTable(out);
    generateMap(fake_rnd, out);
    generateEachTreasure(fake_rnd, out);
    generateSampleDungeon(fake_rnd, out);
    generateCharacter(fake_rnd, out, "simple");
    generateCharacter(fake_rnd, out, "method1");
    generateCharacter(fake_rnd, out, "method2");
    generateCharacter(fake_rnd, out, "method3");
    generateCharacter(fake_rnd, out, "method4");
    generateCharacter(fake_rnd, out, "generalnpc");
    generateCharacter(fake_rnd, out, "specialnpc");
    
    rnd_free(fake_rnd);
}


static void enumerateTreasureItems(struct treasure *treasure, FILE *out)
{
    if (treasure->gems_count) {
        fprintf(out, "  Gems: --------------------------------\n");
        for (int i = 0; i < treasure->gems_count; ++i) {
            fprintf(out, "    %2i  %s\n", i + 1, treasure->gems[i].true_description);
        }
    }
    
    if (treasure->jewelry_count) {
        fprintf(out, "  Jewelry: -----------------------------\n");
        for (int i = 0; i < treasure->jewelry_count; ++i) {
            fprintf(out, "    %2i  %s\n",
                    i + 1, treasure->jewelry[i].true_description);
        }
    }
    
    if (treasure->maps_count) {
        fprintf(out, "  Maps: --------------------------------\n");
        for (int i = 0; i < treasure->maps_count; ++i) {
            fprintf(out, "    %2i  %s\n", i + 1, treasure->maps[i].true_description);
        }
    }
    
    if (treasure->magic_items_count) {
        fprintf(out, "  Magic Items: -------------------------\n");
        for (int i = 0; i < treasure->magic_items_count; ++i) {
            fprintf(out, "    %2i  %s\n",
                    i + 1, treasure->magic_items[i].true_description);
            if (treasure->magic_items[i].true_details) {
                int j = 0;
                while (treasure->magic_items[i].true_details[j]) {
                    fprintf(out, "            %s\n",
                            treasure->magic_items[i].true_details[j]);
                    ++j;
                }
            }
        }
    }
}


static void generateCharacter(struct rnd *rnd,
                              FILE *out,
                              char const *methodName)
{
    enum characteristic_generation_method method = characteristic_generation_method_simple;
    uint32_t specialCharacteristics = STRENGTH;
    if (0 == strcasecmp("method1", methodName)) {
        method = characteristic_generation_method_1;
    } else if (0 == strcasecmp("method2", methodName)) {
        method = characteristic_generation_method_2;
    } else if (0 == strcasecmp("method3", methodName)) {
        method = characteristic_generation_method_3;
    } else if (0 == strcasecmp("method4", methodName)) {
        method = characteristic_generation_method_4;
    } else if (0 == strcasecmp("generalnpc", methodName)) {
        method = characteristic_generation_method_general_NPC;
    } else if (0 == strcasecmp("specialnpc", methodName)) {
        method = characteristic_generation_method_special_NPC;
    } else {
        methodName = "simple";
    }
    
    int *characteristics = alloc_characteristics(rnd, method, specialCharacteristics);
    if (   method == characteristic_generation_method_1
        || method == characteristic_generation_method_2)
    {
        fprintf(out, "Character (%s): -------------------------\n", methodName);
        fprintf(out, "(%s)\n", characteristic_generation_method_description(method));
        for (int i = 0; i < 6; ++i) {
            fprintf(out, "  %2i) %i\n", i + 1, characteristics[i]);
        }
    } else if (method == characteristic_generation_method_4) {
        fprintf(out, "Possible Characters (%s): -------------------------\n", methodName);
        fprintf(out, "(%s)\n", characteristic_generation_method_description(method));
        fprintf(out, "                 %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i\n",
                1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
        fprintf(out, "                 %2s   %2s   %2s   %2s   %2s   %2s   %2s   %2s   %2s   %2s   %2s   %2s\n",
                "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--");
        fprintf(out, "  Strength:      %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i\n",
                characteristics[0], characteristics[0 + 6], characteristics[0 + 12],
                characteristics[0 + 18], characteristics[0 + 24], characteristics[0 + 30],
                characteristics[0 + 36], characteristics[0 + 42], characteristics[0 + 48],
                characteristics[0 + 54], characteristics[0 + 60], characteristics[0 + 66]);
        fprintf(out, "  Intelligence:  %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i\n",
                characteristics[1], characteristics[1 + 6], characteristics[1 + 12],
                characteristics[1 + 18], characteristics[1 + 24], characteristics[1 + 30],
                characteristics[1 + 36], characteristics[1 + 42], characteristics[1 + 48],
                characteristics[1 + 54], characteristics[1 + 60], characteristics[1 + 66]);
        fprintf(out, "  Wisdom:        %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i\n",
                characteristics[2], characteristics[2 + 6], characteristics[2 + 12],
                characteristics[2 + 18], characteristics[2 + 24], characteristics[2 + 30],
                characteristics[2 + 36], characteristics[2 + 42], characteristics[2 + 48],
                characteristics[2 + 54], characteristics[2 + 60], characteristics[2 + 66]);
        fprintf(out, "  Dexterity:     %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i\n",
                characteristics[3], characteristics[3 + 6], characteristics[3 + 12],
                characteristics[3 + 18], characteristics[3 + 24], characteristics[3 + 30],
                characteristics[3 + 36], characteristics[3 + 42], characteristics[3 + 48],
                characteristics[3 + 54], characteristics[3 + 60], characteristics[3 + 66]);
        fprintf(out, "  Constitution:  %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i\n",
                characteristics[4], characteristics[4 + 6], characteristics[4 + 12],
                characteristics[4 + 18], characteristics[4 + 24], characteristics[4 + 30],
                characteristics[4 + 36], characteristics[4 + 42], characteristics[4 + 48],
                characteristics[4 + 54], characteristics[4 + 60], characteristics[4 + 66]);
        fprintf(out, "  Charisma:      %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i\n",
                characteristics[5], characteristics[5 + 6], characteristics[5 + 12],
                characteristics[5 + 18], characteristics[5 + 24], characteristics[5 + 30],
                characteristics[5 + 36], characteristics[5 + 42], characteristics[5 + 48],
                characteristics[5 + 54], characteristics[5 + 60], characteristics[5 + 66]);
    } else {
        fprintf(out, "Character (%s): -------------------------\n", methodName);
        fprintf(out, "(%s)\n", characteristic_generation_method_description(method));
        fprintf(out, "  Strength:     %2i\n", characteristics[0]);
        fprintf(out, "  Intelligence: %2i\n", characteristics[1]);
        fprintf(out, "  Wisdom:       %2i\n", characteristics[2]);
        fprintf(out, "  Dexterity:    %2i\n", characteristics[3]);
        fprintf(out, "  Constitution: %2i\n", characteristics[4]);
        fprintf(out, "  Charisma:     %2i\n", characteristics[5]);
    }
    free_or_die(characteristics);
}


static void generateEachTreasure(struct rnd *rnd, FILE *out)
{
    for (char letter = 'A'; letter <= 'Z'; ++letter) {
        generateTreasureType(rnd, out, letter);
    }
}


static void generateMap(struct rnd *rnd, FILE *out)
{
    struct treasure_map treasureMap;
  treasure_map_initialize(&treasureMap);
  treasure_map_generate(&treasureMap, rnd);
    fprintf(out, "%s\n", treasureMap.true_description);
    enumerateTreasureItems(&treasureMap.treasure, out);
  treasure_map_finalize(&treasureMap);
}


static void generateMagicItems(struct rnd *rnd, FILE *out, int count)
{
    struct magic_item magicItem;
    
    fprintf(out, "Magic Items:\n");
    for (int i = 0; i < count; ++i) {
        magic_item_initialize(&magicItem);
        magic_item_generate(&magicItem, rnd, ANY_MAGIC_ITEM);
        fprintf(out, "  %4i %s\n", (i + 1), magicItem.true_description);
        if (magicItem.true_details) {
            int j = 0;
            while (magicItem.true_details[j]) {
                fprintf(out, "           %s\n", magicItem.true_details[j]);
                ++j;
            }
        }
        magic_item_finalize(&magicItem);
    }
    fprintf(out, "\n");
}


static void generateRandomDungeon(struct rnd *rnd, FILE *out)
{
    struct dungeon dungeon;
    dungeon_initialize(&dungeon);
    
    dungeon_generate(&dungeon, rnd);
    dungeon_graph_level_using_text(&dungeon, 1, out);
    
    char const **descriptions = dungeon_alloc_area_descriptions(&dungeon);
    char const **current = descriptions;
    fprintf(out, "\nDungeon areas:\n");
    while (*current) {
        fprintf(out, "\t%s\n", *current);
        ++current;
    }
    free_or_die(descriptions);
    
    dungeon_finalize(&dungeon);
}


static void generateSampleDungeon(struct rnd *rnd, FILE *out)
{
    struct dungeon dungeon;
    dungeon_initialize(&dungeon);
    
    dungeon_generate_small(&dungeon);
    dungeon_graph_level_using_text(&dungeon, 1, out);
    
    char const **descriptions = dungeon_alloc_area_descriptions(&dungeon);
    char const **current = descriptions;
    fprintf(out, "\nDungeon areas:\n");
    while (*current) {
        fprintf(out, "\t%s\n", *current);
        ++current;
    }
    free_or_die(descriptions);
    
    dungeon_finalize(&dungeon);
}


static void generateTreasureType(struct rnd *rnd, FILE *out, char letter)
{
    struct treasure treasure;
    int individualCount;
    
    fprintf(out, "Treasure type %c: ", letter);
    
    if (letter >= 'J' && letter <= 'N') {
        individualCount = roll("1d10", rnd);
    } else {
        individualCount = 0;
    }
    
    treasure_initialize(&treasure);
    treasure_type_generate(treasure_type_by_letter(letter), rnd, individualCount, &treasure);
    
    char *description = treasure_alloc_description(&treasure);
    int value_cp = treasure_value_in_cp(&treasure);
    char *value_gp = coins_alloc_gp_cp_description(value_cp);
    fprintf(out, "%s (total %s)\n", description, value_gp);
    free_or_die(value_gp);
    free_or_die(description);
    enumerateTreasureItems(&treasure, out);
    
    treasure_finalize(&treasure);
}


static void generateTreasureTypeTable(FILE *out)
{
    for (char letter = 'A'; letter <= 'Z'; ++letter) {
        struct treasure_type *treasureType = treasure_type_by_letter(letter);
        char *description = treasure_type_alloc_description(treasureType, letter == 'A');
        fprintf(out, "%s", description);
        free_or_die(description);
    }
}


int main(int argc, char *argv[])
{
    FILE *out = stdout;
    struct rnd *rnd = global_rnd;
    
    fprintf(out, "Fiends and Fortunes\n");
    
    if (argc < 2) {
        usage(argc, argv);
    } else if (strcasecmp(argv[1], "character") == 0) {
        generateCharacter(rnd, out, (argc >= 3) ? argv[2] : "simple");
    } else if (strcasecmp(argv[1], "check") == 0) {
        check(out, (argc >= 3) ? argv[2] : "0");
    } else if (strcasecmp(argv[1], "dungeon") == 0) {
        if (argc >= 3 && strcasecmp(argv[2], "small")) {
            generateSampleDungeon(rnd, out);
        } else {
            generateRandomDungeon(rnd, out);
        }
    } else if (strcasecmp(argv[1], "each") == 0) {
        generateEachTreasure(rnd, out);
    } else if (strcasecmp(argv[1], "magic") == 0) {
        int count = 10;
        if (argc >= 3) {
            count = (int) strtol(argv[2], NULL, 10);
        }
        generateMagicItems(rnd, out, count);
    } else if (strcasecmp(argv[1], "map") == 0) {
        generateMap(rnd, out);
    } else if (strcasecmp(argv[1], "table") == 0) {
        generateTreasureTypeTable(out);
    } else if (argv[1][0] >= 'A' && argv[1][0] <= 'Z' && argv[1][1] == '\0') {
        generateTreasureType(rnd, out, argv[1][0]);
    } else if (argv[1][0] >= 'a' && argv[1][0] <= 'z' && argv[1][1] == '\0') {
        char letter = argv[1][0] - 'a' + 'A';
        generateTreasureType(rnd, out, letter);
    } else {
        usage(argc, argv);
    }
    
    fprintf(out, "\n");
    alloc_count_is_zero_or_die();
    return EXIT_SUCCESS;
}


static void usage(int argc, char *argv[])
{
    fprintf(stderr, "Usage: %s ACTION\n", basename(argv[0]));
    fprintf(stderr, "\n");
    fprintf(stderr, "Available actions:\n");
    fprintf(stderr, "   character [METHOD]  Generate a character where METHOD is\n");
    fprintf(stderr, "                         `method1', `method2', `method3', `method4',\n");
    fprintf(stderr, "                         `generalnpc', `specialnpc' or `simple'\n");
    fprintf(stderr, "                         (default `simple')\n");
    fprintf(stderr, "   check [N]           Run tests where N is the \"constant\"\n");
    fprintf(stderr, "                         random number (default 0)\n");
    fprintf(stderr, "   dungeon [TYPE]      Generate a dungeon where TYPE is\n");
    fprintf(stderr, "                         `random' or `small' (default `random')\n");
    fprintf(stderr, "   each                Generate one of each treasure\n");
    fprintf(stderr, "   magic [COUNT]       Generate COUNT magic items (default 10)\n");
    fprintf(stderr, "   map                 Generate one treasure map\n");
    fprintf(stderr, "   table               Generate the treasure type table\n");
    fprintf(stderr, "   LETTER              Generate the treasure type for LETTER (A-Z)\n");
}
