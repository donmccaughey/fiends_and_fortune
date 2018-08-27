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
                      uint32_t characteristic_flags)
{
    int *characteristics;
    switch (method) {
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
            characteristics = calloc_or_die(characteristic_count, characteristic_size);
            for (size_t i = 0; i < characteristic_count; ++i) {
                for (int j = 0; j < method_3_roll_count; ++j) {
                    int characteristic = roll("3d6", rnd);
                    if (characteristic > characteristics[i]) {
                        characteristics[i] = characteristic;
                    }
                }
            }
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
            characteristics = calloc_or_die(characteristic_count, characteristic_size);
            for (size_t i = 0; i < characteristic_count; ++i) {
                characteristics[i] = dice_roll_with_average_scoring(dice_make(3, 6), rnd);
            }
            break;
        case characteristic_generation_method_special_NPC:
        {
            characteristics = calloc_or_die(characteristic_count,
                                            characteristic_size);
            for (int i = 0; i < characteristic_count; ++i) {
                uint32_t characteristicFlag = STRENGTH << i;
                if (characteristic_flags & characteristicFlag) {
                    characteristics[i] = dice_roll_and_adjust_upwards(dice_make(3, 6), rnd);
                } else {
                    characteristics[i] = roll("3d6", rnd);
                }
            }
            break;
        }
        default:
            characteristics = calloc_or_die(characteristic_count, characteristic_size);
            for (size_t i = 0; i < characteristic_count; ++i) {
                characteristics[i] = roll("3d6", rnd);
            }
            break;
    }
    return characteristics;
}


void
characteristics_free(int *characteristics)
{
    free_or_die(characteristics);
}
