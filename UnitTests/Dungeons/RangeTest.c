#include <assert.h>

#include "Range.h"


void rangeTest(void);
static void equalRangesTest(void);
static void equalReverseRangesTest(void);
static void expandRangeTest(void);
static void expandReverseRangeTest(void);
static void extendRangeToIncludeValueTest(void);
static void extendReverseRangeToIncludeValueTest(void);
static void lengthOfRangeTest(void);
static void lengthOfReverseRangeTest(void);


void rangeTest(void)
{
  equalRangesTest();
  equalReverseRangesTest();
  expandRangeTest();
  expandReverseRangeTest();
  extendRangeToIncludeValueTest();
  extendReverseRangeToIncludeValueTest();
  lengthOfRangeTest();
  lengthOfReverseRangeTest();
}


static void equalRangesTest(void)
{
  assert(equalRanges(makeRange(0, 0), makeRange(0, 0)));
  assert(equalRanges(makeRange(42, 42), makeRange(0, 0)));
  
  assert(equalRanges(makeRange(-10, 10), makeRange(-10, 10)));
  assert(equalRanges(makeRange(-10, -1), makeRange(-10, -1)));
  assert(equalRanges(makeRange(1, 10), makeRange(1, 10)));
  
  assert( ! equalRanges(makeRange(-10, 10), makeRange(-10, 11)));
  assert( ! equalRanges(makeRange(-10, -1), makeRange(-11, -1)));
  assert( ! equalRanges(makeRange(1, 10), makeRange(1, 11)));
}


static void equalReverseRangesTest(void)
{
  assert(equalReverseRanges(makeReverseRange(0, 0), makeReverseRange(0, 0)));
  assert(equalReverseRanges(makeReverseRange(42, 42), makeReverseRange(0, 0)));

  assert(equalReverseRanges(makeReverseRange(10, -10), makeReverseRange(10, -10)));
  assert(equalReverseRanges(makeReverseRange(-1, -10), makeReverseRange(-1, -10)));
  assert(equalReverseRanges(makeReverseRange(10, 1), makeReverseRange(10, 1)));
  
  assert( ! equalReverseRanges(makeReverseRange(10, -10), makeReverseRange(10, -11)));
  assert( ! equalReverseRanges(makeReverseRange(-1, -10), makeReverseRange(-1, -11)));
  assert( ! equalReverseRanges(makeReverseRange(10, 1), makeReverseRange(10, 0)));
}


static void expandRangeTest(void)
{
  struct Range range = makeRange(0, 0);
  
  range = expandRange(range, 1);
  
  assert(equalRanges(makeRange(-1, 1), range));
  
  range = expandRange(range, 2);
  
  assert(equalRanges(makeRange(-3, 3), range));
  
  range = expandRange(range, -3);
  
  assert(isEmptyRange(range));
}


static void expandReverseRangeTest(void)
{
  struct ReverseRange reverseRange = makeReverseRange(0, 0);
  
  reverseRange = expandReverseRange(reverseRange, 1);
  
  assert(equalReverseRanges(makeReverseRange(1, -1), reverseRange));
  
  reverseRange = expandReverseRange(reverseRange, 2);
  
  assert(equalReverseRanges(makeReverseRange(3, -3), reverseRange));
  
  reverseRange = expandReverseRange(reverseRange, -3);
  
  assert(isEmptyReverseRange(reverseRange));
}


static void extendRangeToIncludeValueTest(void)
{
  struct Range range = makeRange(0, 0);
  
  range = extendRangeToIncludeValue(range, 1);
  
  assert(equalRanges(makeRange(1, 2), range));
  
  range = extendRangeToIncludeValue(range, -1);
  
  assert(equalRanges(makeRange(-1, 2), range));
  
  range = extendRangeToIncludeValue(range, 42);
  
  assert(equalRanges(makeRange(-1, 43), range));
}


static void extendReverseRangeToIncludeValueTest(void)
{
  struct ReverseRange reverseRange = makeReverseRange(0, 0);
  
  reverseRange = extendReverseRangeToIncludeValue(reverseRange, 1);
  
  assert(equalReverseRanges(makeReverseRange(1, 0), reverseRange));
  
  reverseRange = extendReverseRangeToIncludeValue(reverseRange, -1);
  
  assert(equalReverseRanges(makeReverseRange(1, -2), reverseRange));
  
  reverseRange = extendReverseRangeToIncludeValue(reverseRange, 42);
  
  assert(equalReverseRanges(makeReverseRange(42, -2), reverseRange));
}


static void lengthOfRangeTest(void)
{
  assert(0u == lengthOfRange(makeRange(0, 0)));
  assert(0u == lengthOfRange(makeRange(42, 42)));

  assert(1u == lengthOfRange(makeRange(0, 1)));
  assert(1u == lengthOfRange(makeRange(-2, -1)));
  assert(2u == lengthOfRange(makeRange(10, 12)));
  assert(2u == lengthOfRange(makeRange(-1, 1)));
  
  assert(UINT32_MAX == lengthOfRange(makeRange(INT32_MIN, INT32_MAX)));
}


static void lengthOfReverseRangeTest(void)
{
  assert(0u == lengthOfReverseRange(makeReverseRange(0, 0)));
  assert(0u == lengthOfReverseRange(makeReverseRange(42, 42)));
  
  assert(1u == lengthOfReverseRange(makeReverseRange(1, 0)));
  assert(1u == lengthOfReverseRange(makeReverseRange(-1, -2)));
  assert(2u == lengthOfReverseRange(makeReverseRange(12, 10)));
  assert(2u == lengthOfReverseRange(makeReverseRange(1, -1)));
  
  assert(UINT32_MAX == lengthOfReverseRange(makeReverseRange(INT32_MAX, INT32_MIN)));
}
