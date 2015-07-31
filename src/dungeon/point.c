#include "point.h"


int
point_compare(struct point point1, struct point point2)
{
  if (point1.z != point2.z) return point1.z - point2.z;
  if (point1.y != point2.y) return point1.y - point2.y;
  return point1.x - point2.x;
}


struct point
point_make(int32_t x, int32_t y, int32_t z)
{
  return (struct point){ .x=x, .y=y, .z=z };
}
