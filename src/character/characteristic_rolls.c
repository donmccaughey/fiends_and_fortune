#include "characteristic_rolls.h"

#include <base/base.h>


#include "characteristics.h"


static void
general_NPC(struct characteristic_rolls *rolls, struct rnd *rnd);

static void
method_1(struct characteristic_rolls *rolls, struct rnd *rnd);

static void
method_2(struct characteristic_rolls *rolls, struct rnd *rnd);

static void
method_3(struct characteristic_rolls *rolls, struct rnd *rnd);

static void
method_4(struct characteristic_rolls *rolls, struct rnd *rnd);

static void
simple_method(struct characteristic_rolls *rolls, struct rnd *rnd);

static void
special_NPC(struct characteristic_rolls *rolls,
            enum characteristic_flag flags,
            struct rnd *rnd);


struct characteristic_rolls *
characteristic_rolls_alloc(struct rnd *rnd,
                           enum characteristic_generation_method method,
                           enum characteristic_flag flags)
{
    struct characteristic_rolls *rolls = calloc_or_die(1, sizeof(struct characteristic_rolls));
    switch (method) {
        case characteristic_generation_method_simple:
            rolls->type = characteristic_rolls_type_six_characteristics;
            simple_method(rolls, rnd);
            break;
        case characteristic_generation_method_1:
            rolls->type = characteristic_rolls_type_six_scores;
            method_1(rolls, rnd);
            break;
        case characteristic_generation_method_2:
            rolls->type = characteristic_rolls_type_six_scores;
            method_2(rolls, rnd);
            break;
        case characteristic_generation_method_3:
            rolls->type = characteristic_rolls_type_six_characteristics;
            method_3(rolls, rnd);
            break;
        case characteristic_generation_method_4:
            rolls->type = characteristic_rolls_type_twelve_sets_of_six_characteristics;
            method_4(rolls, rnd);
            break;
        case characteristic_generation_method_general_NPC:
            rolls->type = characteristic_rolls_type_six_characteristics;
            general_NPC(rolls, rnd);
            break;
        case characteristic_generation_method_special_NPC:
            rolls->type = characteristic_rolls_type_six_characteristics;
            special_NPC(rolls, flags, rnd);
            break;
        default:
            rolls->type = characteristic_rolls_type_none;
            break;
    }
    return rolls;
}


void
characteristic_rolls_free(struct characteristic_rolls *rolls)
{
    free_or_die(rolls);
}


static void
general_NPC(struct characteristic_rolls *rolls, struct rnd *rnd)
{
    int *scores = characteristics_alloc(rnd,
                                        characteristic_generation_method_general_NPC,
                                        characteristic_flag_none);
    rolls->characteristics.strength = scores[0];
    rolls->characteristics.intelligence = scores[1];
    rolls->characteristics.wisdom = scores[2];
    rolls->characteristics.dexterity = scores[3];
    rolls->characteristics.constitution = scores[4];
    rolls->characteristics.charisma = scores[5];
    characteristics_free(scores);
}


static void
method_1(struct characteristic_rolls *rolls, struct rnd *rnd)
{
    int *scores = characteristics_alloc(rnd,
                                        characteristic_generation_method_1,
                                        characteristic_flag_none);
    memcpy(rolls->scores, scores, 6 * sizeof(scores[0]));
    characteristics_free(scores);
}


static void
method_2(struct characteristic_rolls *rolls, struct rnd *rnd)
{
    int *scores = characteristics_alloc(rnd,
                                        characteristic_generation_method_2,
                                        characteristic_flag_none);
    memcpy(rolls->scores, scores, 6 * sizeof(scores[0]));
    characteristics_free(scores);
}


static void
method_3(struct characteristic_rolls *rolls, struct rnd *rnd)
{
    int *scores = characteristics_alloc(rnd,
                                        characteristic_generation_method_3,
                                        characteristic_flag_none);
    rolls->characteristics.strength = scores[0];
    rolls->characteristics.intelligence = scores[1];
    rolls->characteristics.wisdom = scores[2];
    rolls->characteristics.dexterity = scores[3];
    rolls->characteristics.constitution = scores[4];
    rolls->characteristics.charisma = scores[5];
    characteristics_free(scores);
}


static void
method_4(struct characteristic_rolls *rolls, struct rnd *rnd)
{
    int *scores = characteristics_alloc(rnd,
                                        characteristic_generation_method_4,
                                        characteristic_flag_none);
    for (int i = 0; i < 12; ++i) {
        int j = 6 * i;
        rolls->characteristics_sets[i].strength = scores[0 + j];
        rolls->characteristics_sets[i].intelligence = scores[1 + j];
        rolls->characteristics_sets[i].wisdom = scores[2 + j];
        rolls->characteristics_sets[i].dexterity = scores[3 + j];
        rolls->characteristics_sets[i].constitution = scores[4 + j];
        rolls->characteristics_sets[i].charisma = scores[5 + j];
    }
    characteristics_free(scores);
}


static void
simple_method(struct characteristic_rolls *rolls, struct rnd *rnd)
{
    int *scores = characteristics_alloc(rnd,
                                        characteristic_generation_method_simple,
                                        characteristic_flag_none);
    rolls->characteristics.strength = scores[0];
    rolls->characteristics.intelligence = scores[1];
    rolls->characteristics.wisdom = scores[2];
    rolls->characteristics.dexterity = scores[3];
    rolls->characteristics.constitution = scores[4];
    rolls->characteristics.charisma = scores[5];
    characteristics_free(scores);
}


static void
special_NPC(struct characteristic_rolls *rolls,
            enum characteristic_flag flags,
            struct rnd *rnd)
{
    int *scores = characteristics_alloc(rnd,
                                        characteristic_generation_method_special_NPC,
                                        flags);
    rolls->characteristics.strength = scores[0];
    rolls->characteristics.intelligence = scores[1];
    rolls->characteristics.wisdom = scores[2];
    rolls->characteristics.dexterity = scores[3];
    rolls->characteristics.constitution = scores[4];
    rolls->characteristics.charisma = scores[5];
    characteristics_free(scores);
}
