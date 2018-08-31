#include "characteristics.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <base/base.h>
#include <mechanics/mechanics.h>


static size_t const characteristic_count = 6;
static size_t const characteristic_size = sizeof(int);
static size_t const method_2_characteristic_count = characteristic_count * 2;
static int const method_3_roll_count = 6;
static size_t const method_4_character_count = 12;
static size_t const method_4_characteristic_count = characteristic_count
                                                  * method_4_character_count;


static int
compare_characteristics(void const *characteristic1,
                        void const *characteristic2);

static int
compare_characteristic_sets(void const *characteristic_set1,
                            void const *characteristic_set2);

static int
special_NPC_roll(struct rnd *rnd,
                 enum characteristic_flag flags,
                 enum characteristic_flag flag);


static int
compare_characteristics(void const *characteristic1,
                        void const *characteristic2)
{
    return *((int const *) characteristic2) - *((int const *) characteristic1);
}


static int
compare_characteristic_sets(void const *characteristic_set1,
                            void const *characteristic_set2)
{
    int const *set1 = (int const *) characteristic_set1;
    int const *set2 = (int const *) characteristic_set2;

    int total1 = 0;
    int total2 = 0;
    for (int i = 0; i < characteristic_count; ++i) {
        total1 += set1[i];
        total2 += set2[i];
    }
    return total2 - total1;
}


int *
characteristics_alloc(struct rnd *rnd,
                      enum characteristic_generation_method method,
                      enum characteristic_flag flags)
{
    int *characteristics = NULL;
    switch (method) {
        case characteristic_generation_method_invalid:
            UNEXPECTED_CASE(characteristic_generation_method_invalid);
            break;
        case characteristic_generation_method_simple:
            UNEXPECTED_CASE(characteristic_generation_method_simple);
            break;
        case characteristic_generation_method_1:
            characteristics = calloc_or_die(characteristic_count, characteristic_size);
            for (size_t i = 0; i < characteristic_count; ++i) {
                characteristics[i] = dice_roll_and_drop_lowest(dice_make(4, 6), rnd);
            }
            qsort(characteristics, characteristic_count, characteristic_size,
                  compare_characteristics);
            break;
        case characteristic_generation_method_2:
            characteristics = calloc_or_die(method_2_characteristic_count,
                                            characteristic_size);
            for (size_t i = 0; i < method_2_characteristic_count; ++i) {
                characteristics[i] = roll("3d6", rnd);
            }
            qsort(characteristics, method_2_characteristic_count, characteristic_size,
                  compare_characteristics);
            break;
        case characteristic_generation_method_3:
            UNEXPECTED_CASE(characteristic_generation_method_3);
            break;
        case characteristic_generation_method_4:
            characteristics = calloc_or_die(method_4_characteristic_count,
                                            characteristic_size);
            for (size_t i = 0; i < method_4_characteristic_count; ++i) {
                characteristics[i] = roll("3d6", rnd);
            }
            qsort(characteristics,
                  method_4_character_count,
                  characteristic_size * characteristic_count,
                  compare_characteristic_sets);
            break;
        case characteristic_generation_method_general_NPC:
            UNEXPECTED_CASE(characteristic_generation_method_general_NPC);
            break;
        case characteristic_generation_method_special_NPC:
            UNEXPECTED_CASE(characteristic_generation_method_special_NPC);
            break;
    }
    return characteristics;
}


struct characteristics *
characteristics_alloc_general_NPC(struct rnd *rnd)
{
    struct characteristics *characteristics = calloc_or_die(1, sizeof(struct characteristics));
    struct dice threeD6 = dice_make(3, 6);
    characteristics->strength = dice_roll_with_average_scoring(threeD6, rnd);
    characteristics->intelligence = dice_roll_with_average_scoring(threeD6, rnd);
    characteristics->wisdom = dice_roll_with_average_scoring(threeD6, rnd);
    characteristics->dexterity = dice_roll_with_average_scoring(threeD6, rnd);
    characteristics->constitution = dice_roll_with_average_scoring(threeD6, rnd);
    characteristics->charisma = dice_roll_with_average_scoring(threeD6, rnd);
    return characteristics;
}


struct characteristics *
characteristics_alloc_method_3(struct rnd *rnd)
{
    struct characteristics *characteristics = calloc_or_die(1, sizeof(struct characteristics));
    for (int i = 0; i < method_3_roll_count; ++i) {
        int characteristic = roll("3d6", rnd);
        if (characteristic > characteristics->strength) {
            characteristics->strength = characteristic;
        }
    }
    for (int i = 0; i < method_3_roll_count; ++i) {
        int characteristic = roll("3d6", rnd);
        if (characteristic > characteristics->intelligence) {
            characteristics->intelligence = characteristic;
        }
    }
    for (int i = 0; i < method_3_roll_count; ++i) {
        int characteristic = roll("3d6", rnd);
        if (characteristic > characteristics->wisdom) {
            characteristics->wisdom = characteristic;
        }
    }
    for (int i = 0; i < method_3_roll_count; ++i) {
        int characteristic = roll("3d6", rnd);
        if (characteristic > characteristics->dexterity) {
            characteristics->dexterity = characteristic;
        }
    }
    for (int i = 0; i < method_3_roll_count; ++i) {
        int characteristic = roll("3d6", rnd);
        if (characteristic > characteristics->constitution) {
            characteristics->constitution = characteristic;
        }
    }
    for (int i = 0; i < method_3_roll_count; ++i) {
        int characteristic = roll("3d6", rnd);
        if (characteristic > characteristics->charisma) {
            characteristics->charisma = characteristic;
        }
    }
    return characteristics;
}


struct characteristics *
characteristics_alloc_simple_method(struct rnd *rnd)
{
    struct characteristics *characteristics = calloc_or_die(1, sizeof(struct characteristics));
    characteristics->strength = roll("3d6", rnd);
    characteristics->intelligence = roll("3d6", rnd);
    characteristics->wisdom = roll("3d6", rnd);
    characteristics->dexterity = roll("3d6", rnd);
    characteristics->constitution = roll("3d6", rnd);
    characteristics->charisma = roll("3d6", rnd);
    return characteristics;
}


struct characteristics *
characteristics_alloc_special_NPC(struct rnd *rnd,
                                  enum characteristic_flag flags)
{
    struct characteristics *characteristics = calloc_or_die(1, sizeof(struct characteristics));
    characteristics->strength = special_NPC_roll(rnd, flags, characteristic_flag_strength);
    characteristics->intelligence = special_NPC_roll(rnd, flags, characteristic_flag_intelligence);
    characteristics->wisdom = special_NPC_roll(rnd, flags, characteristic_flag_wisdom);
    characteristics->dexterity = special_NPC_roll(rnd, flags, characteristic_flag_dexterity);
    characteristics->constitution = special_NPC_roll(rnd, flags, characteristic_flag_constitution);
    characteristics->charisma = special_NPC_roll(rnd, flags, characteristic_flag_charisma);
    return characteristics;
}


void
characteristics_free(int *characteristics)
{
    free_or_die(characteristics);
}


void
characteristics_struct_free(struct characteristics *characteristics)
{
    free_or_die(characteristics);
}


static int
special_NPC_roll(struct rnd *rnd,
                 enum characteristic_flag flags,
                 enum characteristic_flag flag)
{
    struct dice threeD6 = dice_make(3, 6);
    return flag & flags ? dice_roll_and_adjust_upwards(threeD6, rnd)
                        : dice_roll(threeD6, rnd, NULL);
}
