#ifndef FNF_CHARACTER_CHARACTER_H_INCLUDED
#define FNF_CHARACTER_CHARACTER_H_INCLUDED


#include <character/characteristic_flag.h>
#include <character/characteristic_generation_method.h>
#include <character/characteristic_rolls.h>
#include <character/characteristic_scores.h>
#include <character/characteristics.h>


struct character {
    int strength;
    int intelligence;
    int wisdom;
    int dexterity;
    int constitution;
    int charisma;
};


#endif
