#include "treasure.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/alloc_or_die.h"
#include "common/str.h"

#include "coins.h"
#include "gem.h"
#include "jewelry.h"
#include "treasure_map.h"


char *
treasure_alloc_description(struct treasure *treasure)
{
    enum phrase {
        phrase_coins = 0,
        phrase_gems,
        phrase_jewelry,
        phrase_maps,
        phrase_magic,
        phrases_count
    };
    char *phrases[phrases_count] = {};

    if (!coins_is_zero(treasure->coins)) {
        phrases[phrase_coins] = coins_alloc_description(treasure->coins);
    }
    
    if (treasure->gems_count) {
        char const *plural = (treasure->gems_count == 1) ? "" : "s";
        phrases[phrase_gems] = str_alloc_formatted("%d gem%s",
                                                  treasure->gems_count, plural);
    }
    
    if (treasure->jewelry_count) {
        char const *plural = (treasure->jewelry_count == 1) ? "" : "s";
        phrases[phrase_jewelry] = str_alloc_formatted("%d piece%s of jewelry",
                                                     treasure->jewelry_count,
                                                     plural);
    }
    
    if (treasure->maps_count) {
        char const *plural = (treasure->maps_count == 1) ? "" : "s";
        phrases[phrase_maps] = str_alloc_formatted("%d map%s",
                                                  treasure->maps_count, plural);
    }
    
    if (treasure->magic_items_count) {
        char const *plural = (treasure->magic_items_count == 1) ? "" : "s";
        phrases[phrase_magic] = str_alloc_formatted("%d magic item%s",
                                                   treasure->magic_items_count,
                                                   plural);
    }
    
    char const separator[] = ", ";
    size_t bytes = 0;
    int phrase_count = 0;
    for (int i = 0; i < phrases_count; ++i) {
        if (phrases[i]) {
            if (phrase_count) bytes += strlen(separator);
            bytes += strlen(phrases[i]);
            ++phrase_count;
        }
    }
    bytes += sizeof('\0');
    
    if (!phrase_count) {
        return strdup_or_die("(no treasure)");
    }
    
    char *description = calloc_or_die(bytes, sizeof(char));
    phrase_count = 0;
    for (int i = 0; i < phrases_count; ++i) {
        if (phrases[i]) {
            if (phrase_count) strcat(description, ", ");
            strcat(description, phrases[i]);
            free_or_die(phrases[i]);
            ++phrase_count;
        }
    }
    return description;
}


void
treasure_finalize(struct treasure *treasure)
{
    for (int i = 0; i < treasure->gems_count; ++i) {
        gem_finalize(&treasure->gems[i]);
    }
    free_or_die(treasure->gems);
    
    for (int i = 0; i < treasure->jewelry_count; ++i) {
        jewelry_finalize(&treasure->jewelry[i]);
    }
    free_or_die(treasure->jewelry);
    
    for (int i = 0; i < treasure->maps_count; ++i) {
        treasure_map_finalize(&treasure->maps[i]);
    }
    free_or_die(treasure->maps);
    
    for (int i = 0; i < treasure->magic_items_count; ++i) {
        magic_item_finalize(&treasure->magic_items[i]);
    }
    free_or_die(treasure->magic_items);
}


void
treasure_generate_magic_items(struct treasure *treasure,
                              struct rnd *rnd,
                              int count,
                              possible_magic_items_t possible_magic_items)
{
    treasure->magic_items = reallocarray_or_die(treasure->magic_items,
                                                treasure->magic_items_count + count,
                                                sizeof(struct magic_item));
    for (int i = 0; i < count; ++i) {
        int j = treasure->magic_items_count + i;
        magic_item_initialize(&treasure->magic_items[j]);
        magic_item_generate(&treasure->magic_items[j], rnd, possible_magic_items);
    }
    treasure->magic_items_count += count;
}


void
treasure_generate_maps(struct treasure *treasure, struct rnd *rnd, int count)
{
    treasure->maps = reallocarray_or_die(treasure->maps,
                                         treasure->maps_count + count,
                                         sizeof(struct treasure_map));
    for (int i = 0; i < count; ++i) {
        int j = treasure->maps_count + i;
        treasure_map_initialize(&treasure->maps[j]);
        treasure_map_generate(&treasure->maps[j], rnd);
    }
    treasure->maps_count += count;
}


void
treasure_initialize(struct treasure *treasure)
{
    memset(treasure, 0, sizeof(struct treasure));
}


int
treasure_value_in_cp(struct treasure *treasure)
{
    int value_in_cp = coins_to_cp(treasure->coins);
    
    for (int i = 0; i < treasure->gems_count; ++i) {
        value_in_cp += gem_value_in_cp(&treasure->gems[i]);
    }
    
    for (int i = 0; i < treasure->jewelry_count; ++i) {
        value_in_cp += jewelry_value_in_cp(&treasure->jewelry[i]);
    }
    
    return value_in_cp;
}
