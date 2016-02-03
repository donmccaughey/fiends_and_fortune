#include "dungeon.h"

#include <assert.h>
#include <string.h>

#include "common/alloc_or_die.h"
#include "common/dice.h"
#include "common/fail.h"
#include "common/str.h"

#include "area.h"
#include "areas.h"
#include "dungeon_generator.h"
#include "tiles.h"


struct dungeon *
dungeon_alloc(void)
{
    struct dungeon *dungeon = calloc_or_die(1, sizeof(struct dungeon));
    dungeon->areas = areas_alloc();
    dungeon->tiles = tiles_alloc();
    return dungeon;
}


char const **
dungeon_alloc_area_descriptions(struct dungeon *dungeon)
{
    size_t descriptions_count = areas_count(dungeon->areas);
    char const **descriptions = calloc_or_die(descriptions_count + 1,
                                              sizeof(char const *));
    
    for (size_t i = 0; i < descriptions_count; ++i) {
        descriptions[i] = areas_area_at_index(dungeon->areas, i)->description;
    }
    return descriptions;
}


void
dungeon_free(struct dungeon *dungeon)
{
    if (dungeon) {
        areas_free(dungeon->areas);
        tiles_free(dungeon->tiles);
        free_or_die(dungeon);
    }
}


void
dungeon_generate(struct dungeon *dungeon, struct rnd *rnd)
{
    struct dungeon_generator *generator = dungeon_generator_alloc(dungeon, rnd);
    dungeon_generator_generate(generator);
    dungeon_generator_free(generator);
}


void
dungeon_generate_small(struct dungeon *dungeon)
{
    struct dungeon_generator *generator = dungeon_generator_alloc(dungeon, NULL);
    dungeon_generator_generate_small(generator);
    dungeon_generator_free(generator);
}
