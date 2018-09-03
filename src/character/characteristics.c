#include "characteristics.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <base/base.h>
#include <mechanics/mechanics.h>


static int
special_NPC_roll(struct rnd *rnd,
                 enum ability_flag flags,
                 enum ability_flag flag);


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
    for (int i = 0; i < 6; ++i) {
        int characteristic = roll("3d6", rnd);
        if (characteristic > characteristics->strength) {
            characteristics->strength = characteristic;
        }
    }
    for (int i = 0; i < 6; ++i) {
        int characteristic = roll("3d6", rnd);
        if (characteristic > characteristics->intelligence) {
            characteristics->intelligence = characteristic;
        }
    }
    for (int i = 0; i < 6; ++i) {
        int characteristic = roll("3d6", rnd);
        if (characteristic > characteristics->wisdom) {
            characteristics->wisdom = characteristic;
        }
    }
    for (int i = 0; i < 6; ++i) {
        int characteristic = roll("3d6", rnd);
        if (characteristic > characteristics->dexterity) {
            characteristics->dexterity = characteristic;
        }
    }
    for (int i = 0; i < 6; ++i) {
        int characteristic = roll("3d6", rnd);
        if (characteristic > characteristics->constitution) {
            characteristics->constitution = characteristic;
        }
    }
    for (int i = 0; i < 6; ++i) {
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
                                  enum ability_flag flags)
{
    struct characteristics *characteristics = calloc_or_die(1, sizeof(struct characteristics));
    characteristics->strength = special_NPC_roll(rnd, flags, ability_flag_strength);
    characteristics->intelligence = special_NPC_roll(rnd, flags, ability_flag_intelligence);
    characteristics->wisdom = special_NPC_roll(rnd, flags, ability_flag_wisdom);
    characteristics->dexterity = special_NPC_roll(rnd, flags, ability_flag_dexterity);
    characteristics->constitution = special_NPC_roll(rnd, flags, ability_flag_constitution);
    characteristics->charisma = special_NPC_roll(rnd, flags, ability_flag_charisma);
    return characteristics;
}


int
characteristics_compare(struct characteristics const *first,
                        struct characteristics const *second)
{
    return characteristics_total(second)
         - characteristics_total(first);
}


void
characteristics_free(struct characteristics *characteristics)
{
    free_or_die(characteristics);
}


int
characteristics_total(struct characteristics const *characteristics)
{
    return characteristics->strength
         + characteristics->intelligence
         + characteristics->wisdom
         + characteristics->dexterity
         + characteristics->constitution
         + characteristics->charisma;
}


static int
special_NPC_roll(struct rnd *rnd,
                 enum ability_flag flags,
                 enum ability_flag flag)
{
    struct dice threeD6 = dice_make(3, 6);
    return flag & flags ? dice_roll_and_adjust_upwards(threeD6, rnd)
                        : dice_roll(threeD6, rnd, NULL);
}
