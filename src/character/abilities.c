#include "abilities.h"

#include <stdlib.h>
#include <base/base.h>
#include <mechanics/mechanics.h>

#include "xmalloc.h"


static int
special_NPC_roll(struct rnd *rnd,
                 enum ability_flag flags,
                 enum ability_flag flag);


struct abilities *
abilities_alloc(struct rnd *rnd)
{
    struct abilities *abilities = xcalloc(1, sizeof(struct abilities));
    abilities->strength = xroll("3d6", rnd);
    abilities->intelligence = xroll("3d6", rnd);
    abilities->wisdom = xroll("3d6", rnd);
    abilities->dexterity = xroll("3d6", rnd);
    abilities->constitution = xroll("3d6", rnd);
    abilities->charisma = xroll("3d6", rnd);
    return abilities;
}


struct abilities *
abilities_alloc_general_NPC(struct rnd *rnd)
{
    struct abilities *abilities = xcalloc(1, sizeof(struct abilities));
    struct xdice threeD6 = xdice_make(3, 6);
    abilities->strength = xdice_roll_with_average_scoring(threeD6, rnd);
    abilities->intelligence = xdice_roll_with_average_scoring(threeD6, rnd);
    abilities->wisdom = xdice_roll_with_average_scoring(threeD6, rnd);
    abilities->dexterity = xdice_roll_with_average_scoring(threeD6, rnd);
    abilities->constitution = xdice_roll_with_average_scoring(threeD6, rnd);
    abilities->charisma = xdice_roll_with_average_scoring(threeD6, rnd);
    return abilities;
}


struct abilities *
abilities_alloc_method_3(struct rnd *rnd)
{
    struct abilities *abilities = xcalloc(1, sizeof(struct abilities));
    for (int i = 0; i < 6; ++i) {
        int ability = xroll("3d6", rnd);
        if (ability > abilities->strength) {
            abilities->strength = ability;
        }
    }
    for (int i = 0; i < 6; ++i) {
        int ability = xroll("3d6", rnd);
        if (ability > abilities->intelligence) {
            abilities->intelligence = ability;
        }
    }
    for (int i = 0; i < 6; ++i) {
        int ability = xroll("3d6", rnd);
        if (ability > abilities->wisdom) {
            abilities->wisdom = ability;
        }
    }
    for (int i = 0; i < 6; ++i) {
        int ability = xroll("3d6", rnd);
        if (ability > abilities->dexterity) {
            abilities->dexterity = ability;
        }
    }
    for (int i = 0; i < 6; ++i) {
        int ability = xroll("3d6", rnd);
        if (ability > abilities->constitution) {
            abilities->constitution = ability;
        }
    }
    for (int i = 0; i < 6; ++i) {
        int ability = xroll("3d6", rnd);
        if (ability > abilities->charisma) {
            abilities->charisma = ability;
        }
    }
    return abilities;
}


struct abilities *
abilities_alloc_special_NPC(struct rnd *rnd,
                            enum ability_flag flags)
{
    struct abilities *abilities = xcalloc(1, sizeof(struct abilities));
    abilities->strength = special_NPC_roll(rnd, flags, ability_flag_strength);
    abilities->intelligence = special_NPC_roll(rnd, flags, ability_flag_intelligence);
    abilities->wisdom = special_NPC_roll(rnd, flags, ability_flag_wisdom);
    abilities->dexterity = special_NPC_roll(rnd, flags, ability_flag_dexterity);
    abilities->constitution = special_NPC_roll(rnd, flags, ability_flag_constitution);
    abilities->charisma = special_NPC_roll(rnd, flags, ability_flag_charisma);
    return abilities;
}


int
abilities_compare(struct abilities const *first,
                  struct abilities const *second)
{
    return abilities_total(second)
         - abilities_total(first);
}


void
abilities_free(struct abilities *abilities)
{
    free(abilities);
}


int
abilities_total(struct abilities const *abilities)
{
    return abilities->strength
         + abilities->intelligence
         + abilities->wisdom
         + abilities->dexterity
         + abilities->constitution
         + abilities->charisma;
}


static int
special_NPC_roll(struct rnd *rnd,
                 enum ability_flag flags,
                 enum ability_flag flag)
{
    struct xdice threeD6 = xdice_make(3, 6);
    return flag & flags ? xdice_roll_and_adjust_upwards(threeD6, rnd)
                        : xdice_roll(threeD6, rnd, NULL);
}
