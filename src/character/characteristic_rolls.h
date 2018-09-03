#ifndef FNF_CHARACTER_CHARACTERISTIC_ROLLS_H_INCLUDED
#define FNF_CHARACTER_CHARACTERISTIC_ROLLS_H_INCLUDED


#include <character/ability_flag.h>
#include <character/characteristic_generation_method.h>
#include <character/characteristics.h>


enum characteristic_rolls_type {
    characteristic_rolls_type_none = 0,
    characteristic_rolls_type_six_characteristics,
    characteristic_rolls_type_six_scores,
    characteristic_rolls_type_twelve_sets_of_six_characteristics,
};


struct rnd;


struct characteristic_rolls {
    enum characteristic_rolls_type type;
    union {
        struct characteristics characteristics;
        struct characteristics characteristics_sets[12];
        int scores[6];
    };
};


struct characteristic_rolls *
characteristic_rolls_alloc(struct rnd *rnd,
                           enum characteristic_generation_method method,
                           enum ability_flag flags);

void
characteristic_rolls_free(struct characteristic_rolls *rolls);


#endif
