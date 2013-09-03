#include "Direction.h"


char const *directionName(enum Direction direction)
{
  switch (direction) {
    case North: return "North";
    case South: return "South";
    case East: return "East";
    case West: return "West";
    default: return "Unknown";
  }
}
