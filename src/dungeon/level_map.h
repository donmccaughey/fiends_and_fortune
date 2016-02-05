#ifndef FNF_DUNGEON_LEVEL_MAP_H_INCLUDED
#define FNF_DUNGEON_LEVEL_MAP_H_INCLUDED


#include "range.h"


struct dungeon;
struct tile;


struct level_map {
    struct dungeon *dungeon;
    struct tile **grid;
    uint32_t level;
    struct range x_range;
    struct range y_range;
};

struct lines_array {
    char **lines;
    int lines_count;
    int line_length;
};


struct level_map *
level_map_alloc(struct dungeon *dungeon, uint32_t level);

void
level_map_free(struct level_map *level_map);

struct tile *
level_map_tile_at(struct level_map *level_map, uint32_t x, uint32_t y);

struct lines_array *
level_map_alloc_text_graph(struct level_map *level_map);

struct lines_array *
lines_array_alloc(int line_length);

void
lines_array_free(struct lines_array *lines_array);

char *
lines_array_add_line(struct lines_array *lines_array);


#endif
