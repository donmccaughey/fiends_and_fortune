#include "Range.h"

#include <assert.h>


Boolean equalRanges(struct Range range1, struct Range range2)
{
  assert(range1.begin <= range1.end);
  assert(range2.begin <= range2.end);

  if (isEmptyRange(range1)) {
    return isEmptyRange(range2);
  } else {
    return range1.begin == range2.begin && range1.end == range2.end;
  }
}


Boolean equalReverseRanges(struct ReverseRange reverseRange1, struct ReverseRange reverseRange2)
{
  assert(reverseRange1.top >= reverseRange1.bottom);
  assert(reverseRange2.top >= reverseRange2.bottom);

  if (isEmptyReverseRange(reverseRange1)) {
    return isEmptyReverseRange(reverseRange2);
  } else {
    return reverseRange1.top == reverseRange2.top
        && reverseRange1.bottom == reverseRange2.bottom;
  }
}


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


struct Range extendRangeToIncludeValue(struct Range range, int32_t value)
{
  assert(range.begin <= range.end);
  assert(value < INT32_MAX);

  if (isEmptyRange(range)) {
    return makeRange(value, value + 1);
  } else {
    struct Range extendedRange = range;
    if (value < range.begin) extendedRange.begin = value;
    if (value >= range.end) extendedRange.end = value + 1;
    return extendedRange;
  }
}


struct ReverseRange extendReverseRangeToIncludeValue(struct ReverseRange reverseRange, int32_t value)
{
  assert(reverseRange.top >= reverseRange.bottom);
  assert(value > INT32_MIN);

  if (isEmptyReverseRange(reverseRange)) {
    return makeReverseRange(value, value - 1);
  } else {
    struct ReverseRange extendedReverseRange = reverseRange;
    if (value > reverseRange.top) extendedReverseRange.top = value;
    if (value <= reverseRange.bottom) extendedReverseRange.bottom = value - 1;
    return extendedReverseRange;
  }
}


Boolean isEmptyRange(struct Range range)
{
  assert(range.begin <= range.end);
  return range.begin == range.end;
}


Boolean isEmptyReverseRange(struct ReverseRange reverseRange)
{
  assert(reverseRange.top >= reverseRange.bottom);
  return reverseRange.top == reverseRange.bottom;
}


uint32_t lengthOfRange(struct Range range)
{
  assert(range.begin <= range.end);
  if (range.begin < 0 && range.end > 0) {
    return (uint32_t) (range.end - 0) + (uint32_t) (0 - range.begin);
  } else {
    return (uint32_t) (range.end - range.begin);
  }
}


uint32_t lengthOfReverseRange(struct ReverseRange reverseRange)
{
  assert(reverseRange.top >= reverseRange.bottom);
  if (reverseRange.top > 0 && reverseRange.bottom < 0) {
    return (uint32_t) (reverseRange.top - 0) + (uint32_t) (0 - reverseRange.bottom);
  } else {
    return (uint32_t) (reverseRange.top - reverseRange.bottom);
  }
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
  return makeReverseRange(range.end - 1, range.begin - 1);
}
