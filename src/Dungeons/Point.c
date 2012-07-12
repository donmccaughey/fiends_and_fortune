#include "Point.h"


int comparePoints(struct Point const *point1, struct Point const *point2)
{
  if (point1->z != point2->z) {
    return point1->z - point2->z;
  }
  if (point1->y != point2->y) {
    return point1->y - point2->y;
  }
  return point1->x - point2->x;
}
