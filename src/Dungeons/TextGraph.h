#ifndef DUNGEONS_TEXTGRAPH_H_INCLUDED
#define DUNGEONS_TEXTGRAPH_H_INCLUDED


#include <stdint.h>
#include <stdio.h>


struct Dungeon;


void graphDungeonLevelUsingText(struct Dungeon *dungeon, int32_t z, FILE *out);


#endif /* #ifndef DUNGEONS_TEXTGRAPH_H_INCLUDED */
