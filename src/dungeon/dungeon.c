#include "dungeon.h"

#include <assert.h>
#include <string.h>

#include "common/alloc_or_die.h"
#include "common/dice.h"
#include "common/fail.h"
#include "common/str.h"

#include "area.h"
#include "dungeon_generator.h"
#include "level_map.h"
#include "text_rectangle.h"
#include "tiles.h"


void
dungeon_add_area(struct dungeon *dungeon, struct area *area)
{
    int index = dungeon->areas_count;
    ++dungeon->areas_count;
    dungeon->areas = reallocarray_or_die(dungeon->areas,
                                         dungeon->areas_count,
                                         sizeof(struct area *));
    dungeon->areas[index] = area;
}


struct dungeon *
dungeon_alloc(void)
{
    struct dungeon *dungeon = calloc_or_die(1, sizeof(struct dungeon));
    dungeon->areas = calloc_or_die(1, sizeof(struct area *));
    dungeon->tiles = tiles_alloc();
    return dungeon;
}


void
dungeon_free(struct dungeon *dungeon)
{
    if (dungeon) {
        for (int i = 0; i < dungeon->areas_count; ++i) {
            area_free(dungeon->areas[i]);
        }
        free_or_die(dungeon->areas);
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


void
dungeon_print_level(struct dungeon *dungeon, int32_t level, FILE *out)
{
    struct level_map *level_map = level_map_alloc(dungeon, level);
    struct text_rectangle *text_rectangle = level_map_alloc_text_graph(level_map);
    fprintf(out, "%s", text_rectangle->chars);
    text_rectangle_free(text_rectangle);
    level_map_free(level_map);
}
