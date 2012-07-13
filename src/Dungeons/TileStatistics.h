#ifndef DUNGEONS_TILESTATISTICS_H_INCLUDED
#define DUNGEONS_TILESTATISTICS_H_INCLUDED


#include <stddef.h>
#include <stdint.h>


struct TileStatistics
{
  size_t count;
  int32_t maxX;
  int32_t maxY;
  int32_t minX;
  int32_t minY;
};


#endif /* #ifndef DUNGEONS_TILESTATISTICS_H_INCLUDED */
