#ifndef DUNGEONS_POINT_H_INCLUDED
#define DUNGEONS_POINT_H_INCLUDED


#include <stdint.h>


struct Point {
  int32_t x;
  int32_t y;
  int32_t z;
};


int comparePoints(struct Point const *point1, struct Point const *point2);


#endif /* #ifndef DUNGEONS_POINT_H_INCLUDED */
