#ifndef FNF_CHARACTER_CHARACTER_H_INCLUDED
#define FNF_CHARACTER_CHARACTER_H_INCLUDED


#include <character/abilities.h>
#include <character/ability_flag.h>
#include <character/ability_scores.h>
#include <character/ability_sets.h>
#include <character/characteristic_generation_method.h>
#include <character/characteristic_rolls.h>


struct character {
    int strength;
    int intelligence;
    int wisdom;
    int dexterity;
    int constitution;
    int charisma;
};


#endif
