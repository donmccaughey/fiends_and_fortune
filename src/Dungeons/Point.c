#include "Point.h"


int comparePoints(struct Point point1, struct Point point2)
{
  if (point1.z != point2.z) return point1.z - point2.z;
  if (point1.y != point2.y) return point1.y - point2.y;
  return point1.x - point2.x;
}


struct Point makePoint(int32_t x, int32_t y, int32_t z)
{
  return (struct Point) { .x = x, .y = y, .z = z, };
}
