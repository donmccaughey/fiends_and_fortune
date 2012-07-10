#ifndef DUNGEONS_DUNGEON_H_INCLUDED
#define DUNGEONS_DUNGEON_H_INCLUDED


#include <stddef.h>
#include <stdio.h>
#include "Boolean.h"
#include "Tiles.h"


struct Dice;
struct Tile;


struct Dungeon {
  struct Tile *mainEntrance;
  struct Tiles index;
};

struct DungeonStatistics {
  int minX;
  int minY;
  int maxX;
  int maxY;
  size_t tileCount;
};

void addTileToDungeon(struct Dungeon *dungeon, struct Tile *tile);

int compareTilesByCoordinate(void const *item1, void const *item2);

void destroyTileGraph(struct Tile *tile);

void finalizeDungeon(struct Dungeon *dungeon);

struct Tile *findTileInDungeonAt(struct Dungeon *dungeon, int x, int y, int z);

void gatherDungeonStatistics(struct Dungeon *dungeon, struct DungeonStatistics *statistics);

void generateDungeon(struct Dungeon *dungeon, struct Dice *dice);

void graphDungeonUsingText(struct Dungeon *dungeon, FILE *out);

void initializeDungeon(struct Dungeon *dungeon);


#endif /* #ifndef DUNGEONS_DUNGEON_H_INCLUDED */
