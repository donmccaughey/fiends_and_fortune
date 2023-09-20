#include "treasure.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <base/base.h>
#include <json/json.h>

#include "coins.h"
#include "gem.h"
#include "jewelry.h"
#include "treasure_map.h"
#include "treasure_struct.h"
#include "treasure_type.h"


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
            free(phrases[i]);
            ++phrase_count;
        }
    }
    return description;
}


struct ptr_array *
treasure_alloc_details(struct treasure *treasure)
{
    struct ptr_array *lines = ptr_array_alloc();

    char *treasure_type_name = treasure_type_alloc_name(treasure->type);
    ptr_array_add(lines, str_alloc_formatted("Treasure type %s", treasure_type_name));
    free(treasure_type_name);

    char *description = treasure_alloc_description(treasure);
    int value_cp = treasure_value_in_cp(treasure);
    char *value_gp = coins_alloc_gp_cp_description(value_cp);
    ptr_array_add(lines, str_alloc_formatted("    %s (total %s)", description, value_gp));
    free(value_gp);
    free(description);

    if (treasure->gems_count) {
        ptr_array_add(lines, strdup_or_die("Gems: --------------------------------"));
        for (int i = 0; i < treasure->gems_count; ++i) {
            ptr_array_add(lines, str_alloc_formatted("    %2i  %s", i + 1,
                                                     treasure->gems[i].visible_description));
            ptr_array_add(lines, str_alloc_formatted("          %s",
                                                     treasure->gems[i].true_description));
        }
    }

    if (treasure->jewelry_count) {
        ptr_array_add(lines, strdup_or_die("Jewelry: -----------------------------"));
        for (int i = 0; i < treasure->jewelry_count; ++i) {
            ptr_array_add(lines, str_alloc_formatted("    %2i  %s", i + 1, treasure->jewelry[i].true_description));
        }
    }

    if (treasure->maps_count) {
        ptr_array_add(lines, strdup_or_die("Maps: --------------------------------"));
        for (int i = 0; i < treasure->maps_count; ++i) {
            ptr_array_add(lines, str_alloc_formatted("    %2i  %s", i + 1, treasure->maps[i].true_description));
        }
    }

    if (treasure->magic_items_count) {
        ptr_array_add(lines, strdup_or_die("Magic Items: -------------------------"));
        for (int i = 0; i < treasure->magic_items_count; ++i) {
            ptr_array_add(lines, str_alloc_formatted("    %2i  %s", i + 1, treasure->magic_items[i].true_description));
            if (treasure->magic_items[i].true_details) {
                int j = 0;
                while (treasure->magic_items[i].true_details[j]) {
                    ptr_array_add(lines, str_alloc_formatted("            %s", treasure->magic_items[i].true_details[j]));
                    ++j;
                }
            }
        }
    }

    return lines;
}


struct cJSON *
treasure_create_json_object(struct treasure *treasure)
{
    struct cJSON *json_object = cJSON_CreateObject();
    cJSON_AddStringToObject(json_object, "struct", "treasure");
    cJSON_AddNumberToObject(json_object, "rev", 0);

    if (treasure->type) {
        char *type = treasure_type_alloc_name(treasure->type);
        cJSON_AddStringToObject(json_object, "type", type);
        free(type);
    } else {
        cJSON_AddNullToObject(json_object, "type");
    }

    struct cJSON *coins = coins_create_json_object(&treasure->coins);
    cJSON_AddItemToObject(json_object, "coins", coins);

    struct cJSON *gems = cJSON_AddArrayToObject(json_object, "gems");
    for (int i = 0; i < treasure->gems_count; ++i) {
        struct cJSON *gem = gem_create_json_object(&treasure->gems[i]);
        cJSON_AddItemToArray(gems, gem);
    }

    struct cJSON *jewelry = cJSON_AddArrayToObject(json_object, "jewelry");
    for (int i = 0; i < treasure->jewelry_count; ++i) {
        struct cJSON *jewelry_item = jewelry_create_json_object(&treasure->jewelry[i]);
        cJSON_AddItemToArray(jewelry, jewelry_item);
    }

    struct cJSON *maps = cJSON_AddArrayToObject(json_object, "maps");
    for (int i = 0; i < treasure->maps_count; ++i) {
        struct cJSON *map = treasure_map_create_json_object(&treasure->maps[i]);
        cJSON_AddItemToArray(maps, map);
    }

    struct cJSON *magic_items = cJSON_AddArrayToObject(json_object, "magic_items");
    for (int i = 0; i < treasure->magic_items_count; ++i) {
        struct cJSON *magic_item = magic_item_create_json_object(&treasure->magic_items[i]);
        cJSON_AddItemToArray(magic_items, magic_item);
    }

    return json_object;
}


void
treasure_finalize(struct treasure *treasure)
{
    for (int i = 0; i < treasure->gems_count; ++i) {
        gem_finalize(&treasure->gems[i]);
    }
    free(treasure->gems);

    for (int i = 0; i < treasure->jewelry_count; ++i) {
        jewelry_finalize(&treasure->jewelry[i]);
    }
    free(treasure->jewelry);

    for (int i = 0; i < treasure->maps_count; ++i) {
        treasure_map_finalize(&treasure->maps[i]);
    }
    free(treasure->maps);

    for (int i = 0; i < treasure->magic_items_count; ++i) {
        magic_item_finalize(&treasure->magic_items[i]);
    }
    free(treasure->magic_items);
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


void
treasure_initialize_from_json_object(struct treasure *treasure,
                                     struct cJSON *json_object)
{
    treasure_initialize(treasure);

    if ( ! cJSON_IsObject(json_object)) return;
    if ( ! json_object_has_struct_member(json_object, "treasure")) return;

    char const *type = json_object_get_string_value(json_object, "type", NULL);
    if (type && type[0] >= 'A' && type[0] <= 'Z') {
        treasure->type = treasure_type_by_letter(type[0]);
    }

    struct cJSON *coins = cJSON_GetObjectItemCaseSensitive(json_object, "coins");
    treasure->coins = coins_make_from_json_object(coins);

    struct cJSON *gems = cJSON_GetObjectItemCaseSensitive(json_object, "gems");
    if (cJSON_IsArray(gems)) {
        treasure->gems_count = cJSON_GetArraySize(gems);
        if (treasure->gems_count) {
            treasure->gems = calloc_or_die(treasure->gems_count,
                                           sizeof(struct gem));
            for (int i = 0; i < treasure->gems_count; ++i) {
                struct cJSON *gem = cJSON_GetArrayItem(gems, i);
                gem_initialize_from_json_object(&treasure->gems[i], gem);
            }
        }
    }

    struct cJSON *jewelry = cJSON_GetObjectItemCaseSensitive(json_object, "jewelry");
    if (cJSON_IsArray(jewelry)) {
        treasure->jewelry_count = cJSON_GetArraySize(jewelry);
        if (treasure->jewelry_count) {
            treasure->jewelry = calloc_or_die(treasure->jewelry_count,
                                              sizeof(struct jewelry));
            for (int i = 0; i < treasure->jewelry_count; ++i) {
                struct cJSON *jewelry_item = cJSON_GetArrayItem(jewelry, i);
                jewelry_initialize_from_json_object(&treasure->jewelry[i], jewelry_item);
            }
        }
    }

    struct cJSON *maps = cJSON_GetObjectItemCaseSensitive(json_object, "maps");
    if (cJSON_IsArray(maps)) {
        treasure->maps_count = cJSON_GetArraySize(maps);
        if (treasure->maps_count) {
            treasure->maps = calloc_or_die(treasure->maps_count,
                                           sizeof(struct treasure_map));
            for (int i = 0; i < treasure->maps_count; ++i) {
                struct cJSON *map = cJSON_GetArrayItem(maps, i);
                treasure_map_initialize_from_json_object(&treasure->maps[i], map);
            }
        }
    }

    struct cJSON *magic_items = cJSON_GetObjectItemCaseSensitive(json_object, "magic_items");
    if (cJSON_IsArray(magic_items)) {
        treasure->magic_items_count = cJSON_GetArraySize(magic_items);
        if (treasure->magic_items_count) {
            treasure->magic_items = calloc_or_die(treasure->magic_items_count,
                                                  sizeof(struct magic_item));
            for (int i = 0; i < treasure->magic_items_count; ++i) {
                struct cJSON *magic_item = cJSON_GetArrayItem(magic_items, i);
                magic_item_initialize_from_json_object(&treasure->magic_items[i], magic_item);
            }
        }
    }
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
