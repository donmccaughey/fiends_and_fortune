#include "dungeon.h"

#include <assert.h>
#include <string.h>

#include "common/alloc_or_die.h"
#include "common/dice.h"
#include "common/fail.h"
#include "common/str.h"

#include "area.h"
#include "areas.h"
#include "generator.h"
#include "tiles.h"


char const **
dungeon_alloc_area_descriptions(struct dungeon *dungeon)
{
    size_t descriptionsCount = areas_count(dungeon->areas);
    char const **descriptions = calloc_or_die(descriptionsCount + 1,
                                              sizeof(char const *));
    
    for (size_t i = 0; i < descriptionsCount; ++i) {
        descriptions[i] = areas_area_at_index(dungeon->areas, i)->description;
    }
    return descriptions;
}


void
dungeon_finalize(struct dungeon *dungeon)
{
    areas_free(dungeon->areas);
    tiles_free(dungeon->tiles);
}


void
dungeon_generate(struct dungeon *dungeon, struct rnd *rnd)
{
    struct generator *generator = generator_alloc(dungeon, rnd);
    generator_generate(generator);
    generator_free(generator);
}


void
dungeon_generate_small(struct dungeon *dungeon)
{
    struct generator *generator = generator_alloc(dungeon, NULL);
    generator_generate_small(generator);
    generator_free(generator);
}


void
dungeon_initialize(struct dungeon *dungeon)
{
    memset(dungeon, 0, sizeof(struct dungeon));
    dungeon->areas = areas_alloc();
    dungeon->tiles = tiles_alloc();
}
