#ifndef FNF_DUNGEON_LEVEL_MAP_H_INCLUDED
#define FNF_DUNGEON_LEVEL_MAP_H_INCLUDED


#include <stdbool.h>
#include <dungeon/box.h>


struct dungeon;
struct text_rectangle;
struct tile;


struct level_map {
    struct dungeon *dungeon;
    struct box box;
    struct tile **tiles;
};


struct level_map *
level_map_alloc(struct dungeon *dungeon, int level);

void
level_map_free(struct level_map *level_map);

struct tile *
level_map_tile_at(struct level_map const *level_map, struct point point);

struct text_rectangle *
level_map_alloc_text_rectangle(struct level_map *level_map, bool show_scale);

void
level_map_calculate_text_rectangle_dimensions(struct size level_map_size,
                                              bool show_scale,
                                              int *column_count_out,
                                              int *row_count_out);


#endif
