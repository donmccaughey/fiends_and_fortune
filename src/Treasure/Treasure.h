#ifndef TREASURE_H_INCLUDED
#define TREASURE_H_INCLUDED


#include "coins.h"
#include "MagicItem.h"


struct Gem;
struct Jewelry;
struct MagicItem;
struct rnd;
struct TreasureMap;
struct TreasureType;


struct Treasure {
    struct TreasureType *type;
    struct coins coins;
    struct Gem *gems;
    int gemsCount;
    struct Jewelry *jewelry;
    int jewelryCount;
    struct TreasureMap *maps;
    int mapsCount;
    struct MagicItem *magicItems;
    int magicItemsCount;
};


char *describeTreasure(struct Treasure *treasure);

void finalizeTreasure(struct Treasure *treasure);

void generateMagicItemsForTreasure(struct Treasure *treasure,
                                   struct rnd *rnd,
                                   int count,
                                   PossibleMagicItems possibleMagicItems);

void generateMapsForTreasure(struct Treasure *treasure,
                             struct rnd *rnd,
                             int count);

void initializeTreasure(struct Treasure *treasure);

int treasureValue_cp(struct Treasure *treasure);


#endif
