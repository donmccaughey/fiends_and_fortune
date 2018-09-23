#include "action.h"

#include <string.h>
#include <base/base.h>


static struct {
    enum action action;
    char const *string;
} action_strings[] = {
    { action_character, "character" },
    { action_check, "check" },
    { action_dungeon, "dungeon" },
    { action_each, "each" },
    { action_magic, "magic" },
    { action_map, "map" },
    { action_table, "table" },
};
static int action_strings_count = ARRAY_COUNT(action_strings);


enum action
action_from_string(char const *string)
{
    if (1 == strlen(string)) {
        if (   (string[0] >= 'A' && string[0] <= 'Z')
            || (string[0] >= 'a' && string[0] <= 'z'))
        {
            return action_treasure;
        }
    }
    for (int i = 0; i < action_strings_count; ++i) {
        if (0 == strcasecmp(string, action_strings[i].string)) {
            return action_strings[i].action;
        }
    }
    return action_invalid;
}
