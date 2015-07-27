#include "Treasure.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/alloc_or_die.h"

#include "coins.h"
#include "gem.h"
#include "jewelry.h"
#include "TreasureMap.h"


char *describeTreasure(struct Treasure *treasure) {
    enum Phrase {
        CoinsPhrase = 0,
        GemsPhrase,
        JewelryPhrase,
        MapsPhrase,
        MagicPhrase,
        PhraseCount
    };
    char *phrases[PhraseCount] = {};
    size_t phrasesCount = sizeof(phrases) / sizeof(char *);
    
    if (!coins_is_zero(treasure->coins)) {
        phrases[CoinsPhrase] = coins_alloc_description(treasure->coins);
    }
    
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


void finalizeTreasure(struct Treasure *treasure) {
    for (int i = 0; i < treasure->gemsCount; ++i) {
        gem_finalize(&treasure->gems[i]);
    }
    free_or_die(treasure->gems);
    
    for (int i = 0; i < treasure->jewelryCount; ++i) {
        finalize_jewelry(&treasure->jewelry[i]);
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
        value_cp += gem_value_in_cp(&treasure->gems[i]);
    }
    
    for (int i = 0; i < treasure->jewelryCount; ++i) {
        value_cp += jewelry_value_in_cp(&treasure->jewelry[i]);
    }
    
    return value_cp;
}
