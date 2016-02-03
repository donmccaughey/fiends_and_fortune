#ifndef FNF_DUNGEON_TILES_H_INCLUDED
#define FNF_DUNGEON_TILES_H_INCLUDED


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "point.h"
#include "range.h"


struct tile;
struct tiles;


/// The tile is added to the parent tiles struct as well.
void
tiles_add_tile(struct tiles *tiles, struct tile *tile);

struct tiles *
tiles_alloc_with_parent(struct tiles *parent);

struct tiles *
tiles_alloc(void);

struct tiles *
tiles_alloc_with_parent_and_level(struct tiles *parent, int32_t level);

void
tiles_free(struct tiles *tiles);

struct tile *
tiles_find_tile_at(struct tiles const *tiles, struct point point);

bool
tiles_remove_tile(struct tiles *tiles, struct tile const *tile);

struct tiles *
tiles_root(struct tiles *tiles);

struct tile *
tiles_tile_at_index(struct tiles const *tiles, size_t index);

size_t
tiles_count(struct tiles const *tiles);

struct range
tiles_x_range(struct tiles const *tiles);

struct range
tiles_y_range(struct tiles const *tiles);

struct range
tiles_z_range(struct tiles const *tiles);

bool
tiles_has_tile_in_range(struct tiles *tiles,
                        struct range x_range,
                        struct range y_range,
                        struct range z_range);

#endif
