#ifndef FNF_DUNGEON_DUNGEON_H_INCLUDED
#define FNF_DUNGEON_DUNGEON_H_INCLUDED


#include <stdio.h>

#include <dungeon/area.h>
#include <dungeon/area_features.h>
#include <dungeon/area_type.h>
#include <dungeon/box.h>
#include <dungeon/digger.h>
#include <dungeon/dungeon_options.h>
#include <dungeon/exit.h>
#include <dungeon/generator.h>
#include <dungeon/level_map.h>
#include <dungeon/orientation.h>
#include <dungeon/periodic_check.h>
#include <dungeon/point.h>
#include <dungeon/size.h>
#include <dungeon/text_rectangle.h>
#include <dungeon/tile.h>
#include <dungeon/tile_features.h>
#include <dungeon/tile_type.h>
#include <dungeon/wall_type.h>


struct area;
struct dungeon_options;
struct generator;
struct rnd;
struct tile;


typedef void (dungeon_progress_callback)(struct generator *generator, void *user_data);


struct dungeon {
    struct area **areas;
    int areas_count;
    struct tile **tiles;
    int tiles_count;
};


struct dungeon *
dungeon_alloc(void);

void
dungeon_free(struct dungeon *dungeon);

void
dungeon_generate(struct dungeon *dungeon,
                 struct rnd *rnd,
                 struct dungeon_options *dungeon_options,
                 dungeon_progress_callback *progress_callback,
                 void *callback_user_data);

void
dungeon_generate_small(struct dungeon *dungeon);

int
dungeon_level_count(struct dungeon const *dungeon);

int
dungeon_starting_level(struct dungeon const *dungeon);

int
dungeon_ending_level(struct dungeon const *dungeon);

void
dungeon_print_map_for_level(struct dungeon *dungeon, int level, FILE *out);

void
dungeon_print_areas_for_level(struct dungeon *dungeon, int level, FILE *out);

void
dungeon_add_area(struct dungeon *dungeon, struct area *area);

bool
dungeon_is_box_excavated(struct dungeon *dungeon, struct box box);

struct box
dungeon_box_for_level(struct dungeon *dungeon, int level);

struct tile **
dungeon_alloc_tiles_for_box(struct dungeon *dungeon, struct box box);

struct tile *
dungeon_tile_at(struct dungeon *dungeon, struct point point);


#endif
