#include <assert.h>
#include <base/base.h>
#include <dungeon/dungeon.h>


void
size_test(void);


static void
size_make_test(void)
{
    struct size size = size_make(1, 2, 3);
    assert(1 == size.width);
    assert(2 == size.length);
    assert(3 == size.height);
}


static void
size_make_empty_test(void)
{
    struct size size = size_make_empty();
    assert(0 == size.width);
    assert(0 == size.length);
    assert(0 == size.height);
}


static void
size_make_unit_test(void)
{
    struct size size = size_make_unit();
    assert(1 == size.width);
    assert(1 == size.length);
    assert(1 == size.height);
}


static void
size_make_from_points_test(void)
{
    struct point point1 = point_make(1, 1, 1);
    struct point point2 = point_make(2, 2, 2);

    struct size size = size_make_from_points(point1, point2);
    assert(1 == size.width);
    assert(1 == size.length);
    assert(1 == size.height);

    struct point point3 = point_make(1, 2, 3);
    size = size_make_from_points(point1, point3);
    assert(0 == size.width);
    assert(1 == size.length);
    assert(2 == size.height);

    struct point point4 = point_make(3, 2, 1);
    size = size_make_from_points(point4, point3);
    assert(-2 == size.width);
    assert(0 == size.length);
    assert(2 == size.height);
}


static void
size_compare_test(void)
{
    struct size unit1 = size_make(1, 1, 1);
    struct size unit2 = size_make(1, 1, 1);

    assert(0 == size_compare(unit1, unit2));

    // order first by height

    struct size tall = size_make(1, 1, 5);
    assert(size_compare(tall, unit1) > 0);
    assert(size_compare(unit1, tall) < 0);

    // order second by length

    struct size lengthy = size_make(1, 5, 1);
    assert(size_compare(lengthy, unit1) > 0);
    assert(size_compare(unit1, lengthy) < 0);

    assert(size_compare(tall, lengthy) > 0);
    assert(size_compare(lengthy, tall) < 0);

    // order third by width

    struct size wide = size_make(5, 1, 1);
    assert(size_compare(wide, unit1) > 0);
    assert(size_compare(unit1, wide) < 0);

    assert(size_compare(lengthy, wide) > 0);
    assert(size_compare(wide, lengthy) < 0);

    assert(size_compare(tall, wide) > 0);
    assert(size_compare(wide, tall) < 0);
}


static void
size_equals_test(void)
{
    struct size unit1 = size_make(1, 1, 1);
    struct size unit2 = size_make(1, 1, 1);

    assert(size_equals(unit1, unit2));
    assert(size_equals(unit2, unit1));

    struct size tall = size_make(1, 1, 5);
    assert( ! size_equals(unit1, tall));
    assert( ! size_equals(tall, unit1));

    struct size lengthy = size_make(1, 5, 1);
    assert( ! size_equals(unit1, lengthy));
    assert( ! size_equals(lengthy, unit1));

    struct size wide = size_make(5, 1, 1);
    assert( ! size_equals(unit1, wide));
    assert( ! size_equals(wide, unit1));
}


static void
size_is_empty_test(void)
{
    struct size unit = size_make(1, 1, 1);
    assert( ! size_is_empty(unit));

    struct size no_height = size_make(1, 1, 0);
    assert( ! size_is_empty(no_height));

    struct size no_length_height = size_make(1, 0, 0);
    assert( ! size_is_empty(no_length_height));

    struct size no_size = size_make(0, 0, 0);
    assert(size_is_empty(no_size));

    struct size negative_width = size_make(-1, 0, 0);
    assert( ! size_is_empty(negative_width));
}


static void
size_has_volume_test(void)
{
    struct size unit = size_make(1, 1, 1);
    assert(size_has_volume(unit));

    struct size no_height = size_make(1, 1, 0);
    assert( ! size_has_volume(no_height));

    struct size no_length_height = size_make(1, 0, 0);
    assert( ! size_has_volume(no_length_height));

    struct size no_size = size_make(0, 0, 0);
    assert( ! size_has_volume(no_size));

    struct size negative_width = size_make(-1, 0, 0);
    assert( ! size_has_volume(negative_width));
}


void
size_test(void)
{
    size_make_test();
    size_make_empty_test();
    size_make_unit_test();
    size_make_from_points_test();
    size_compare_test();
    size_equals_test();
    size_is_empty_test();
    size_has_volume_test();
}
