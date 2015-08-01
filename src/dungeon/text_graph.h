#ifndef FNF_DUNGEON_TEXT_GRAPH_H_INCLUDED
#define FNF_DUNGEON_TEXT_GRAPH_H_INCLUDED


#include <stdint.h>
#include <stdio.h>


struct dungeon;


void
dungeon_graph_level_using_text(struct dungeon *dungeon,
                               int32_t level,
                               FILE *out);


#endif
