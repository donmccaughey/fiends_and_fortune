#ifndef TREASURE_TREASUREMAP_H_INCLUDED
#define TREASURE_TREASUREMAP_H_INCLUDED


#include <stdbool.h>

#include "Treasure.h"


struct rnd;


struct TreasureMap {
    bool isFalse;
    struct Treasure treasure;
    char *trueDescription;
};


void finalizeTreasureMap(struct TreasureMap *treasureMap);

void generateTreasureMap(struct TreasureMap *treasureMap, struct rnd *rnd);

void initializeTreasureMap(struct TreasureMap *treasureMap);


#endif /* #ifndef TREASURE_TREASUREMAP_H_INCLUDED */
