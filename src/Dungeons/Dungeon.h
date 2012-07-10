#ifndef DUNGEON_H_INCLUDED
#define DUNGEON_H_INCLUDED


#include <stddef.h>
#include <stdio.h>
#include "Boolean.h"


struct Dice;
struct Tile;


typedef int (*CompareFunction)(void const *, void const *);

/* return TRUE to stop graph traversal */
typedef Boolean (*VisitFunction)(struct Tile *tile, void *context);


enum Direction {
  North, South, East, West
};

enum TileType {
  SolidTileType = 0,
  EmptyTileType
};


struct Tile {
  enum TileType type;
  int x;
  int y;
  int z;
  struct Tile *north;
  struct Tile *south;
  struct Tile *east;
  struct Tile *west;
};

struct Tiles
{
  struct Tile **tiles;
  size_t capacity;
  size_t count;
  CompareFunction compare;
};

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

void addTileToTileIndex(struct Tiles *index, struct Tile *tile);

int compareTilesByCoordinate(void const *item1, void const *item2);

void destroyTileGraph(struct Tile *tile);

void finalizeDungeon(struct Dungeon *dungeon);

void finalizeTiles(struct Tiles *tiles);

struct Tile *findTileInDungeonAt(struct Dungeon *dungeon, int x, int y, int z);

void gatherDungeonStatistics(struct Dungeon *dungeon, struct DungeonStatistics *statistics);

void generateDungeon(struct Dungeon *dungeon, struct Dice *dice);

void graphDungeonUsingText(struct Dungeon *dungeon, FILE *out);

void initializeDungeon(struct Dungeon *dungeon);

void initializeTiles(struct Tiles *tiles);

void initializeTileIndex(struct Tiles *index, CompareFunction compare);

struct Tile *popTile(struct Tiles *stack);

void pushTile(struct Tiles *stack, struct Tile *tile);

Boolean removeTileFromTileIndex(struct Tiles *index, struct Tile *tile);

Boolean visitTiles(struct Tile *tile, void *context, VisitFunction visit);


#endif /* #ifndef DUNGEON_H_INCLUDED */
