#include "TreasureMap.h"

#include <stdlib.h>
#include <string.h>

#include "alloc_or_die.h"
#include "dice.h"
#include "Gem.h"
#include "Jewelry.h"
#include "MagicItem.h"


enum TreasureMapType {
  FalseMap,
  MapToMonetaryTreasure,
  MapToMagicTreasure,
  MapToCombinedHoard
};


static char const *treasureMapTypes[] = {
  "treasure",
  "monetary treasure",
  "magic treasure",
  "combined hoard"
};


static char const *compassDirections[] = {
  "north",
  "northeast",
  "east",
  "southeast",
  "south",
  "southwest",
  "west",
  "northwest"
};


static void generateCombinedHoard(struct Treasure *treasure, 
                                  struct rnd *rnd);

static void generateMagicTreasure(struct Treasure *treasure, 
                                  struct rnd *rnd);

static void generateMagicTreasure_1to5_ItemAndPotions(struct Treasure *treasure, 
                                                      struct rnd *rnd);

static void generateMagicTreasure_6to8_TwoItems(struct Treasure *treasure, 
                                                struct rnd *rnd);

static void generateMagicTreasure_9to12_SwordArmorAndWeapon(struct Treasure *treasure, 
                                                            struct rnd *rnd);

static void generateMagicTreasure_13to14_ThreeItems(struct Treasure *treasure, 
                                                    struct rnd *rnd);

static void generateMagicTreasure_15to18_PotionsAndScrolls(struct Treasure *treasure, 
                                                           struct rnd *rnd);

static void generateMagicTreasure_19_FourItems(struct Treasure *treasure, 
                                               struct rnd *rnd);

static void generateMagicTreasure_20_FiveItems(struct Treasure *treasure, 
                                               struct rnd *rnd);

static void generateMonetaryTreasure(struct Treasure *treasure, 
                                     struct rnd *rnd);

static void generateMonetaryTreasure_1to2_CopperAndSilver(struct Treasure *treasure, 
                                                          struct rnd *rnd);

static void generateMonetaryTreasure_3to5_Electrum(struct Treasure *treasure, 
                                                   struct rnd *rnd);

static void generateMonetaryTreasure_6to10_Gold(struct Treasure *treasure, 
                                                struct rnd *rnd);

static void generateMonetaryTreasure_11to12_Platinum(struct Treasure *treasure, 
                                                     struct rnd *rnd);

static void generateMonetaryTreasure_13to15_Gems(struct Treasure *treasure, 
                                                 struct rnd *rnd);

static void generateMonetaryTreasure_16to17_Jewelry(struct Treasure *treasure, 
                                                    struct rnd *rnd);


void finalizeTreasureMap(struct TreasureMap *treasureMap)
{
  finalizeTreasure(&treasureMap->treasure);
  free(treasureMap->trueDescription);
}


static void generateCombinedHoard(struct Treasure *treasure, 
                                  struct rnd *rnd)
{
  int dieRoll = roll(rnd, "1d100");
  if (dieRoll <= 20) {
    generateMonetaryTreasure_1to2_CopperAndSilver(treasure, rnd);
    generateMagicTreasure_1to5_ItemAndPotions(treasure, rnd);
  } else if (dieRoll <= 40) {
    generateMonetaryTreasure_6to10_Gold(treasure, rnd);
    generateMagicTreasure_1to5_ItemAndPotions(treasure, rnd);
  } else if (dieRoll <= 55) {
    generateMonetaryTreasure_3to5_Electrum(treasure, rnd);
    generateMonetaryTreasure_6to10_Gold(treasure, rnd);
    generateMagicTreasure_1to5_ItemAndPotions(treasure, rnd);
    generateMagicTreasure_15to18_PotionsAndScrolls(treasure, rnd);
  } else if (dieRoll <= 65) {
    generateMonetaryTreasure_1to2_CopperAndSilver(treasure, rnd);
    generateMonetaryTreasure_3to5_Electrum(treasure, rnd);
    generateMonetaryTreasure_6to10_Gold(treasure, rnd);
    generateMagicTreasure_9to12_SwordArmorAndWeapon(treasure, rnd);
    generateMagicTreasure_13to14_ThreeItems(treasure, rnd);
  } else if (dieRoll <= 75) {
    generateMonetaryTreasure_6to10_Gold(treasure, rnd);
    generateMonetaryTreasure_11to12_Platinum(treasure, rnd);
    generateMagicTreasure_6to8_TwoItems(treasure, rnd);
    generateMagicTreasure_15to18_PotionsAndScrolls(treasure, rnd);
  } else if (dieRoll <= 80) {
    generateMonetaryTreasure_3to5_Electrum(treasure, rnd);
    generateMonetaryTreasure_6to10_Gold(treasure, rnd);
    generateMonetaryTreasure_11to12_Platinum(treasure, rnd);
    generateMonetaryTreasure_16to17_Jewelry(treasure, rnd);
    generateMagicTreasure_1to5_ItemAndPotions(treasure, rnd);
    generateMagicTreasure_9to12_SwordArmorAndWeapon(treasure, rnd);
  } else if (dieRoll <= 85) {
    generateMonetaryTreasure_1to2_CopperAndSilver(treasure, rnd);
    generateMonetaryTreasure_3to5_Electrum(treasure, rnd);
    generateMonetaryTreasure_6to10_Gold(treasure, rnd);
    generateMonetaryTreasure_11to12_Platinum(treasure, rnd);
    generateMonetaryTreasure_13to15_Gems(treasure, rnd);
    generateMonetaryTreasure_16to17_Jewelry(treasure, rnd);
    /* TODO: map to generateMagicTreasure_1to5_ItemAndPotions() */
  } else if (dieRoll <= 90) {
    generateMonetaryTreasure_1to2_CopperAndSilver(treasure, rnd);
    generateMonetaryTreasure_3to5_Electrum(treasure, rnd);
    generateMonetaryTreasure_6to10_Gold(treasure, rnd);
    generateMonetaryTreasure_11to12_Platinum(treasure, rnd);
    generateMonetaryTreasure_13to15_Gems(treasure, rnd);
    generateMonetaryTreasure_16to17_Jewelry(treasure, rnd);
    /* TODO: map to generateMagicTreasure_19_FourItems() */
  } else if (dieRoll <= 96) {
    /* TODO: map to generateMonetaryTreasure_1to2_CopperAndSilver(), 
                    generateMonetaryTreasure_3to5_Electrum() */
    generateMagicTreasure_20_FiveItems(treasure, rnd);
  } else {
    /* TODO: map to generateMonetaryTreasure_11to12_Platinum(), 
                    generateMonetaryTreasure_13to15_Gems() */
    generateMagicTreasure_15to18_PotionsAndScrolls(treasure, rnd);
    generateMagicTreasure_20_FiveItems(treasure, rnd);
  }
}


static void generateMagicTreasure(struct Treasure *treasure, 
                                  struct rnd *rnd)
{
  int dieRoll = roll(rnd, "1d20");
  if (dieRoll <= 5) {
    generateMagicTreasure_1to5_ItemAndPotions(treasure, rnd);
  } else if (dieRoll <= 8) {
    generateMagicTreasure_6to8_TwoItems(treasure, rnd);
  } else if (dieRoll <= 12) {
    generateMagicTreasure_9to12_SwordArmorAndWeapon(treasure, rnd);
  } else if (dieRoll <= 14) {
    generateMagicTreasure_13to14_ThreeItems(treasure, rnd);
  } else if (dieRoll <= 18) {
    generateMagicTreasure_15to18_PotionsAndScrolls(treasure, rnd);
  } else if (dieRoll <= 19) {
    generateMagicTreasure_19_FourItems(treasure, rnd);
  } else {
    generateMagicTreasure_20_FiveItems(treasure, rnd);
  }
}


static void generateMagicTreasure_1to5_ItemAndPotions(struct Treasure *treasure, 
                                                      struct rnd *rnd)
{
  generateMagicItemsForTreasure(treasure, rnd, 1, ANY_MAGIC_ITEM);
  generateMagicItemsForTreasure(treasure, rnd, 4, POTION);
}


static void generateMagicTreasure_6to8_TwoItems(struct Treasure *treasure, 
                                                struct rnd *rnd)
{
  generateMagicItemsForTreasure(treasure, rnd, 2, ANY_MAGIC_ITEM);
}


static void generateMagicTreasure_9to12_SwordArmorAndWeapon(struct Treasure *treasure, 
                                                            struct rnd *rnd)
{
  generateMagicItemsForTreasure(treasure, rnd, 1, SWORD);
  generateMagicItemsForTreasure(treasure, rnd, 1, ARMOR_SHIELD);
  generateMagicItemsForTreasure(treasure, rnd, 1, MISC_WEAPON);
}


static void generateMagicTreasure_13to14_ThreeItems(struct Treasure *treasure, 
                                                    struct rnd *rnd)
{
  generateMagicItemsForTreasure(treasure, rnd, 3,
                                (ANY_MAGIC_ITEM & ~SWORD & ~POTION));
}


static void generateMagicTreasure_15to18_PotionsAndScrolls(struct Treasure *treasure, 
                                                           struct rnd *rnd)
{
  generateMagicItemsForTreasure(treasure, rnd, 6, POTION);
  generateMagicItemsForTreasure(treasure, rnd, 6, SCROLL);
}


static void generateMagicTreasure_19_FourItems(struct Treasure *treasure, 
                                               struct rnd *rnd)
{
  generateMagicItemsForTreasure(treasure, rnd, 1, RING);
  generateMagicItemsForTreasure(treasure, rnd, 1, ROD_STAFF_WAND);
  generateMagicItemsForTreasure(treasure, rnd, 2, ANY_MAGIC_ITEM);
}


static void generateMagicTreasure_20_FiveItems(struct Treasure *treasure, 
                                               struct rnd *rnd)
{
  generateMagicItemsForTreasure(treasure, rnd, 1, ROD_STAFF_WAND);
  generateMagicItemsForTreasure(treasure, rnd, 1, MISC_MAGIC);
  generateMagicItemsForTreasure(treasure, rnd, 3, ANY_MAGIC_ITEM);
}


static void generateMonetaryTreasure(struct Treasure *treasure, 
                                     struct rnd *rnd)
{
  int multipleRolls = 0;
  int dieRollMax = 20;
  do {
    int dieRoll = rollDice(rnd, 1, dieRollMax);
    if (dieRoll <= 2) {
      generateMonetaryTreasure_1to2_CopperAndSilver(treasure, rnd);
      --multipleRolls;
    } else if (dieRoll <= 5) {
      generateMonetaryTreasure_3to5_Electrum(treasure, rnd);
      --multipleRolls;
    } else if (dieRoll <= 10) {
      generateMonetaryTreasure_6to10_Gold(treasure, rnd);
      --multipleRolls;
    } else if (dieRoll <= 12) {
      generateMonetaryTreasure_11to12_Platinum(treasure, rnd);
      --multipleRolls;
    } else if (dieRoll <= 15) {
      generateMonetaryTreasure_13to15_Gems(treasure, rnd);
      --multipleRolls;
    } else if (dieRoll <= 17) {
      generateMonetaryTreasure_16to17_Jewelry(treasure, rnd);
      --multipleRolls;
    } else if (dieRoll <= 18) {
      dieRollMax = 17;
      multipleRolls = 2;
    } else if (dieRoll <= 19) {
      dieRollMax = 17;
      multipleRolls = 3;
    } else {
      generateMonetaryTreasure_1to2_CopperAndSilver(treasure, rnd);
      generateMonetaryTreasure_3to5_Electrum(treasure, rnd);
      generateMonetaryTreasure_6to10_Gold(treasure, rnd);
      generateMonetaryTreasure_11to12_Platinum(treasure, rnd);
      generateMonetaryTreasure_13to15_Gems(treasure, rnd);
      generateMonetaryTreasure_16to17_Jewelry(treasure, rnd);
    }
  } while (multipleRolls > 0);
}


static void generateMonetaryTreasure_1to2_CopperAndSilver(struct Treasure *treasure, 
                                                          struct rnd *rnd)
{
  treasure->copper += roll(rnd, "2d4") * 10000;
  treasure->silver += (roll(rnd, "1d4") + 1) * 10000;
}


static void generateMonetaryTreasure_3to5_Electrum(struct Treasure *treasure, 
                                                   struct rnd *rnd)
{
  treasure->electrum += roll(rnd, "5d6") * 1000;
}


static void generateMonetaryTreasure_6to10_Gold(struct Treasure *treasure, 
                                                struct rnd *rnd)
{
  treasure->gold += roll(rnd, "3d6") * 1000;
}


static void generateMonetaryTreasure_11to12_Platinum(struct Treasure *treasure, 
                                                     struct rnd *rnd)
{
  treasure->platinum += roll(rnd, "5d4") * 100;
}


static void generateMonetaryTreasure_13to15_Gems(struct Treasure *treasure, 
                                                 struct rnd *rnd)
{
  int count = roll(rnd, "1d10") * 10;
  treasure->gems = reallocarray_or_die(treasure->gems,
                                       treasure->gemsCount + count,
                                       sizeof(struct Gem));
  for (int i = 0; i < count; ++i) {
    int j = treasure->gemsCount + i;
    initializeGem(&treasure->gems[j]);
    generateGem(&treasure->gems[j], rnd);
  }
  treasure->gemsCount += count;
}


static void generateMonetaryTreasure_16to17_Jewelry(struct Treasure *treasure, 
                                                    struct rnd *rnd)
{
  int count = roll(rnd, "5d10");
  treasure->jewelry = reallocarray_or_die(treasure->jewelry,
                                          treasure->jewelryCount + count,
                                          sizeof(struct Jewelry));
  for (int i = 0; i < count; ++i) {
    int j = treasure->jewelryCount + i;
    initializeJewelry(&treasure->jewelry[j]);
    generateJewelry(&treasure->jewelry[j], rnd);
  }
  treasure->jewelryCount += count;
}


void generateTreasureMap(struct TreasureMap *treasureMap, struct rnd *rnd)
{
  enum TreasureMapType treasureMapType;
  int dieRoll = roll(rnd, "1d100");
  if (dieRoll <= 5) {
    treasureMapType = FalseMap;
    treasureMap->isFalse = true;
    /* TODO: generate description of false treasure */
  } else if (dieRoll <= 70) {
    treasureMapType = MapToMonetaryTreasure;
    generateMonetaryTreasure(&treasureMap->treasure, rnd);
  } else if (dieRoll <= 90) {
    treasureMapType = MapToMagicTreasure;
    generateMagicTreasure(&treasureMap->treasure, rnd);
  } else {
    treasureMapType = MapToCombinedHoard;
    generateCombinedHoard(&treasureMap->treasure, rnd);
  }
  
  int miles;
  dieRoll = roll(rnd, "1D100");
  if (dieRoll <= 20) {
    miles = 0;
  } else if (dieRoll <= 60) {
    miles = roll(rnd, "1D4") + 4;
  } else if (dieRoll <= 90) {
    miles = roll(rnd, "1D4") * 10;
  } else {
    miles = roll(rnd, "1D10") * 50;
  }
  
  int compassDirection = roll(rnd, "1D8-1");
  if (miles) {
    char const *disposition;
    dieRoll = roll(rnd, "1D100");
    if (dieRoll <= 10) {
      disposition = "buried and unguarded";
    } else if (dieRoll <= 20) {
      disposition = "hidden in water";
    } else if (dieRoll <= 70) {
      disposition = "guarded in a lair";
    } else if (dieRoll <= 80) {
      disposition = "somewhere in a ruins";
    } else if (dieRoll <= 90) {
      disposition = "in a burial crypt";
    } else {
      disposition = "secreted in a town";
    }
    
    asprintf_or_die(&treasureMap->trueDescription,
                    "%smap to %s of %s %i miles to the %s, %s",
                    (treasureMap->isFalse ? "false " : ""),
                    treasureMapTypes[treasureMapType],
                    describeTreasure(&treasureMap->treasure),
                    miles,
                    compassDirections[compassDirection],
                    disposition);
  } else {
    asprintf_or_die(&treasureMap->trueDescription,
                    "%smap to %s of %s in nearby labyrinth to the %s",
                    (treasureMap->isFalse ? "false " : ""),
                    treasureMapTypes[treasureMapType],
                    describeTreasure(&treasureMap->treasure),
                    compassDirections[compassDirection]);
  }
}


void initializeTreasureMap(struct TreasureMap *treasureMap)
{
  memset(treasureMap, 0, sizeof(struct TreasureMap));
}
