#ifndef DUNGEONS_TILESTATISTICS_H_INCLUDED
#define DUNGEONS_TILESTATISTICS_H_INCLUDED


#include <stddef.h>


struct TileStatistics
{
  size_t count;
  int maxX;
  int maxY;
  int minX;
  int minY;
};


#endif /* #ifndef DUNGEONS_TILESTATISTICS_H_INCLUDED */
