#include "size.h"


int
size_compare(struct size size, struct size other)
{
    if (size.height != other.height) return size.height - other.height;
    if (size.length != other.length) return size.length - other.length;
    return size.width - other.width;
}


bool
size_equals(struct size size, struct size other)
{
    return 0 == size_compare(size, other);
}


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
