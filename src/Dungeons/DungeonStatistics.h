#ifndef DUNGEONS_DUNGEONSTATISTICS_H_INCLUDED
#define DUNGEONS_DUNGEONSTATISTICS_H_INCLUDED


#include <stddef.h>


struct DungeonStatistics {
  int minX;
  int minY;
  int maxX;
  int maxY;
  size_t tileCount;
};


#endif /* #ifndef DUNGEONS_DUNGEONSTATISTICS_H_INCLUDED */
