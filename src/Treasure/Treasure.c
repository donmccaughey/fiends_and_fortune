#include "Treasure.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc_or_die.h"
#include "coinage.h"
#include "Dice.h"
#include "Gem.h"
#include "Jewelry.h"
#include "TreasureMap.h"


static void describeTreasureCoins(int *coins, char const *name, char **phrase);


char *describeTreasure(struct Treasure *treasure) {
  enum Phrase {
    CopperPhrase = 0,
    SilverPhrase,
    ElectrumPhrase,
    GoldPhrase,
    PlatinumPhrase,
    GemsPhrase,
    JewelryPhrase,
    MapsPhrase,
    MagicPhrase,
    PhraseCount
  };
  char *phrases[PhraseCount] = {};
  size_t phrasesCount = sizeof(phrases) / sizeof(char *);
  
  describeTreasureCoins(&treasure->copper, "copper", &phrases[CopperPhrase]);
  describeTreasureCoins(&treasure->silver, "silver", &phrases[SilverPhrase]);
  describeTreasureCoins(&treasure->electrum, 
                        "electrum", &phrases[ElectrumPhrase]);
  describeTreasureCoins(&treasure->gold, "gold", &phrases[GoldPhrase]);
  describeTreasureCoins(&treasure->platinum, 
                        "platinum", &phrases[PlatinumPhrase]);
  
  if (treasure->gemsCount) {
    char const *plural = (treasure->gemsCount == 1) ? "" : "s";
    asprintf_or_die(&phrases[GemsPhrase], "%d gem%s",
                    treasure->gemsCount, plural);
  }
  
  if (treasure->jewelryCount) {
    char const *plural = (treasure->jewelryCount == 1) ? "" : "s";
    asprintf_or_die(&phrases[JewelryPhrase], "%d piece%s of jewelry",
                    treasure->jewelryCount, plural);
  }
  
  if (treasure->mapsCount) {
    char const *plural = (treasure->mapsCount == 1) ? "" : "s";
    asprintf_or_die(&phrases[MapsPhrase], "%d map%s",
                    treasure->mapsCount, plural);
  }
  
  if (treasure->magicItemsCount) {
    char const *plural = (treasure->magicItemsCount == 1) ? "" : "s";
    asprintf_or_die(&phrases[MagicPhrase], "%d magic item%s",
                    treasure->magicItemsCount, plural);
  }
  
  char const separator[] = ", ";
  size_t bytes = 0; 
  int phraseCount = 0;
  for (int i = 0; i < phrasesCount; ++i) {
    if (phrases[i]) {
      if (phraseCount) {
        bytes += strlen(separator);
      }
      bytes += strlen(phrases[i]);
      ++phraseCount;
    }
  }
  bytes += sizeof('\0');
  
  if ( ! phraseCount) {
    return strdup_or_die("(no treasure)");
  }
  
  char *description = calloc_or_die(bytes, sizeof(char));
  phraseCount = 0;
  for (int i = 0; i < phrasesCount; ++i) {
    if (phrases[i]) {
      if (phraseCount) {
        strcat(description, ", ");
      }
      strcat(description, phrases[i]);
      free(phrases[i]);
      ++phraseCount;
    }
  }
  return description;
}


static void describeTreasureCoins(int *coins, char const *name, char **phrase) {
  if (*coins) {
    asprintf_or_die(phrase, "%d %s", *coins, name);
  }
}


void finalizeTreasure(struct Treasure *treasure) {
  for (int i = 0; i < treasure->gemsCount; ++i) {
    finalizeGem(&treasure->gems[i]);
  }
  free(treasure->gems);
  
  for (int i = 0; i < treasure->jewelryCount; ++i) {
    finalizeJewelry(&treasure->jewelry[i]);
  }
  free(treasure->jewelry);
  
  for (int i = 0; i < treasure->mapsCount; ++i) {
    finalizeTreasureMap(&treasure->maps[i]);
  }
  free(treasure->maps);
  
  for (int i = 0; i < treasure->magicItemsCount; ++i) {
    finalizeMagicItem(&treasure->magicItems[i]);
  }
  free(treasure->magicItems);
}


void generateMagicItemsForTreasure(struct Treasure *treasure, 
                                   struct Dice *dice, 
                                   int count, 
                                   PossibleMagicItems possibleMagicItems)
{
  size_t newSize = (treasure->magicItemsCount + count)  
                 * sizeof(struct MagicItem);
  treasure->magicItems = realloc_or_die(treasure->magicItems, newSize);
  for (int i = 0; i < count; ++i) {
    int j = treasure->magicItemsCount + i;
    initializeMagicItem(&treasure->magicItems[j]);
    generateMagicItem(&treasure->magicItems[j], dice, possibleMagicItems);
  }
  treasure->magicItemsCount += count;
}


void generateMapsForTreasure(struct Treasure *treasure, 
                             struct Dice *dice, 
                             int count)
{
  size_t newSize = (treasure->mapsCount + count) * sizeof(struct TreasureMap);
  treasure->maps = realloc_or_die(treasure->maps, newSize);
  for (int i = 0; i < count; ++i) {
    int j = treasure->mapsCount + i;
    initializeTreasureMap(&treasure->maps[j]);
    generateTreasureMap(&treasure->maps[j], dice);
  }
  treasure->mapsCount += count;
}


void initializeTreasure(struct Treasure *treasure) {
  memset(treasure, 0, sizeof(struct Treasure));
}


int treasureValue_cp(struct Treasure *treasure) {
  int value_cp = 0;
  value_cp += treasure->copper;
  value_cp += treasure->silver * CP_PER_SP;
  value_cp += treasure->electrum * CP_PER_EP;
  value_cp += treasure->gold * CP_PER_GP;
  value_cp += treasure->platinum * CP_PER_PP;
  
  for (int i = 0; i < treasure->gemsCount; ++i) {
    value_cp += gemValue_cp(&treasure->gems[i]);
  }
  
  for (int i = 0; i < treasure->jewelryCount; ++i) {
    value_cp += jewelryValue_cp(&treasure->jewelry[i]);
  }
  
  return value_cp;
}
