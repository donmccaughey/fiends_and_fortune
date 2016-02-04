#ifndef FNF_DUNGEON_TILES_H_INCLUDED
#define FNF_DUNGEON_TILES_H_INCLUDED


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "point.h"
#include "range.h"


struct tile;


struct tiles {
    int capacity;
    int (*compare)(void const *, void const *);
    int count;
    struct tiles *parent;
    struct tile **members;
    struct range x_range;
    struct range y_range;
    struct range z_range;
};


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

bool
tiles_has_tile_in_range(struct tiles *tiles,
                        struct range x_range,
                        struct range y_range,
                        struct range z_range);

#endif
