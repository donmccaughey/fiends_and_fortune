#ifndef FNF_DUNGEON_SIZE_H_INCLUDED
#define FNF_DUNGEON_SIZE_H_INCLUDED


#include "point.h"


struct size {
    int width;  // x-axis
    int length; // y-axis
    int height; // z-axis
};


struct size
size_make(int width, int length, int height);

struct size
size_make_from_points(struct point begin, struct point end);


#endif
