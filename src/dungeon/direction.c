#include "direction.h"


char const *
direction_name(enum direction direction)
{
  switch (direction) {
    case North: return "North";
    case South: return "South";
    case East: return "East";
    case West: return "West";
    default: return "unknown";
  }
}
