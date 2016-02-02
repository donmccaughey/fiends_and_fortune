#include "direction.h"


enum direction
direction_90_degrees_left(enum direction direction)
{
    switch (direction) {
        case North: return West;
        case South: return East;
        case East: return North;
        case West: return South;
        default: return Unknown;
    }
}


enum direction
direction_90_degrees_right(enum direction direction)
{
    switch (direction) {
        case North: return East;
        case South: return West;
        case East: return South;
        case West: return North;
        default: return Unknown;
    }
}


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
