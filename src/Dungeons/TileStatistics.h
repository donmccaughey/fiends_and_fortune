#ifndef DUNGEONS_TILESTATISTICS_H_INCLUDED
#define DUNGEONS_TILESTATISTICS_H_INCLUDED


#include <stddef.h>
#include <stdint.h>
#include "Range.h"


struct TileStatistics {
  size_t count;
  struct Range xRange;
  struct Range yRange;
};


#endif /* #ifndef DUNGEONS_TILESTATISTICS_H_INCLUDED */
