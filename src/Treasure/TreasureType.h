#ifndef TREASURETYPE_H_INCLUDED
#define TREASURETYPE_H_INCLUDED


#include <stddef.h>

#include "Boolean.h"


struct Dice;
struct Treasure;
struct TreasureType;


char *describeTreasureType(struct TreasureType *treasureType, 
                           Boolean includeHeader);

void generateTreasure(struct TreasureType *treasureType, 
                      struct Treasure *treasure, 
                      struct Dice *dice,
                      int individualCount);

struct TreasureType *treasureTypeByLetter(char letter);


#endif /* #ifndef TREASURETYPE_H_INCLUDED */
