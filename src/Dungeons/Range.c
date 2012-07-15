#include "Range.h"

#include <assert.h>


struct Range expandRange(struct Range range, int32_t amount)
{
  // TODO: add test coverage for these asserts
  assert(range.begin <= range.end);
  assert(amount < 0
            ? (range.end - range.begin) / 2 >= -amount
            : INT32_MIN - range.begin <= -amount && INT32_MAX - range.end >= amount
        );
  return makeRange(range.begin - amount, range.end + amount);
}


struct ReverseRange expandReverseRange(struct ReverseRange reverseRange, int32_t amount)
{
  // TODO: add test coverage for these asserts
  assert(reverseRange.top >= reverseRange.bottom);
  assert(amount < 0
            ? (reverseRange.top - reverseRange.bottom) / 2 >= -amount
            : INT32_MIN - reverseRange.bottom <= -amount && INT32_MAX - reverseRange.top >= amount
        );
  return makeReverseRange(reverseRange.top + amount, reverseRange.bottom - amount);
}


struct Range makeRange(int32_t begin, int32_t end)
{
  assert(begin <= end);
  return (struct Range) { .begin = begin, .end = end };
}


struct Range makeRangeFromReverseRange(struct ReverseRange reverseRange)
{
  assert(reverseRange.top >= reverseRange.bottom);
  assert(reverseRange.top < INT32_MAX);
  assert(reverseRange.bottom < INT32_MAX);
  return makeRange(reverseRange.bottom + 1, reverseRange.top + 1);
}


struct ReverseRange makeReverseRange(int32_t top, int32_t bottom)
{
  assert(top >= bottom);
  return (struct ReverseRange) { .top = top, .bottom = bottom };
}


struct ReverseRange makeReverseRangeFromRange(struct Range range)
{
  assert(range.begin <= range.end);
  assert(range.begin > INT32_MIN);
  assert(range.end > INT32_MIN);
  return makeReverseRange(range.end - 1, range.begin - 1);
}
