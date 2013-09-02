#include <assert.h>
#include "Point.h"


void pointTest(void);
static void comparePointsTest(void);


void pointTest(void)
{
  comparePointsTest();
}


static void comparePointsTest(void)
{
  assert(0 == comparePoints(makePoint(1, 2, 3), makePoint(1, 2, 3)));

  assert(-1 == comparePoints(makePoint(1, 2, 3), makePoint(2, 2, 3)));
  assert(-2 == comparePoints(makePoint(1, 2, 3), makePoint(1, 4, 3)));
  assert(-3 == comparePoints(makePoint(1, 2, 3), makePoint(1, 2, 6)));

  assert(3 == comparePoints(makePoint(4, 2, 3), makePoint(1, 2, 3)));
  assert(2 == comparePoints(makePoint(1, 4, 3), makePoint(1, 2, 3)));
  assert(1 == comparePoints(makePoint(1, 2, 4), makePoint(1, 2, 3)));
}
