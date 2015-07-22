#include "TreasureType.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc_or_die.h"
#include "dice.h"
#include "Gem.h"
#include "Jewelry.h"
#include "MagicItem.h"
#include "Treasure.h"
#include "TreasureMap.h"
#include "unexpected.h"


struct CoinsGemsOrJewelry {
  bool isPossible;
  bool isPerIndividual;
  int percentChance;
  char const *amount;
  int multiplier;
};


struct MapsOrMagicType {
  int amount;
  char const *variableAmount;
  bool isMapPossible;
  PossibleMagicItems possibleMagicItems;
};


struct MapsOrMagic {
  bool isPossible;
  int percentChance;
  struct MapsOrMagicType types[6];
  int typeCount; 
};


struct TreasureType {
  char letter;
  struct CoinsGemsOrJewelry copper;
  struct CoinsGemsOrJewelry silver;
  struct CoinsGemsOrJewelry electrum;
  struct CoinsGemsOrJewelry gold;
  struct CoinsGemsOrJewelry platinum;
  struct CoinsGemsOrJewelry gems;
  struct CoinsGemsOrJewelry jewelry;
  struct MapsOrMagic mapsOrMagic;
};


static struct TreasureType treasureTypes[] = {
  {
    'A',
    { true, false, 25, "1D6", 1000 },
    { true, false, 30, "1D6", 1000 },
    { true, false, 35, "1D6", 1000 },
    { true, false, 40, "1D10", 1000 },
    { true, false, 25, "1D4", 100 },
    { true, false, 60, "4D10", 1 },
    { true, false, 50, "3D10", 1 },
    {
      true, 30, 
      {
        { 3, "", true, ANY_MAGIC_ITEM } 
      }, 
      1 
    }
  },
  {
    'B',
    { true, false, 50, "1D8", 1000 },
    { true, false, 25, "1D6", 1000 },
    { true, false, 25, "1D4", 1000 },
    { true, false, 25, "1D3", 1000 },
    { false, false, 0, "", 0 },
    { true, false, 30, "1D8", 1 },
    { true, false, 20, "1D4", 1 },
    {
      true, 10, 
      {
        { 1, "", false, MAGIC_WEAPON_OR_ARMOR } 
      }, 
      1 
    }
  },
  {
    'C',
    { true, false, 20, "1D12", 1000 },
    { true, false, 30, "1D6", 1000 },
    { true, false, 10, "1D4", 1000 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { true, false, 25, "1D6", 1 },
    { true, false, 20, "1D3", 1 },
    {
      true, 10, 
      {
        { 2, "", true, ANY_MAGIC_ITEM } 
      }, 
      1 
    }
  },
  {
    'D',
    { true, false, 10, "1D8", 1000 },
    { true, false, 15, "1D12", 1000 },
    { true, false, 15, "1D8", 1000 },
    { true, false, 50, "1D6", 1000 },
    { false, false, 0, "", 0 },
    { true, false, 30, "1D10", 1 },
    { true, false, 25, "1D6", 1 },
    {
      true, 15, 
      {
        { 2, "", true, ANY_MAGIC_ITEM },
        { 1, "", false, POTION } 
      }, 
      2
    }
  },
  {
    'E',
    { true, false, 5, "1D10", 1000 },
    { true, false, 25, "1D12", 1000 },
    { true, false, 25, "1D6", 1000 },
    { true, false, 25, "1D8", 1000 },
    { false, false, 0, "", 0 },
    { true, false, 15, "1D12", 1 },
    { true, false, 10, "1D8", 1 },
    {
      true, 25, 
      {
        { 3, "", true, ANY_MAGIC_ITEM },
        { 1, "", false, SCROLL } 
      }, 
      2
    }
  },
  {
    'F',
    { false, false, 0, "", 0 },
    { true, false, 10, "1D20", 1000 },
    { true, false, 15, "1D12", 1000 },
    { true, false, 40, "1D10", 1000 },
    { true, false, 35, "1D8", 100 },
    { true, false, 20, "3D10", 1 },
    { true, false, 10, "1D10", 1 },
    {
      true, 30, 
      {
        { 3, "", true, NON_WEAPON_MAGIC },
        { 1, "", false, POTION },
        { 1, "", false, SCROLL } 
      }, 
      3
    }
  },
  {
    'G',
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { true, false, 50, "10D4", 1000 },
    { true, false, 50, "1D20", 100 },
    { true, false, 30, "5D4", 1 },
    { true, false, 25, "1D10", 1 },
    {
      true, 35, 
      {
        { 4, "", true, ANY_MAGIC_ITEM },
        { 1, "", false, SCROLL } 
      }, 
      2
    }
  },
  {
    'H',
    { true, false, 25, "5D6", 1000 },
    { true, false, 40, "1D100", 1000 },
    { true, false, 40, "10D4", 1000 },
    { true, false, 55, "10D6", 1000 },
    { true, false, 25, "5D10", 100 },
    { true, false, 50, "1D100", 1 },
    { true, false, 50, "10D4", 1 },
    {
      true, 15, 
      {
        { 4, "", true, ANY_MAGIC_ITEM },
        { 1, "", false, POTION },
        { 1, "", false, SCROLL } 
      }, 
      3
    }
  },
  {
    'I',
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { true, false, 30, "3D6", 100 },
    { true, false, 55, "2D10", 1 },
    { true, false, 50, "1D12", 1 },
    {
      true, 15, 
      {
        { 1, "", true, ANY_MAGIC_ITEM } 
      }, 
      1 
    }
  },
  {
    'J',
    { true, true, 100, "3D8", 1 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, 0, {}, 0 }
  },
  {
    'K',
    { false, false, 0, "", 0 },
    { true, true, 100, "3D6", 1 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, 0, {}, 0 }
  },
  {
    'L',
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { true, true, 100, "2D6", 1 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, 0, {}, 0 }
  },
  {
    'M',
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { true, true, 100, "2D4", 1 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, 0, {}, 0 }
  },
  {
    'N',
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { true, true, 100, "1D6", 1 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, 0, {}, 0 }
  },
  {
    'O',
    { true, false, 25, "1D4", 1000 },
    { true, false, 20, "1D3", 1000 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, 0, {}, 0 }
  },
  {
    'P',
    { false, false, 0, "", 0 },
    { true, false, 30, "1D6", 1000 },
    { true, false, 25, "1D2", 1000 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, 0, {}, 0 }
  },
  {
    'Q',
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { true, false, 50, "1D4", 1 },
    { false, false, 0, "", 0 },
    { false, 0, {}, 0 }
  },
  {
    'R',
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { true, false, 40, "2D4", 1000 },
    { true, false, 50, "10D6", 100 },
    { true, false, 55, "4D8", 1 },
    { true, false, 45, "1D12", 1 },
    { false, 0, {}, 0 }
  },
  {
    'S',
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    {
      true, 40, 
      {
        { 0, "2D4", false, POTION } 
      }, 
      1 
    }
  },
  {
    'T',
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    {
      true, 50, 
      {
        { 0, "1D4", false, SCROLL } 
      }, 
      1 
    }
  },
  {
    'U',
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { true, false, 90, "10D8", 1 },
    { true, false, 80, "5D6", 1 },
    {
      true, 70, 
      {
        { 1, "", false, RING },
        { 1, "", false, ROD_STAFF_WAND },
        { 1, "", false, MISC_MAGIC },
        { 1, "", false, ARMOR_SHIELD },
        { 1, "", false, SWORD },
        { 1, "", false, MISC_WEAPON }
      }, 
      6
    }
  },
  {
    'V',
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    {
      true, 85, 
      {
        { 2, "", false, RING },
        { 2, "", false, ROD_STAFF_WAND },
        { 2, "", false, MISC_MAGIC },
        { 2, "", false, ARMOR_SHIELD },
        { 2, "", false, SWORD },
        { 2, "", false, MISC_WEAPON }
      }, 
      6
    }
  },
  {
    'W',
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { true, false, 60, "5D6", 1000 },
    { true, false, 15, "1D8", 100 },
    { true, false, 60, "10D8", 1 },
    { true, false, 50, "5D8", 1 },
    {
      true, 55, 
      {
        { 1, "", true, NO_MAGIC_ITEM }
      }, 
      1
    }
  },
  {
    'X',
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    {
      true, 60, 
      {
        { 1, "", false, MISC_MAGIC },
        { 1, "", false, POTION }
      }, 
      2
    }
  },
  {
    'Y',
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { true, false, 70, "2D6", 1000 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, false, 0, "", 0 },
    { false, 0, {}, 0 }
  },
  {
    'Z',
    { true, false, 20, "1D3", 1000 },
    { true, false, 25, "1D4", 1000 },
    { true, false, 25, "1D4", 1000 },
    { true, false, 30, "1D4", 1000 },
    { true, false, 30, "1D6", 100 },
    { true, false, 55, "10D6", 1 },
    { true, false, 50, "5D6", 1 },
    {
      true, 50, 
      {
        { 3, "", false, ANY_MAGIC_ITEM }
      }, 
      1
    }
  }
};


static char *describeCoinsGemsOrJewelry(struct CoinsGemsOrJewelry *coinsGemsOrJewelry);

static char *describeMapsOrMagic(struct MapsOrMagic *mapsOrMagic);

static void generateTreasureCoins(int *coins, 
                                  struct rnd *rnd,
                                  struct CoinsGemsOrJewelry *coinsType, 
                                  int individualCount);

static void generateTreasureGems(struct Treasure *treasure, struct rnd *rnd);

static void generateTreasureJewelry(struct Treasure *treasure, 
                                    struct rnd *rnd);

static void generateTreasureMapsOrMagic(struct Treasure *treasure, 
                                        struct rnd *rnd);

static void generateTreasureMapsOrMagicType(struct MapsOrMagicType *type, 
                                            struct Treasure *treasure, 
                                            struct rnd *rnd);

static char const *possibleMapsOrMagicName(bool isMapPossible,
                                           PossibleMagicItems possibleMagicItems);


char *describeTreasureType(struct TreasureType *treasureType, 
                           bool includeHeader)
{
  /*          1000       1000       1000       1000       100                          */
  /* Type    copper     silver    electrum     gold     platinum     gems      jewelry */
  /*  A     1-6 :25%   1-6 :30%   1-6 :35%   1-10:40%   1-4 :25%   4-40:60%   3-30:50% */
  /*                                                                                   */
  /*  J    3-24 pieces                                                                 */
  /*      per individual                                                               */
  static char const *header =
  "          |  1,000's  |  1,000's  |  1,000's  |  1,000's  |   100's   |           |           |    Maps   \n"
  "Treasure  |    of     |    of     |    of     |    of     |    of     |           |           |     or    \n"
  "  Type    |  Copper   |  Silver   | Electrum  |   Gold    | Platinum  |   Gems    |  Jewelry  |   Magic   \n"
  "----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------\n";
  char *letterFormat = "    %c    ";
  
  char *letter;
  char *copper = describeCoinsGemsOrJewelry(&treasureType->copper);
  char *silver = describeCoinsGemsOrJewelry(&treasureType->silver);
  char *electrum = describeCoinsGemsOrJewelry(&treasureType->electrum);
  char *gold = describeCoinsGemsOrJewelry(&treasureType->gold);
  char *platinum = describeCoinsGemsOrJewelry(&treasureType->platinum);
  char *gems = describeCoinsGemsOrJewelry(&treasureType->gems);
  char *jewelry = describeCoinsGemsOrJewelry(&treasureType->jewelry);
  char *mapsOrMagic = describeMapsOrMagic(&treasureType->mapsOrMagic);
  
  asprintf_or_die(&letter, letterFormat, treasureType->letter);
  
  char *descriptionFormat = "%s%s | %s | %s | %s | %s | %s | %s | %s | %s\n";
  char *description;
  asprintf_or_die(&description, descriptionFormat,
                  (includeHeader ? header : ""),
                  letter, copper, silver, electrum,
                  gold, platinum, gems, jewelry, mapsOrMagic);
  free(letter);
  free(copper);
  free(silver);
  free(electrum);
  free(gold);
  free(platinum);
  free(gems);
  free(jewelry);
  free(mapsOrMagic);
  
  return description;
}


static char *describeCoinsGemsOrJewelry(struct CoinsGemsOrJewelry *coinsGemsOrJewelry) {
  if (coinsGemsOrJewelry->isPossible) {
    if (coinsGemsOrJewelry->isPerIndividual) {
      int minAmount = dice_min_score(coinsGemsOrJewelry->amount);
      int minLength = (minAmount < 10 ? 1 : 2);
      
      int maxAmount = dice_max_score(coinsGemsOrJewelry->amount);
      int maxLength = (maxAmount < 10 ? 1 : 2);
      
      int dieRollLength = minLength + 1 /* dash */ + maxLength;
      char const *suffix = (dieRollLength < 4 ? " " : "");
      
      char *description;
      asprintf_or_die(&description, " %i-%i per%s",
                      minAmount, maxAmount, suffix);
      
      return description;
    } else {
      int minAmount = dice_min_score(coinsGemsOrJewelry->amount);
      int minLength = (minAmount < 10 ? 1 : 2);
      
      int maxAmount = dice_max_score(coinsGemsOrJewelry->amount);
      int maxLength = (maxAmount < 100 ? (maxAmount < 10 ? 1 : 2) : 3);
      
      int dieRollLength = minLength + 1 /* dash */ + maxLength;
      char const *prefix;
      char const *suffix;
      switch (dieRollLength) {
        case 3: prefix = " "; suffix = " "; break;
        case 4: prefix = " "; suffix = ""; break;
        case 5: prefix = ""; suffix = ""; break;
        default: 
          UNEXPECTED("dieRollLength = %i", dieRollLength);
          prefix = "("; suffix = ")"; 
          break;
      }
      
      char *dieRoll;
      asprintf_or_die(&dieRoll, "%i-%i", minAmount, maxAmount);
      
      char *description;
      asprintf_or_die(&description, "%s%s:%2i%%%s", prefix, dieRoll,
                      coinsGemsOrJewelry->percentChance, suffix);
      free(dieRoll);
      return description;
    }
  } else {
    return strdup_or_die("   nil   ");
  }
}


static char *describeMapsOrMagic(struct MapsOrMagic *mapsOrMagic) {
  if (mapsOrMagic->isPossible) {
    char *typeDescriptions[6] = {};
    
    for (int i = 0; i < mapsOrMagic->typeCount; ++i) {
      struct MapsOrMagicType *type = &mapsOrMagic->types[i];
      char const *typeName = possibleMapsOrMagicName(type->isMapPossible, 
                                                     type->possibleMagicItems);
      if (type->variableAmount[0]) {
        int minAmount = dice_min_score(type->variableAmount);
        int maxAmount = dice_max_score(type->variableAmount);
        asprintf_or_die(&typeDescriptions[i], "%i-%i %ss",
                        minAmount, maxAmount, typeName);
      } else if (   type->isMapPossible 
                 && type->possibleMagicItems == ANY_MAGIC_ITEM)
      {
        asprintf_or_die(&typeDescriptions[i], "any %i", type->amount);
      } else if (type->possibleMagicItems == NON_WEAPON_MAGIC) {
        asprintf_or_die(&typeDescriptions[i],
                        "any %i except sword or misc weapon", type->amount);
      } else if (type->possibleMagicItems == ANY_MAGIC_ITEM) {
        asprintf_or_die(&typeDescriptions[i], "any %i magic", type->amount);
      } else {
        char const *plural = (type->amount == 1) ? "" : "s";
        asprintf_or_die(&typeDescriptions[i], "%i %s%s",
                        type->amount, typeName, plural);
      }
    }
    
    char const *separator = ", ";
    size_t separatorLength = strlen(separator);
    size_t typeListSize = sizeof '\0';
    for (int i = 0; i < mapsOrMagic->typeCount; ++i) {
      if (i) {
        typeListSize += separatorLength;
      }
      typeListSize += strlen(typeDescriptions[i]);
    }
    char *typeList = calloc_or_die(typeListSize, sizeof(char));
    for (int i = 0; i < mapsOrMagic->typeCount; ++i) {
      if (i) {
        strcat(typeList, separator);
      }
      strcat(typeList, typeDescriptions[i]);
      free(typeDescriptions[i]);
    }
    
    char *description;
    asprintf_or_die(&description, "%s: %i%%",
                    typeList, mapsOrMagic->percentChance);
    free(typeList);
    return description;
  } else {
    return strdup_or_die("   nil   ");
  }
}


void generateTreasure(struct TreasureType *treasureType, 
                      struct Treasure *treasure, 
                      struct rnd *rnd,
                      int individualCount)
{
  treasure->type = treasureType;
  generateTreasureCoins(&treasure->copper, rnd, 
                        &treasureType->copper, individualCount);
  generateTreasureCoins(&treasure->silver, rnd, 
                        &treasureType->silver, individualCount);
  generateTreasureCoins(&treasure->electrum, rnd, 
                        &treasureType->electrum, individualCount);
  generateTreasureCoins(&treasure->gold, rnd, 
                        &treasureType->gold, individualCount);
  generateTreasureCoins(&treasure->platinum, rnd, 
                        &treasureType->platinum, individualCount);
  generateTreasureGems(treasure, rnd);
  generateTreasureJewelry(treasure, rnd);
  generateTreasureMapsOrMagic(treasure, rnd);
}


static void generateTreasureCoins(int *coins, 
                                  struct rnd *rnd,
                                  struct CoinsGemsOrJewelry *coinsType, 
                                  int individualCount)
{
  *coins = 0;
  if (coinsType->isPossible) {
    if (coinsType->isPerIndividual) {
      assert(individualCount > 0);
      for (int i = 0; i < individualCount; ++i) {
        *coins += roll(coinsType->amount, rnd);
      }
    } else {
      int percentRoll = roll("1d100", rnd);
      if (percentRoll <= coinsType->percentChance) {
        *coins = roll(coinsType->amount, rnd) * coinsType->multiplier;
      }
    }
  }
}


static void generateTreasureGems(struct Treasure *treasure, 
                                 struct rnd *rnd)
{
  treasure->gems = NULL;
  treasure->gemsCount = 0;
  if (treasure->type->gems.isPossible) {
    int percentRoll = roll("1d100", rnd);
    if (percentRoll <= treasure->type->gems.percentChance) {
      treasure->gemsCount = roll(treasure->type->gems.amount, rnd);
      treasure->gems = calloc_or_die(treasure->gemsCount, sizeof(struct Gem));
      for (int i = 0; i < treasure->gemsCount; ++i) {
        initializeGem(&treasure->gems[i]);
        generateGem(&treasure->gems[i], rnd);
      }
    }
  }
}


static void generateTreasureJewelry(struct Treasure *treasure, 
                                    struct rnd *rnd)
{
  treasure->jewelry = NULL;
  treasure->jewelryCount = 0;
  if (treasure->type->jewelry.isPossible) {
    int percentRoll = roll("1d100", rnd);
    if (percentRoll <= treasure->type->jewelry.percentChance) {
      treasure->jewelryCount = roll(treasure->type->jewelry.amount, rnd);
      treasure->jewelry = calloc_or_die(treasure->jewelryCount,
                                        sizeof(struct Jewelry));
      for (int i = 0; i < treasure->jewelryCount; ++i) {
        initializeJewelry(&treasure->jewelry[i]);
        generateJewelry(&treasure->jewelry[i], rnd);
      }
    }
  }
}


static void generateTreasureMapsOrMagic(struct Treasure *treasure, 
                                        struct rnd *rnd)
{
  if (treasure->type->mapsOrMagic.isPossible) {
    int percentRoll = roll("1d100", rnd);
    if (percentRoll <= treasure->type->mapsOrMagic.percentChance) {
      for (int i = 0; i < treasure->type->mapsOrMagic.typeCount; ++i) {
        struct MapsOrMagicType *type = &treasure->type->mapsOrMagic.types[i];
        generateTreasureMapsOrMagicType(type, treasure, rnd);
      }
    }
  }
}


static void generateTreasureMapsOrMagicType(struct MapsOrMagicType *type, 
                                            struct Treasure *treasure, 
                                            struct rnd *rnd)
{
  int amount = type->amount;
  bool isVariable = type->variableAmount[0];
  if (isVariable) {
    amount = roll(type->variableAmount, rnd);
  }
  
  int magicItemsCount = 0;
  int mapsCount = 0;
  for (int i = 0; i < amount; ++i) {
    if (type->isMapPossible) {
      bool isMagicItemPossible = type->possibleMagicItems & ANY_MAGIC_ITEM;
      if (isMagicItemPossible) {
        (roll("1d100", rnd) <= 10) ? ++mapsCount : ++magicItemsCount;
      } else {
        ++mapsCount;
      }
    } else {
      ++magicItemsCount;
    }
  }
  
  if (mapsCount) {
    generateMapsForTreasure(treasure, rnd, mapsCount);
  }
  if (magicItemsCount) {
    generateMagicItemsForTreasure(treasure, rnd, magicItemsCount, 
                                  type->possibleMagicItems);
  }
}


static char const *possibleMapsOrMagicName(bool isMapPossible, 
                                           PossibleMagicItems possibleMagicItems)
{
  if (isMapPossible) {
    switch (possibleMagicItems) {
      case NO_MAGIC_ITEM: return "map";
      case ANY_MAGIC_ITEM: return "any";
      case NON_WEAPON_MAGIC: return "map or non weapon magic";
      default: 
        UNEXPECTED("possibleMagicItems = %0x", possibleMagicItems); 
        return "(map or magic)";
    }
  } else {
    switch (possibleMagicItems) {
      case POTION: return "potion";
      case SCROLL: return "scroll";
      case RING: return "ring";
      case ROD_STAFF_WAND: return "rod/staff/wand";
      case MISC_MAGIC: return "misc magic";
      case ARMOR_SHIELD: return "armor/shield";
      case SWORD: return "sword";
      case MISC_WEAPON: return "misc weapon";
      
      case ANY_MAGIC_ITEM: return "any magic";
      case MAGIC_WEAPON_OR_ARMOR: return "sword, armor or misc weapon";
      default: 
        UNEXPECTED("possibleMagicItems = %0x", possibleMagicItems); 
        return "(magic)";
    }
  }
}


struct TreasureType *treasureTypeByLetter(char letter) {
  assert(letter >= 'A' && letter <= 'Z');
  
  return &treasureTypes[letter - 'A'];
}
