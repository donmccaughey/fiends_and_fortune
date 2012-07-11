#include "TreasureType.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Dice.h"
#include "Gem.h"
#include "heap.h"
#include "Jewelry.h"
#include "MagicItem.h"
#include "Treasure.h"
#include "TreasureMap.h"
#include "unexpected.h"


struct CoinsGemsOrJewelry {
  Boolean isPossible;
  Boolean isPerIndividual;
  int percentChance;
  char const *amount;
  int multiplier;
};


struct MapsOrMagicType {
  int amount;
  char const *variableAmount;
  Boolean isMapPossible;
  PossibleMagicItems possibleMagicItems;
};


struct MapsOrMagic {
  Boolean isPossible;
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
    { TRUE, FALSE, 25, "1D6", 1000 },
    { TRUE, FALSE, 30, "1D6", 1000 },
    { TRUE, FALSE, 35, "1D6", 1000 },
    { TRUE, FALSE, 40, "1D10", 1000 },
    { TRUE, FALSE, 25, "1D4", 100 },
    { TRUE, FALSE, 60, "4D10", 1 },
    { TRUE, FALSE, 50, "3D10", 1 },
    {
      TRUE, 30, 
      {
        { 3, "", TRUE, ANY_MAGIC_ITEM } 
      }, 
      1 
    }
  },
  {
    'B',
    { TRUE, FALSE, 50, "1D8", 1000 },
    { TRUE, FALSE, 25, "1D6", 1000 },
    { TRUE, FALSE, 25, "1D4", 1000 },
    { TRUE, FALSE, 25, "1D3", 1000 },
    { FALSE, FALSE, 0, "", 0 },
    { TRUE, FALSE, 30, "1D8", 1 },
    { TRUE, FALSE, 20, "1D4", 1 },
    {
      TRUE, 10, 
      {
        { 1, "", FALSE, MAGIC_WEAPON_OR_ARMOR } 
      }, 
      1 
    }
  },
  {
    'C',
    { TRUE, FALSE, 20, "1D12", 1000 },
    { TRUE, FALSE, 30, "1D6", 1000 },
    { TRUE, FALSE, 10, "1D4", 1000 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { TRUE, FALSE, 25, "1D6", 1 },
    { TRUE, FALSE, 20, "1D3", 1 },
    {
      TRUE, 10, 
      {
        { 2, "", TRUE, ANY_MAGIC_ITEM } 
      }, 
      1 
    }
  },
  {
    'D',
    { TRUE, FALSE, 10, "1D8", 1000 },
    { TRUE, FALSE, 15, "1D12", 1000 },
    { TRUE, FALSE, 15, "1D8", 1000 },
    { TRUE, FALSE, 50, "1D6", 1000 },
    { FALSE, FALSE, 0, "", 0 },
    { TRUE, FALSE, 30, "1D10", 1 },
    { TRUE, FALSE, 25, "1D6", 1 },
    {
      TRUE, 15, 
      {
        { 2, "", TRUE, ANY_MAGIC_ITEM },
        { 1, "", FALSE, POTION } 
      }, 
      2
    }
  },
  {
    'E',
    { TRUE, FALSE, 5, "1D10", 1000 },
    { TRUE, FALSE, 25, "1D12", 1000 },
    { TRUE, FALSE, 25, "1D6", 1000 },
    { TRUE, FALSE, 25, "1D8", 1000 },
    { FALSE, FALSE, 0, "", 0 },
    { TRUE, FALSE, 15, "1D12", 1 },
    { TRUE, FALSE, 10, "1D8", 1 },
    {
      TRUE, 25, 
      {
        { 3, "", TRUE, ANY_MAGIC_ITEM },
        { 1, "", FALSE, SCROLL } 
      }, 
      2
    }
  },
  {
    'F',
    { FALSE, FALSE, 0, "", 0 },
    { TRUE, FALSE, 10, "1D20", 1000 },
    { TRUE, FALSE, 15, "1D12", 1000 },
    { TRUE, FALSE, 40, "1D10", 1000 },
    { TRUE, FALSE, 35, "1D8", 100 },
    { TRUE, FALSE, 20, "3D10", 1 },
    { TRUE, FALSE, 10, "1D10", 1 },
    {
      TRUE, 30, 
      {
        { 3, "", TRUE, NON_WEAPON_MAGIC },
        { 1, "", FALSE, POTION },
        { 1, "", FALSE, SCROLL } 
      }, 
      3
    }
  },
  {
    'G',
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { TRUE, FALSE, 50, "10D4", 1000 },
    { TRUE, FALSE, 50, "1D20", 100 },
    { TRUE, FALSE, 30, "5D4", 1 },
    { TRUE, FALSE, 25, "1D10", 1 },
    {
      TRUE, 35, 
      {
        { 4, "", TRUE, ANY_MAGIC_ITEM },
        { 1, "", FALSE, SCROLL } 
      }, 
      2
    }
  },
  {
    'H',
    { TRUE, FALSE, 25, "5D6", 1000 },
    { TRUE, FALSE, 40, "1D100", 1000 },
    { TRUE, FALSE, 40, "10D4", 1000 },
    { TRUE, FALSE, 55, "10D6", 1000 },
    { TRUE, FALSE, 25, "5D10", 100 },
    { TRUE, FALSE, 50, "1D100", 1 },
    { TRUE, FALSE, 50, "10D4", 1 },
    {
      TRUE, 15, 
      {
        { 4, "", TRUE, ANY_MAGIC_ITEM },
        { 1, "", FALSE, POTION },
        { 1, "", FALSE, SCROLL } 
      }, 
      3
    }
  },
  {
    'I',
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { TRUE, FALSE, 30, "3D6", 100 },
    { TRUE, FALSE, 55, "2D10", 1 },
    { TRUE, FALSE, 50, "1D12", 1 },
    {
      TRUE, 15, 
      {
        { 1, "", TRUE, ANY_MAGIC_ITEM } 
      }, 
      1 
    }
  },
  {
    'J',
    { TRUE, TRUE, 100, "3D8", 1 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, 0, {}, 0 }
  },
  {
    'K',
    { FALSE, FALSE, 0, "", 0 },
    { TRUE, TRUE, 100, "3D6", 1 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, 0, {}, 0 }
  },
  {
    'L',
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { TRUE, TRUE, 100, "2D6", 1 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, 0, {}, 0 }
  },
  {
    'M',
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { TRUE, TRUE, 100, "2D4", 1 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, 0, {}, 0 }
  },
  {
    'N',
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { TRUE, TRUE, 100, "1D6", 1 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, 0, {}, 0 }
  },
  {
    'O',
    { TRUE, FALSE, 25, "1D4", 1000 },
    { TRUE, FALSE, 20, "1D3", 1000 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, 0, {}, 0 }
  },
  {
    'P',
    { FALSE, FALSE, 0, "", 0 },
    { TRUE, FALSE, 30, "1D6", 1000 },
    { TRUE, FALSE, 25, "1D2", 1000 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, 0, {}, 0 }
  },
  {
    'Q',
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { TRUE, FALSE, 50, "1D4", 1 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, 0, {}, 0 }
  },
  {
    'R',
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { TRUE, FALSE, 40, "2D4", 1000 },
    { TRUE, FALSE, 50, "10D6", 100 },
    { TRUE, FALSE, 55, "4D8", 1 },
    { TRUE, FALSE, 45, "1D12", 1 },
    { FALSE, 0, {}, 0 }
  },
  {
    'S',
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    {
      TRUE, 40, 
      {
        { 0, "2D4", FALSE, POTION } 
      }, 
      1 
    }
  },
  {
    'T',
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    {
      TRUE, 50, 
      {
        { 0, "1D4", FALSE, SCROLL } 
      }, 
      1 
    }
  },
  {
    'U',
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { TRUE, FALSE, 90, "10D8", 1 },
    { TRUE, FALSE, 80, "5D6", 1 },
    {
      TRUE, 70, 
      {
        { 1, "", FALSE, RING },
        { 1, "", FALSE, ROD_STAFF_WAND },
        { 1, "", FALSE, MISC_MAGIC },
        { 1, "", FALSE, ARMOR_SHIELD },
        { 1, "", FALSE, SWORD },
        { 1, "", FALSE, MISC_WEAPON }
      }, 
      6
    }
  },
  {
    'V',
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    {
      TRUE, 85, 
      {
        { 2, "", FALSE, RING },
        { 2, "", FALSE, ROD_STAFF_WAND },
        { 2, "", FALSE, MISC_MAGIC },
        { 2, "", FALSE, ARMOR_SHIELD },
        { 2, "", FALSE, SWORD },
        { 2, "", FALSE, MISC_WEAPON }
      }, 
      6
    }
  },
  {
    'W',
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { TRUE, FALSE, 60, "5D6", 1000 },
    { TRUE, FALSE, 15, "1D8", 100 },
    { TRUE, FALSE, 60, "10D8", 1 },
    { TRUE, FALSE, 50, "5D8", 1 },
    {
      TRUE, 55, 
      {
        { 1, "", TRUE, NO_MAGIC_ITEM }
      }, 
      1
    }
  },
  {
    'X',
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    {
      TRUE, 60, 
      {
        { 1, "", FALSE, MISC_MAGIC },
        { 1, "", FALSE, POTION }
      }, 
      2
    }
  },
  {
    'Y',
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { TRUE, FALSE, 70, "2D6", 1000 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, FALSE, 0, "", 0 },
    { FALSE, 0, {}, 0 }
  },
  {
    'Z',
    { TRUE, FALSE, 20, "1D3", 1000 },
    { TRUE, FALSE, 25, "1D4", 1000 },
    { TRUE, FALSE, 25, "1D4", 1000 },
    { TRUE, FALSE, 30, "1D4", 1000 },
    { TRUE, FALSE, 30, "1D6", 100 },
    { TRUE, FALSE, 55, "10D6", 1 },
    { TRUE, FALSE, 50, "5D6", 1 },
    {
      TRUE, 50, 
      {
        { 3, "", FALSE, ANY_MAGIC_ITEM }
      }, 
      1
    }
  }
};


static char *describeCoinsGemsOrJewelry(struct CoinsGemsOrJewelry *coinsGemsOrJewelry);

static char *describeMapsOrMagic(struct MapsOrMagic *mapsOrMagic);

static void generateTreasureCoins(int *coins, 
                                  struct Dice *dice, 
                                  struct CoinsGemsOrJewelry *coinsType, 
                                  int individualCount);

static void generateTreasureGems(struct Treasure *treasure, struct Dice *dice);

static void generateTreasureJewelry(struct Treasure *treasure, 
                                    struct Dice *dice);

static void generateTreasureMapsOrMagic(struct Treasure *treasure, 
                                        struct Dice *dice);

static void generateTreasureMapsOrMagicType(struct MapsOrMagicType *type, 
                                            struct Treasure *treasure, 
                                            struct Dice *dice);

static char const *possibleMapsOrMagicName(Boolean isMapPossible, 
                                           PossibleMagicItems possibleMagicItems);


char *describeTreasureType(struct TreasureType *treasureType, 
                           Boolean includeHeader)
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
  
  ASPRINTF_OR_DIE(&letter, letterFormat, treasureType->letter);
  
  char *descriptionFormat = "%s%s | %s | %s | %s | %s | %s | %s | %s | %s\n";
  char *description;
  ASPRINTF_OR_DIE(&description, descriptionFormat, 
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
      int minAmount = minDieRoll(coinsGemsOrJewelry->amount);
      int minLength = (minAmount < 10 ? 1 : 2);
      
      int maxAmount = maxDieRoll(coinsGemsOrJewelry->amount);
      int maxLength = (maxAmount < 10 ? 1 : 2);
      
      int dieRollLength = minLength + 1 /* dash */ + maxLength;
      char const *suffix = (dieRollLength < 4 ? " " : "");
      
      char *description;
      ASPRINTF_OR_DIE(&description, " %i-%i per%s", 
                      minAmount, maxAmount, suffix);
      
      return description;
    } else {
      int minAmount = minDieRoll(coinsGemsOrJewelry->amount);
      int minLength = (minAmount < 10 ? 1 : 2);
      
      int maxAmount = maxDieRoll(coinsGemsOrJewelry->amount);
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
      ASPRINTF_OR_DIE(&dieRoll, "%i-%i", minAmount, maxAmount);
      
      char *description;
      ASPRINTF_OR_DIE(&description, "%s%s:%2i%%%s", prefix, dieRoll, 
                      coinsGemsOrJewelry->percentChance, suffix);
      free(dieRoll);
      return description;
    }
  } else {
    return STRDUP_OR_DIE("   nil   ");
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
        int minAmount = minDieRoll(type->variableAmount);
        int maxAmount = maxDieRoll(type->variableAmount);
        ASPRINTF_OR_DIE(&typeDescriptions[i], "%i-%i %ss", 
                        minAmount, maxAmount, typeName);
      } else if (   type->isMapPossible 
                 && type->possibleMagicItems == ANY_MAGIC_ITEM)
      {
        ASPRINTF_OR_DIE(&typeDescriptions[i], "any %i", type->amount);
      } else if (type->possibleMagicItems == NON_WEAPON_MAGIC) {
        ASPRINTF_OR_DIE(&typeDescriptions[i], 
                        "any %i except sword or misc weapon", type->amount);
      } else if (type->possibleMagicItems == ANY_MAGIC_ITEM) {
        ASPRINTF_OR_DIE(&typeDescriptions[i], "any %i magic", type->amount);
      } else {
        char const *plural = (type->amount == 1) ? "" : "s";
        ASPRINTF_OR_DIE(&typeDescriptions[i], "%i %s%s", 
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
    char *typeList = CALLOC_OR_DIE(typeListSize, sizeof(char));
    for (int i = 0; i < mapsOrMagic->typeCount; ++i) {
      if (i) {
        strcat(typeList, separator);
      }
      strcat(typeList, typeDescriptions[i]);
      free(typeDescriptions[i]);
    }
    
    char *description;
    ASPRINTF_OR_DIE(&description, "%s: %i%%", 
                    typeList, mapsOrMagic->percentChance);
    free(typeList);
    return description;
  } else {
    return STRDUP_OR_DIE("   nil   ");
  }
}


void generateTreasure(struct TreasureType *treasureType, 
                      struct Treasure *treasure, 
                      struct Dice *dice,
                      int individualCount)
{
  treasure->type = treasureType;
  generateTreasureCoins(&treasure->copper, dice, 
                        &treasureType->copper, individualCount);
  generateTreasureCoins(&treasure->silver, dice, 
                        &treasureType->silver, individualCount);
  generateTreasureCoins(&treasure->electrum, dice, 
                        &treasureType->electrum, individualCount);
  generateTreasureCoins(&treasure->gold, dice, 
                        &treasureType->gold, individualCount);
  generateTreasureCoins(&treasure->platinum, dice, 
                        &treasureType->platinum, individualCount);
  generateTreasureGems(treasure, dice);
  generateTreasureJewelry(treasure, dice);
  generateTreasureMapsOrMagic(treasure, dice);
}


static void generateTreasureCoins(int *coins, 
                                  struct Dice *dice, 
                                  struct CoinsGemsOrJewelry *coinsType, 
                                  int individualCount)
{
  *coins = 0;
  if (coinsType->isPossible) {
    if (coinsType->isPerIndividual) {
      assert(individualCount > 0);
      for (int i = 0; i < individualCount; ++i) {
        *coins += roll(dice, coinsType->amount);
      }
    } else {
      int percentRoll = roll(dice, "1D100");
      if (percentRoll <= coinsType->percentChance) {
        *coins = roll(dice, coinsType->amount) * coinsType->multiplier;
      }
    }
  }
}


static void generateTreasureGems(struct Treasure *treasure, 
                                 struct Dice *dice)
{
  treasure->gems = NULL;
  treasure->gemsCount = 0;
  if (treasure->type->gems.isPossible) {
    int percentRoll = roll(dice, "1D100");
    if (percentRoll <= treasure->type->gems.percentChance) {
      treasure->gemsCount = roll(dice, treasure->type->gems.amount);
      treasure->gems = CALLOC_OR_DIE(treasure->gemsCount, sizeof(struct Gem));
      for (int i = 0; i < treasure->gemsCount; ++i) {
        initializeGem(&treasure->gems[i]);
        generateGem(&treasure->gems[i], dice);
      }
    }
  }
}


static void generateTreasureJewelry(struct Treasure *treasure, 
                                    struct Dice *dice)
{
  treasure->jewelry = NULL;
  treasure->jewelryCount = 0;
  if (treasure->type->jewelry.isPossible) {
    int percentRoll = roll(dice, "1D100");
    if (percentRoll <= treasure->type->jewelry.percentChance) {
      treasure->jewelryCount = roll(dice, treasure->type->jewelry.amount);
      treasure->jewelry = CALLOC_OR_DIE(treasure->jewelryCount, 
                                        sizeof(struct Jewelry));
      for (int i = 0; i < treasure->jewelryCount; ++i) {
        initializeJewelry(&treasure->jewelry[i]);
        generateJewelry(&treasure->jewelry[i], dice);
      }
    }
  }
}


static void generateTreasureMapsOrMagic(struct Treasure *treasure, 
                                        struct Dice *dice)
{
  if (treasure->type->mapsOrMagic.isPossible) {
    int percentRoll = roll(dice, "1d100");
    if (percentRoll <= treasure->type->mapsOrMagic.percentChance) {
      for (int i = 0; i < treasure->type->mapsOrMagic.typeCount; ++i) {
        struct MapsOrMagicType *type = &treasure->type->mapsOrMagic.types[i];
        generateTreasureMapsOrMagicType(type, treasure, dice);
      }
    }
  }
}


static void generateTreasureMapsOrMagicType(struct MapsOrMagicType *type, 
                                            struct Treasure *treasure, 
                                            struct Dice *dice)
{
  int amount = type->amount;
  Boolean isVariable = type->variableAmount[0];
  if (isVariable) {
    amount = roll(dice, type->variableAmount);
  }
  
  int magicItemsCount = 0;
  int mapsCount = 0;
  for (int i = 0; i < amount; ++i) {
    if (type->isMapPossible) {
      Boolean isMagicItemPossible = type->possibleMagicItems & ANY_MAGIC_ITEM;
      if (isMagicItemPossible) {
        (roll(dice, "1d100") <= 10) ? ++mapsCount : ++magicItemsCount;
      } else {
        ++mapsCount;
      }
    } else {
      ++magicItemsCount;
    }
  }
  
  if (mapsCount) {
    generateMapsForTreasure(treasure, dice, mapsCount);
  }
  if (magicItemsCount) {
    generateMagicItemsForTreasure(treasure, dice, magicItemsCount, 
                                  type->possibleMagicItems);
  }
}


static char const *possibleMapsOrMagicName(Boolean isMapPossible, 
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
