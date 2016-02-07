#include "size.h"


struct size
size_make(int width, int length, int height)
{
    return (struct size){ .height=height, .length=length, .width=width };
}


struct size
size_make_from_points(struct point begin, struct point end)
{
    return size_make(end.x - begin.x, end.y - begin.y, end.z - begin.z);
}
