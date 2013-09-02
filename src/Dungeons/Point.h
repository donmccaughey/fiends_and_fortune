#ifndef DUNGEONS_POINT_H_INCLUDED
#define DUNGEONS_POINT_H_INCLUDED


#include <stdint.h>


struct Point {
  int32_t x;
  int32_t y;
  int32_t z;
};


int comparePoints(struct Point point1, struct Point point2);

struct Point makePoint(int32_t x, int32_t y, int32_t z);


#endif /* #ifndef DUNGEONS_POINT_H_INCLUDED */
