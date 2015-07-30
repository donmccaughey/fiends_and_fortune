#include "language.h"

#include <string.h>

#include "common/dice.h"
#include "common/fail.h"
#include "common/rnd.h"


static bool
contains(char const *set[], size_t set_count, char const *s)
{
    if ( ! set) return false;

    for (int i = 0; i < set_count; ++i) {
        if (0 == strcasecmp(set[i], s)) return true;
    }
    return false;
}


char const *
language_determine(struct rnd *rnd,
                   char const *exclude[],
                   size_t exclude_count)
{
    static struct {
        int percent;
        char const *language;
    } const language_table[] = {
        {  1 -  0, "Brownie" },
        {  3 -  1, "Bugbear" },
        {  4 -  3, "Centaur" },
        {  5 -  4, "Black Dragon" },
        {  6 -  5, "Blue Dragon" },
        {  7 -  6, "Brass Dragon" },
        {  8 -  7, "Bronze Dragon" },
        {  9 -  8, "Copper Dragon" },
        { 10 -  9, "Gold Dragon" },
        { 11 - 10, "Green Dragon" },
        { 12 - 11, "Red Dragon" },
        { 13 - 12, "Silver Dragon" },
        { 14 - 13, "White Dragon" },
        { 15 - 14, "Dryad" },
        { 20 - 15, "Dwarvish" },
        { 25 - 20, "Elvish" },
        { 26 - 25, "Ettin" },
        { 27 - 26, "Gargoyle" },
        { 28 - 27, "Cloud Giant" },
        { 29 - 28, "Fire Giant" },
        { 30 - 29, "Frost Giant" },
        { 33 - 30, "Hill Giant" },
        { 34 - 33, "Stone Giant" },
        { 35 - 34, "Storm Giant" },
        { 39 - 35, "Goblin" },
        { 40 - 39, "Gnoll" },
        { 44 - 40, "Gnome" },
        { 49 - 44, "Halfling" },
        { 51 - 49, "Hobgoblin" },
        { 54 - 51, "Kobold" },
        { 55 - 54, "Lammasu" },
        { 58 - 55, "Lizard Man" },
        { 59 - 58, "Manticore" },
        { 60 - 59, "Medusian" },
        { 61 - 60, "Minotaur" },
        { 62 - 61, "Guardian Naga" },
        { 63 - 62, "Spirit Naga" },
        { 64 - 63, "Water Naga" },
        { 65 - 64, "Nixie" },
        { 66 - 65, "Nymph" },
        { 70 - 66, "Ogrish" },
        { 71 - 70, "Ogre Magian" },
        { 76 - 71, "Orcish" },
        { 77 - 76, "Pixie" },
        { 78 - 77, "Salamander" },
        { 79 - 78, "Satyr" },
        { 80 - 79, "Shedu" },
        { 81 - 80, "Sprite" },
        { 82 - 81, "Sylph" },
        { 83 - 82, "Titan" },
        { 84 - 83, "Troll" },
        { 85 - 84, "Xorn" }
    };
    size_t const language_table_count = sizeof language_table
                                      / sizeof language_table[0];
    
    int total = 0;
    for (int i = 0; i < language_table_count; ++i) {
        if (!contains(exclude, exclude_count, language_table[i].language)) {
            total += language_table[i].percent;
        }
    }
    
    int score = dice_roll(dice_make(1, total), rnd, NULL);
    int range = 0;
    for (int i = 0; i < language_table_count; ++i) {
        if (!contains(exclude, exclude_count, language_table[i].language)) {
            range += language_table[i].percent;
            if (score <= range) return language_table[i].language;
        }
    }
    fail("Did not match table entry (score=%i, range=%i)", score, range);
    return NULL;
}
