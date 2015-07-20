#ifndef TREASURETYPE_H_INCLUDED
#define TREASURETYPE_H_INCLUDED


#include <stdbool.h>
#include <stddef.h>


struct rnd;
struct Treasure;
struct TreasureType;


char *describeTreasureType(struct TreasureType *treasureType, 
                           bool includeHeader);

void generateTreasure(struct TreasureType *treasureType, 
                      struct Treasure *treasure, 
                      struct rnd *rnd,
                      int individualCount);

struct TreasureType *treasureTypeByLetter(char letter);


#endif /* #ifndef TREASURETYPE_H_INCLUDED */
