#include <assert.h>
#include <base/base.h>
#include <dungeon/dungeon.h>


void
point_test(void);


static void
point_make_test(void)
{
    struct point point = point_make(1, 2, 3);
    
    assert(1 == point.x);
    assert(2 == point.y);
    assert(3 == point.z);
}


static void
point_compare_test(void)
{
    struct point point1 = point_make(1, 2, 3);
    struct point point2 = point_make(1, 2, 3);
    assert(0 == point_compare(point1, point2));
    assert(0 == point_compare(point2, point1));

    struct point more_x = point_make(2, 2, 3);
    assert(point_compare(point1, more_x) < 0);
    assert(point_compare(more_x, point1) > 0);

    struct point more_y = point_make(1, 4, 3);
    assert(point_compare(point1, more_y) < 0);
    assert(point_compare(more_y, point1) > 0);

    struct point more_z = point_make(1, 2, 6);
    assert(point_compare(point1, more_z) < 0);
    assert(point_compare(more_z, point1) > 0);
}


static void
point_equals_test(void)
{
    struct point point1 = point_make(1, 2, 3);
    struct point point2 = point_make(1, 2, 3);
    assert(point_equals(point1, point2));
    assert(point_equals(point2, point1));

    struct point more_x = point_make(2, 2, 3);
    assert( ! point_equals(point1, more_x));
    assert( ! point_equals(more_x, point1));

    struct point less_y = point_make(1, 1, 3);
    assert( ! point_equals(point1, more_x));
    assert( ! point_equals(more_x, point1));

    struct point more_z = point_make(1, 2, 4);
    assert( ! point_equals(point1, more_x));
    assert( ! point_equals(more_x, point1));
}


static void
point_move_test(void)
{
    struct point start = point_make(1, 1, 1);
    struct point end;

    end = point_move(start, 3, direction_north);
    assert(1 == end.x);
    assert(4 == end.y);
    assert(1 == end.z);

    end = point_move(start, 3, direction_northeast);
    assert(4 == end.x);
    assert(4 == end.y);
    assert(1 == end.z);

    end = point_move(start, 3, direction_east);
    assert(4 == end.x);
    assert(1 == end.y);
    assert(1 == end.z);

    end = point_move(start, 3, direction_southeast);
    assert(4 == end.x);
    assert(-2 == end.y);
    assert(1 == end.z);

    end = point_move(start, 3, direction_south);
    assert(1 == end.x);
    assert(-2 == end.y);
    assert(1 == end.z);

    end = point_move(start, 3, direction_southwest);
    assert(-2 == end.x);
    assert(-2 == end.y);
    assert(1 == end.z);

    end = point_move(start, 3, direction_west);
    assert(-2 == end.x);
    assert(1 == end.y);
    assert(1 == end.z);

    end = point_move(start, 3, direction_northwest);
    assert(-2 == end.x);
    assert(4 == end.y);
    assert(1 == end.z);
}


static void
point_above_test(void)
{
    struct point point = point_make(1, 2, 3);
    struct point above = point_above(point);

    assert(1 == above.x);
    assert(2 == above.y);
    assert(2 == above.z);
}


static void
point_below_test(void)
{
    struct point point = point_make(1, 2, 3);
    struct point below = point_below(point);

    assert(1 == below.x);
    assert(2 == below.y);
    assert(4 == below.z);
}


static void
point_north_test(void)
{
    struct point point = point_make(1, 2, 3);
    struct point north = point_north(point);

    assert(1 == north.x);
    assert(3 == north.y);
    assert(3 == north.z);
}


static void
point_south_test(void)
{
    struct point point = point_make(1, 2, 3);
    struct point south = point_south(point);

    assert(1 == south.x);
    assert(1 == south.y);
    assert(3 == south.z);
}


static void
point_east_test(void)
{
    struct point point = point_make(1, 2, 3);
    struct point east = point_east(point);

    assert(2 == east.x);
    assert(2 == east.y);
    assert(3 == east.z);
}


static void
point_west_test(void)
{
    struct point point = point_make(1, 2, 3);
    struct point west = point_west(point);

    assert(0 == west.x);
    assert(2 == west.y);
    assert(3 == west.z);
}


static void
point_northeast_test(void)
{
    struct point point = point_make(1, 2, 3);
    struct point northeast = point_northeast(point);

    assert(2 == northeast.x);
    assert(3 == northeast.y);
    assert(3 == northeast.z);
}


static void
point_northwest_test(void)
{
    struct point point = point_make(1, 2, 3);
    struct point northwest = point_northwest(point);

    assert(0 == northwest.x);
    assert(3 == northwest.y);
    assert(3 == northwest.z);
}


static void
point_southeast_test(void)
{
    struct point point = point_make(1, 2, 3);
    struct point southeast = point_southeast(point);

    assert(2 == southeast.x);
    assert(1 == southeast.y);
    assert(3 == southeast.z);
}


static void
point_southwest_test(void)
{
    struct point point = point_make(1, 2, 3);
    struct point southwest = point_southwest(point);

    assert(0 == southwest.x);
    assert(1 == southwest.y);
    assert(3 == southwest.z);
}


static void
point_rotate_90_degrees_left_cardinal_directions_test(void)
{
    struct point start = point_make(2, 2, 2);
    struct point end;

    // 3  .  .  .
    // 2  .  a  .
    // 1  b  *  .
    //    1  2  3
    end = point_rotate_90_degrees_left(start, direction_north);
    assert(1 == end.x);
    assert(1 == end.y);
    assert(2 == end.z);

    // 3  b  .  .
    // 2  *  a  .
    // 1  .  .  .
    //    1  2  3
    end = point_rotate_90_degrees_left(start, direction_east);
    assert(1 == end.x);
    assert(3 == end.y);
    assert(2 == end.z);

    // 3  .  *  b
    // 2  .  a  .
    // 1  .  .  .
    //    1  2  3
    end = point_rotate_90_degrees_left(start, direction_south);
    assert(3 == end.x);
    assert(3 == end.y);
    assert(2 == end.z);

    // 3  .  .  .
    // 2  .  a  *
    // 1  .  .  b
    //    1  2  3
    end = point_rotate_90_degrees_left(start, direction_west);
    assert(3 == end.x);
    assert(1 == end.y);
    assert(2 == end.z);
}


static void
point_rotate_90_degrees_left_intercardinal_directions_test(void)
{
    struct point start = point_make(2, 2, 2);
    struct point end;

    // 4  .  .  .  .  .
    // 3  .  .  .  .  .
    // 2  b  .  a  .  .
    // 1  .  *  .  .  .
    // 0  .  .  .  .  .
    //    0  1  2  3  4
    end = point_rotate_90_degrees_left(start, direction_northeast);
    assert(0 == end.x);
    assert(2 == end.y);
    assert(2 == end.z);

    // 4  .  .  b  .  .
    // 3  .  *  .  .  .
    // 2  .  .  a  .  .
    // 1  .  .  .  .  .
    // 0  .  .  .  .  .
    //    0  1  2  3  4
    end = point_rotate_90_degrees_left(start, direction_southeast);
    assert(2 == end.x);
    assert(4 == end.y);
    assert(2 == end.z);

    // 4  .  .  .  .  .
    // 3  .  .  .  *  .
    // 2  .  .  a  .  b
    // 1  .  .  .  .  .
    // 0  .  .  .  .  .
    //    0  1  2  3  4
    end = point_rotate_90_degrees_left(start, direction_southwest);
    assert(4 == end.x);
    assert(2 == end.y);
    assert(2 == end.z);

    // 4  .  .  .  .  .
    // 3  .  .  .  .  .
    // 2  .  .  a  .  .
    // 1  .  .  .  *  .
    // 0  .  .  b  .  .
    //    0  1  2  3  4
    end = point_rotate_90_degrees_left(start, direction_northwest);
    assert(2 == end.x);
    assert(0 == end.y);
    assert(2 == end.z);
}


static void
point_rotate_90_degrees_right_cardinal_directions_test(void)
{
    struct point start = point_make(2, 2, 2);
    struct point end;

    // 3  .  .  .
    // 2  .  a  .
    // 1  .  *  b
    //    1  2  3
    end = point_rotate_90_degrees_right(start, direction_north);
    assert(3 == end.x);
    assert(1 == end.y);
    assert(2 == end.z);

    // 3  .  .  .
    // 2  *  a  .
    // 1  b  .  .
    //    1  2  3
    end = point_rotate_90_degrees_right(start, direction_east);
    assert(1 == end.x);
    assert(1 == end.y);
    assert(2 == end.z);

    // 3  b  *  .
    // 2  .  a  .
    // 1  .  .  .
    //    1  2  3
    end = point_rotate_90_degrees_right(start, direction_south);
    assert(1 == end.x);
    assert(3 == end.y);
    assert(2 == end.z);

    // 3  .  .  b
    // 2  .  a  *
    // 1  .  .  .
    //    1  2  3
    end = point_rotate_90_degrees_right(start, direction_west);
    assert(3 == end.x);
    assert(3 == end.y);
    assert(2 == end.z);
}


static void
point_rotate_90_degrees_right_intercardinal_directions_test(void)
{
    struct point start = point_make(2, 2, 2);
    struct point end;

    // 4  .  .  .  .  .
    // 3  .  .  .  .  .
    // 2  .  .  a  .  .
    // 1  .  *  .  .  .
    // 0  .  .  b  .  .
    //    0  1  2  3  4
    end = point_rotate_90_degrees_right(start, direction_northeast);
    assert(2 == end.x);
    assert(0 == end.y);
    assert(2 == end.z);

    // 4  .  .  .  .  .
    // 3  .  *  .  .  .
    // 2  b  .  a  .  .
    // 1  .  .  .  .  .
    // 0  .  .  .  .  .
    //    0  1  2  3  4
    end = point_rotate_90_degrees_right(start, direction_southeast);
    assert(0 == end.x);
    assert(2 == end.y);
    assert(2 == end.z);

    // 4  .  .  b  .  .
    // 3  .  .  .  *  .
    // 2  .  .  a  .  .
    // 1  .  .  .  .  .
    // 0  .  .  .  .  .
    //    0  1  2  3  4
    end = point_rotate_90_degrees_right(start, direction_southwest);
    assert(2 == end.x);
    assert(4 == end.y);
    assert(2 == end.z);

    // 4  .  .  .  .  .
    // 3  .  .  .  .  .
    // 2  .  .  a  .  b
    // 1  .  .  .  *  .
    // 0  .  .  .  .  .
    //    0  1  2  3  4
    end = point_rotate_90_degrees_right(start, direction_northwest);
    assert(4 == end.x);
    assert(2 == end.y);
    assert(2 == end.z);
}


static void
point_alloc_xy_test(void)
{
    char *point = point_alloc_xy(point_make(1, 2, 3));
    assert(str_eq("(1, 2)", point));
    free_or_die(point);
}


void point_test(void)
{
    point_make_test();
    point_compare_test();
    point_equals_test();
    point_move_test();
    point_above_test();
    point_below_test();
    point_north_test();
    point_south_test();
    point_east_test();
    point_west_test();
    point_northeast_test();
    point_northwest_test();
    point_southeast_test();
    point_southwest_test();
    point_rotate_90_degrees_left_cardinal_directions_test();
    point_rotate_90_degrees_left_intercardinal_directions_test();
    point_rotate_90_degrees_right_cardinal_directions_test();
    point_rotate_90_degrees_right_intercardinal_directions_test();
    point_alloc_xy_test();
}
