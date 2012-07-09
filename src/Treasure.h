#ifndef TREASURE_H_INCLUDED
#define TREASURE_H_INCLUDED


#include "MagicItem.h"


struct Gem;
struct Jewelry;
struct MagicItem;
struct Dice;
struct TreasureMap;
struct TreasureType;


struct Treasure {
  struct TreasureType *type;
  int copper;
  int silver;
  int electrum;
  int gold;
  int platinum;
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
                                   struct Dice *dice, 
                                   int count, 
                                   PossibleMagicItems possibleMagicItems);

void generateMapsForTreasure(struct Treasure *treasure, 
                             struct Dice *dice, 
                             int count);

void initializeTreasure(struct Treasure *treasure);

int treasureValue_cp(struct Treasure *treasure);


#endif /* #ifndef TREASURE_H_INCLUDED */
