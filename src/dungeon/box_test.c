#include "box.h"

#include <assert.h>


void
box_test(void);


static void
box_make_test(void)
{
    struct box box = box_make(point_make(1, 2, 3), size_make(4, 5, 6));
    assert(1 == box.origin.x);
    assert(2 == box.origin.y);
    assert(3 == box.origin.z);
    assert(4 == box.size.width);
    assert(5 == box.size.length);
    assert(6 == box.size.height);
}


static void
box_make_empty_test(void)
{
    struct box box = box_make_empty(point_make(1, 2, 3));
    assert(1 == box.origin.x);
    assert(2 == box.origin.y);
    assert(3 == box.origin.z);
    assert(0 == box.size.width);
    assert(0 == box.size.length);
    assert(0 == box.size.height);
}


static void
box_make_unit_test(void)
{
    struct box box = box_make_unit(point_make(1, 2, 3));
    assert(1 == box.origin.x);
    assert(2 == box.origin.y);
    assert(3 == box.origin.z);
    assert(1 == box.size.width);
    assert(1 == box.size.length);
    assert(1 == box.size.height);
}


static void
box_make_from_boxes_test(void)
{
    struct box box1 = box_make(point_make(0, 0, 0), size_make(1, 1, 1));
    struct box box2 = box_make(point_make(1, 0, 0), size_make(1, 1, 1));
    struct box box3 = box_make(point_make(3, 4, 5), size_make(1, 1, 1));
    struct box box4 = box_make(point_make(-2, -2, -2), size_make(2, 2, 2));
    
    struct box box = box_make_from_boxes(box1, box2);
    
    assert(0 == box.origin.x);
    assert(0 == box.origin.y);
    assert(0 == box.origin.z);
    assert(2 == box.size.width);
    assert(1 == box.size.length);
    assert(1 == box.size.height);
    
    box = box_make_from_boxes(box1, box3);
    
    assert(0 == box.origin.x);
    assert(0 == box.origin.y);
    assert(0 == box.origin.z);
    assert(4 == box.size.width);
    assert(5 == box.size.length);
    assert(6 == box.size.height);
    
    box = box_make_from_boxes(box3, box4);
    
    assert(-2 == box.origin.x);
    assert(-2 == box.origin.y);
    assert(-2 == box.origin.z);
    assert(6 == box.size.width);
    assert(7 == box.size.length);
    assert(8 == box.size.height);
}


static void
box_make_from_points_test(void)
{
    struct point point1 = point_make(0, 0, 0);
    struct point point2 = point_make(1, 1, 1);
    struct point point3 = point_make(3, 4, 5);
    struct point point4 = point_make(-2, -2, -2);
    
    struct box box = box_make_from_points(point1, point2);
    
    assert(0 == box.origin.x);
    assert(0 == box.origin.y);
    assert(0 == box.origin.z);
    assert(1 == box.size.width);
    assert(1 == box.size.length);
    assert(1 == box.size.height);
    
    box = box_make_from_points(point2, point3);
    
    assert(1 == box.origin.x);
    assert(1 == box.origin.y);
    assert(1 == box.origin.z);
    assert(2 == box.size.width);
    assert(3 == box.size.length);
    assert(4 == box.size.height);
    
    box = box_make_from_points(point4, point3);
    
    assert(-2 == box.origin.x);
    assert(-2 == box.origin.y);
    assert(-2 == box.origin.z);
    assert(5 == box.size.width);
    assert(6 == box.size.length);
    assert(7 == box.size.height);
    
    box = box_make_from_points(point3, point4);
    
    assert(3 == box.origin.x);
    assert(4 == box.origin.y);
    assert(5 == box.origin.z);
    assert(-5 == box.size.width);
    assert(-6 == box.size.length);
    assert(-7 == box.size.height);
}


static void
box_equals_test(void)
{
    struct box box = box_make(point_make(1, 2, 3), size_make(4, 5, 6));
    struct box identical = box_make(point_make(1, 2, 3), size_make(4, 5, 6));
    struct box different_point = box_make(point_make(2, 2, 3), size_make(4, 5, 6));
    struct box different_size = box_make(point_make(1, 2, 3), size_make(4, 5, 7));

    assert(box_equals(box, identical));
    assert(box_equals(identical, box));

    assert( ! box_equals(box, different_point));
    assert( ! box_equals(different_point, box));

    assert( ! box_equals(box, different_size));
    assert( ! box_equals(different_size, box));
}


static void
box_contains_box_test(void)
{
    struct box box = box_make(point_make(1, 1, 1), size_make(4, 4, 4));
    struct box identical = box_make(point_make(1, 1, 1), size_make(4, 4, 4));

    assert(box_contains_box(box, identical));
    assert(box_contains_box(identical, box));

    struct box taller = box_make(point_make(1, 1, 1), size_make(4, 4, 5));
    struct box longer = box_make(point_make(1, 1, 1), size_make(4, 5, 4));
    struct box wider = box_make(point_make(1, 1, 1), size_make(5, 4, 4));

    assert( ! box_contains_box(box, taller));
    assert(box_contains_box(taller, box));

    assert( ! box_contains_box(box, longer));
    assert(box_contains_box(longer, box));

    assert( ! box_contains_box(box, wider));
    assert(box_contains_box(wider, box));

    struct box shorter = box_make(point_make(1, 1, 1), size_make(4, 4, 3));
    struct box less_long = box_make(point_make(1, 1, 1), size_make(4, 3, 4));
    struct box less_wide = box_make(point_make(1, 1, 1), size_make(3, 4, 4));

    assert(box_contains_box(box, shorter));
    assert( ! box_contains_box(shorter, box));

    assert(box_contains_box(box, less_long));
    assert( ! box_contains_box(less_long, box));

    assert(box_contains_box(box, less_wide));
    assert( ! box_contains_box(less_wide, box));

    struct box shifted_x = box_make(point_make(0, 1, 1), size_make(4, 4, 4));
    struct box shifted_y = box_make(point_make(1, 0, 1), size_make(4, 4, 4));
    struct box shifted_z = box_make(point_make(1, 1, 0), size_make(4, 4, 4));

    assert( ! box_contains_box(box, shifted_x));
    assert( ! box_contains_box(shifted_x, box));

    assert( ! box_contains_box(box, shifted_y));
    assert( ! box_contains_box(shifted_y, box));

    assert( ! box_contains_box(box, shifted_z));
    assert( ! box_contains_box(shifted_z, box));
}


static void
box_contains_point_test(void)
{
    struct box box = box_make(point_make(1, 1, 1), size_make(4, 4, 4));

    assert(box_contains_point(box, point_make(1, 1, 1)));

    assert(box_contains_point(box, point_make(2, 1, 1)));
    assert(box_contains_point(box, point_make(1, 2, 1)));
    assert(box_contains_point(box, point_make(1, 1, 2)));

    assert(box_contains_point(box, point_make(4, 1, 1)));
    assert(box_contains_point(box, point_make(1, 4, 1)));
    assert(box_contains_point(box, point_make(1, 1, 4)));

    assert(box_contains_point(box, point_make(4, 4, 1)));
    assert(box_contains_point(box, point_make(4, 1, 4)));
    assert(box_contains_point(box, point_make(1, 4, 4)));

    assert(box_contains_point(box, point_make(3, 4, 4)));
    assert(box_contains_point(box, point_make(4, 3, 4)));
    assert(box_contains_point(box, point_make(4, 4, 3)));

    assert(box_contains_point(box, point_make(4, 4, 4)));

    assert( ! box_contains_point(box, point_make(0, 1, 1)));
    assert( ! box_contains_point(box, point_make(1, 0, 1)));
    assert( ! box_contains_point(box, point_make(1, 1, 0)));

    assert( ! box_contains_point(box, point_make(5, 4, 4)));
    assert( ! box_contains_point(box, point_make(4, 5, 4)));
    assert( ! box_contains_point(box, point_make(4, 4, 5)));
}


static void
box_end_point_test(void)
{
    struct box box = box_make(point_make(1, 1, 1), size_make(2, 3, 4));
    assert(point_equals(point_make(3, 4, 5), box_end_point(box)));
}


static void
box_area_test(void)
{
    struct box empty = box_make_empty(point_make(1, 2, 3));
    struct box unit = box_make_unit(point_make(1, 2, 3));
    struct box box = box_make(point_make(1, 2, 3), size_make(2, 3, 4));

    assert(0 == box_area(empty));
    assert(1 == box_area(unit));
    assert(6 == box_area(box));
}


static void
box_volume_test(void)
{
    struct box empty = box_make_empty(point_make(1, 2, 3));
    struct box unit = box_make_unit(point_make(1, 2, 3));
    struct box box = box_make(point_make(1, 2, 3), size_make(2, 3, 4));

    assert(0 == box_volume(empty));
    assert(1 == box_volume(unit));
    assert(24 == box_volume(box));
}


static void
box_normalize_test(void)
{
    struct box box1 = box_make(point_make(0, 0, 0), size_make(1, 1, 1));
    struct box box2 = box_make(point_make(0, 0, 0), size_make(-1, 1, 1));
    struct box box3 = box_make(point_make(0, 0, 0), size_make(-2, 1, 1));
    struct box box4 = box_make(point_make(-4, -4, -4), size_make(-2, -3, -4));
    
    struct box box = box_normalize(box1);
    assert(box_equals(box1, box));
    
    box = box_normalize(box2);
    assert(point_equals(point_make(0, 0, 0), box.origin));
    assert(size_equals(size_make(1, 1, 1), box.size));
    
    box = box_normalize(box3);
    assert(point_equals(point_make(-1, 0, 0), box.origin));
    assert(size_equals(size_make(2, 1, 1), box.size));
    
    box = box_normalize(box4);
    assert(point_equals(point_make(-5, -6, -7), box.origin));
    assert(size_equals(size_make(2, 3, 4), box.size));
}


static void
box_expand_test(void)
{
    // TODO: add test for box with zero size

    struct box box = box_make(point_make(0, 0, 0), size_make(1, 1, 1));
    box = box_expand(box, size_make(0, 0, 0));
    
    assert(0 == box.origin.x);
    assert(0 == box.origin.y);
    assert(0 == box.origin.z);
    assert(1 == box.size.width);
    assert(1 == box.size.length);
    assert(1 == box.size.height);
    
    box = box_expand(box, size_make(1, 1, 1));
    
    assert(-1 == box.origin.x);
    assert(-1 == box.origin.y);
    assert(-1 == box.origin.z);
    assert(3 == box.size.width);
    assert(3 == box.size.length);
    assert(3 == box.size.height);
    
    box = box_expand(box, size_make(1, 2, -1));
    
    assert(-2 == box.origin.x);
    assert(-3 == box.origin.y);
    assert(0 == box.origin.z);
    assert(5 == box.size.width);
    assert(7 == box.size.length);
    assert(1 == box.size.height);
}


static void
box_extend_to_include_point_test(void)
{
    struct box box = box_make(point_make(0, 0, 0), size_make(1, 1, 1));
    
    box = box_extend_to_include_point(box, point_make(1, 2, 3));
    assert(point_equals(point_make(0, 0, 0), box.origin));
    assert(size_equals(size_make(2, 3, 4), box.size));
    
    box = box_extend_to_include_point(box, point_make(-3, 0, 0));
    assert(point_equals(point_make(-3, 0, 0), box.origin));
    assert(size_equals(size_make(5, 3, 4), box.size));
    
    box = box_extend_to_include_point(box, point_make(4, 4, 4));
    assert(point_equals(point_make(-3, 0, 0), box.origin));
    assert(size_equals(size_make(8, 5, 5), box.size));
    
    box = box_extend_to_include_point(box, point_make(-3, -3, -3));
    assert(point_equals(point_make(-3, -3, -3), box.origin));
    assert(size_equals(size_make(8, 8, 8), box.size));
}


static void
box_index_for_point_test(void)
{
    struct box box = box_make(point_make(0, 0, 0), size_make(2, 3, 4));
    
    assert(0 == box_index_for_point(box, point_make(0, 0, 0)));
    assert(1 == box_index_for_point(box, point_make(1, 0, 0)));
    assert(2 == box_index_for_point(box, point_make(0, 1, 0)));
    assert(3 == box_index_for_point(box, point_make(1, 1, 0)));
    assert(4 == box_index_for_point(box, point_make(0, 2, 0)));
    assert(5 == box_index_for_point(box, point_make(1, 2, 0)));
    assert(6 == box_index_for_point(box, point_make(0, 0, 1)));
    assert(7 == box_index_for_point(box, point_make(1, 0, 1)));
}


static void
box_intersects_test(void)
{
    struct box box1 = box_make(point_make(0, 0, 0), size_make(10, 10, 1));
    struct box box2 = box_make(point_make(-5, -5, 0), size_make(10, 10, 1));
    struct box box3 = box_make(point_make(5, 5, 0), size_make(10, 10, 1));
    struct box box4 = box_make(point_make(3, 3, 0), size_make(3, 3, 1));
    struct box box5 = box_make(point_make(3, 3, 1), size_make(3, 3, 1));
    struct box box6 = box_make(point_make(3, 3, -1), size_make(3, 3, 1));
    struct box box7 = box_make(point_make(3, 3, -1), size_make(3, 3, 3));
    
    assert(box_intersects(box1, box1));
    assert(box_intersects(box1, box2));
    assert(box_intersects(box1, box3));
    assert(!box_intersects(box2, box3));
    assert(box_intersects(box1, box4));
    assert(box_intersects(box2, box4));
    assert(box_intersects(box3, box4));
    assert(!box_intersects(box4, box5));
    assert(!box_intersects(box4, box6));
    assert(box_intersects(box4, box7));
    assert(box_intersects(box5, box7));
}


static void
box_for_area_test_north(void)
{
    struct box box = box_for_area(point_make(2, 2, 2), direction_north, 3, 3, 0, 1);

    // 5  :  :  :  :  :
    // 4  :  .  .  .  :
    // 3  :  .  .  .  :
    // 2  :  *  .  .  :
    //    1  2  3  4  5
    assert(point_equals(point_make(1, 2, 2), box.origin));
    assert(size_equals(size_make(5, 4, 1), box.size));
}


static void
box_for_area_test_north_with_left_offset(void)
{
    struct box box = box_for_area(point_make(2, 2, 2), direction_north, 3, 3, 1, 1);

    // 5  :  :  :  :  :
    // 4  :  .  .  .  :
    // 3  :  .  .  .  :
    // 2  :  .  *  .  :
    //    0  1  2  3  4
    assert(point_equals(point_make(0, 2, 2), box.origin));
    assert(size_equals(size_make(5, 4, 1), box.size));
}


static void
box_for_area_test_north_with_left_offset_and_no_padding(void)
{
    struct box box = box_for_area(point_make(2, 2, 2), direction_north, 3, 3, 2, 0);

    // 4  .  .  .
    // 3  .  .  .
    // 2  .  .  *
    //    0  1  2
    assert(point_equals(point_make(0, 2, 2), box.origin));
    assert(size_equals(size_make(3, 3, 1), box.size));
}


static void
box_for_area_test_south(void)
{
    struct box box = box_for_area(point_make(2, 2, 2), direction_south, 3, 3, 0, 1);

    // 2  :  .  .  *  :
    // 1  :  .  .  .  :
    // 0  :  .  .  .  :
    //-1  :  :  :  :  :
    //   -1  0  1  2  3
    assert(point_equals(point_make(-1, -1, 2), box.origin));
    assert(size_equals(size_make(5, 4, 1), box.size));
}


static void
box_for_area_test_south_with_left_offset(void)
{
    struct box box = box_for_area(point_make(2, 2, 2), direction_south, 3, 3, 1, 1);

    // 2  :  .  *  .  :
    // 1  :  .  .  .  :
    // 0  :  .  .  .  :
    //-1  :  :  :  :  :
    //    0  1  2  3  4
    assert(point_equals(point_make(0, -1, 2), box.origin));
    assert(size_equals(size_make(5, 4, 1), box.size));
}


static void
box_for_area_test_south_with_left_offset_and_no_padding(void)
{
    struct box box = box_for_area(point_make(2, 2, 2), direction_south, 3, 3, 2, 0);

    // 2  *  .  .
    // 1  .  .  .
    // 0  .  .  .
    //    2  3  4
    assert(point_equals(point_make(2, 0, 2), box.origin));
    assert(size_equals(size_make(3, 3, 1), box.size));
}


static void
box_for_area_test_east(void)
{
    struct box box = box_for_area(point_make(2, 2, 2), direction_east, 3, 3, 0, 1);

    // 3  :  :  :  :
    // 2  *  .  .  :
    // 1  .  .  .  :
    // 0  .  .  .  :
    //-1  :  :  :  :
    //    2  3  4  5
    assert(point_equals(point_make(2, -1, 2), box.origin));
    assert(size_equals(size_make(4, 5, 1), box.size));
}


static void
box_for_area_test_east_with_left_offset(void)
{
    struct box box = box_for_area(point_make(2, 2, 2), direction_east, 3, 3, 1, 1);

    // 4  :  :  :  :
    // 3  .  .  .  :
    // 2  *  .  .  :
    // 1  .  .  .  :
    // 0  :  :  :  :
    //    2  3  4  5
    assert(point_equals(point_make(2, 0, 2), box.origin));
    assert(size_equals(size_make(4, 5, 1), box.size));
}


static void
box_for_area_test_east_with_left_offset_and_no_padding(void)
{
    struct box box = box_for_area(point_make(2, 2, 2), direction_east, 3, 3, 2, 0);

    // 4  .  .  .
    // 3  .  .  .
    // 2  *  .  .
    //    2  3  4
    assert(point_equals(point_make(2, 2, 2), box.origin));
    assert(size_equals(size_make(3, 3, 1), box.size));
}


static void
box_for_area_test_west(void)
{
    struct box box = box_for_area(point_make(2, 2, 2), direction_west, 3, 3, 0, 1);

    // 5  :  :  :  :
    // 4  :  .  .  .
    // 3  :  .  .  .
    // 2  :  .  .  *
    // 1  :  :  :  :
    //   -1  0  1  2
    assert(point_equals(point_make(-1, 1, 2), box.origin));
    assert(size_equals(size_make(4, 5, 1), box.size));
}


static void
box_for_area_test_west_with_left_offset(void)
{
    struct box box = box_for_area(point_make(2, 2, 2), direction_west, 3, 3, 1, 1);

    // 4  :  :  :  :
    // 3  :  .  .  .
    // 2  :  .  .  *
    // 1  :  .  .  .
    // 0  :  :  :  :
    //   -1  0  1  2
    assert(point_equals(point_make(-1, 0, 2), box.origin));
    assert(size_equals(size_make(4, 5, 1), box.size));
}


static void
box_for_area_test_west_with_left_offset_and_no_padding(void)
{
    struct box box = box_for_area(point_make(2, 2, 2), direction_west, 3, 3, 2, 0);

    // 2  .  .  *
    // 1  .  .  .
    // 0  .  .  .
    //    0  1  2
    assert(point_equals(point_make(0, 0, 2), box.origin));
    assert(size_equals(size_make(3, 3, 1), box.size));
}


void
box_test(void)
{
    box_make_test();
    box_make_empty_test();
    box_make_unit_test();
    box_make_from_boxes_test();
    box_make_from_points_test();
    box_equals_test();
    box_contains_box_test();
    box_contains_point_test();
    box_end_point_test();
    box_volume_test();
    box_area_test();
    box_normalize_test();
    box_expand_test();
    box_extend_to_include_point_test();
    box_index_for_point_test();
    box_intersects_test();
    box_for_area_test_north();
    box_for_area_test_north_with_left_offset();
    box_for_area_test_north_with_left_offset_and_no_padding();
    box_for_area_test_south();
    box_for_area_test_south_with_left_offset();
    box_for_area_test_south_with_left_offset_and_no_padding();
    box_for_area_test_east();
    box_for_area_test_east_with_left_offset();
    box_for_area_test_east_with_left_offset_and_no_padding();
    box_for_area_test_west();
    box_for_area_test_west_with_left_offset();
    box_for_area_test_west_with_left_offset_and_no_padding();
}
