#ifndef DUNGEONS_DUNGEON_H_INCLUDED
#define DUNGEONS_DUNGEON_H_INCLUDED


#include <stdio.h>
#include "Tiles.h"


struct Dice;
struct TileStatistics;
struct Tile;


struct Dungeon {
  struct Tiles tiles;
};


void finalizeDungeon(struct Dungeon *dungeon);

void generateDungeon(struct Dungeon *dungeon, struct Dice *dice);

void graphDungeonLevelUsingText(struct Dungeon *dungeon, int z, FILE *out);

void initializeDungeon(struct Dungeon *dungeon);


#endif /* #ifndef DUNGEONS_DUNGEON_H_INCLUDED */
