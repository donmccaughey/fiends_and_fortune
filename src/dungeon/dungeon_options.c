#include "dungeon_options.h"

#include "common/alloc_or_die.h"


struct dungeon_options *
dungeon_options_alloc(int max_iteration_count,
                      struct size max_size,
                      int padding)
{
    struct dungeon_options *dungeon_options = calloc_or_die(1, sizeof(struct dungeon_options));
    dungeon_options->max_iteration_count = max_iteration_count;
    dungeon_options->max_size = max_size;
    dungeon_options->padding = padding;
    return dungeon_options;
}


struct dungeon_options *
dungeon_options_alloc_default(void)
{
    return dungeon_options_alloc(100, size_make(30, 20, 5), 1);
}


void
dungeon_options_free(struct dungeon_options *dungeon_options)
{
    free_or_die(dungeon_options);
}
