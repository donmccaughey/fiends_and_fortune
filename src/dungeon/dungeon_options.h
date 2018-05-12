#ifndef FNF_DUNGEON_DUNGEON_OPTIONS_H_INCLUDED
#define FNF_DUNGEON_DUNGEON_OPTIONS_H_INCLUDED


#include "size.h"


struct dungeon_options {
    int max_iteration_count;
    struct size max_size;
    int padding;
};


struct dungeon_options *
dungeon_options_alloc(int max_iteration_count,
                      struct size max_size,
                      int padding);

struct dungeon_options *
dungeon_options_alloc_default(void);

void
dungeon_options_free(struct dungeon_options *dungeon_options);


#endif
