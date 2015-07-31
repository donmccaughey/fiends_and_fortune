#ifndef FNF_DUNGEON_POINT_H_INCLUDED
#define FNF_DUNGEON_POINT_H_INCLUDED


#include <stdint.h>


struct point {
  int32_t x;
  int32_t y;
  int32_t z;
};


int
point_compare(struct point point1, struct point point2);

struct point
point_make(int32_t x, int32_t y, int32_t z);


#endif
