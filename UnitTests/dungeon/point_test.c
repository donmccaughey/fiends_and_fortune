#include <assert.h>

#include "point.h"


void
point_test(void);


static void
point_compare_test(void)
{
  assert(0 == point_compare(point_make(1, 2, 3), point_make(1, 2, 3)));

  assert(-1 == point_compare(point_make(1, 2, 3), point_make(2, 2, 3)));
  assert(-2 == point_compare(point_make(1, 2, 3), point_make(1, 4, 3)));
  assert(-3 == point_compare(point_make(1, 2, 3), point_make(1, 2, 6)));

  assert(3 == point_compare(point_make(4, 2, 3), point_make(1, 2, 3)));
  assert(2 == point_compare(point_make(1, 4, 3), point_make(1, 2, 3)));
  assert(1 == point_compare(point_make(1, 2, 4), point_make(1, 2, 3)));
}


void point_test(void)
{
    point_compare_test();
}
