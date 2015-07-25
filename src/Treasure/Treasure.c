#include "Treasure.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc_or_die.h"
#include "coins.h"
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
    
    // TODO: extract coins_alloc_description() from this
    describeTreasureCoins(&treasure->coins.cp,
                          "copper", &phrases[CopperPhrase]);
    describeTreasureCoins(&treasure->coins.sp,
                          "silver", &phrases[SilverPhrase]);
    describeTreasureCoins(&treasure->coins.ep,
                          "electrum", &phrases[ElectrumPhrase]);
    describeTreasureCoins(&treasure->coins.gp, "gold", &phrases[GoldPhrase]);
    describeTreasureCoins(&treasure->coins.pp,
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
            free_or_die(phrases[i]);
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
    free_or_die(treasure->gems);
    
    for (int i = 0; i < treasure->jewelryCount; ++i) {
        finalizeJewelry(&treasure->jewelry[i]);
    }
    free_or_die(treasure->jewelry);
    
    for (int i = 0; i < treasure->mapsCount; ++i) {
        finalizeTreasureMap(&treasure->maps[i]);
    }
    free_or_die(treasure->maps);
    
    for (int i = 0; i < treasure->magicItemsCount; ++i) {
        finalizeMagicItem(&treasure->magicItems[i]);
    }
    free_or_die(treasure->magicItems);
}


void generateMagicItemsForTreasure(struct Treasure *treasure,
                                   struct rnd *rnd,
                                   int count,
                                   PossibleMagicItems possibleMagicItems)
{
    treasure->magicItems = reallocarray_or_die(treasure->magicItems,
                                               treasure->magicItemsCount + count,
                                               sizeof(struct MagicItem));
    for (int i = 0; i < count; ++i) {
        int j = treasure->magicItemsCount + i;
        initializeMagicItem(&treasure->magicItems[j]);
        generateMagicItem(&treasure->magicItems[j], rnd, possibleMagicItems);
    }
    treasure->magicItemsCount += count;
}


void generateMapsForTreasure(struct Treasure *treasure,
                             struct rnd *rnd,
                             int count)
{
    treasure->maps = reallocarray_or_die(treasure->maps,
                                         treasure->mapsCount + count,
                                         sizeof(struct TreasureMap));
    for (int i = 0; i < count; ++i) {
        int j = treasure->mapsCount + i;
        initializeTreasureMap(&treasure->maps[j]);
        generateTreasureMap(&treasure->maps[j], rnd);
    }
    treasure->mapsCount += count;
}


void initializeTreasure(struct Treasure *treasure) {
    memset(treasure, 0, sizeof(struct Treasure));
}


int treasureValue_cp(struct Treasure *treasure) {
    int value_cp = coins_to_cp(treasure->coins);
    
    for (int i = 0; i < treasure->gemsCount; ++i) {
        value_cp += gemValue_cp(&treasure->gems[i]);
    }
    
    for (int i = 0; i < treasure->jewelryCount; ++i) {
        value_cp += jewelryValue_cp(&treasure->jewelry[i]);
    }
    
    return value_cp;
}
