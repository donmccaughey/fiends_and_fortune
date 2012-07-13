#ifndef DUNGEONS_RANGE_H_INCLUDED
#define DUNGEONS_RANGE_H_INCLUDED


#include <stdint.h>


struct Range {
  int32_t begin;
  int32_t end;
};

struct ReverseRange {
  int32_t top;
  int32_t bottom;
};


struct Range expandRange(struct Range range, int32_t amount);

struct ReverseRange expandReverseRange(struct ReverseRange reverseRange, int32_t amount);

struct Range makeRange(int32_t begin, int32_t end);

struct Range makeRangeFromReverseRange(struct ReverseRange reverseRange);

struct ReverseRange makeReverseRange(int32_t top, int32_t bottom);

struct ReverseRange makeReverseRangeFromRange(struct Range range);


#endif /* #ifndef DUNGEONS_RANGE_H_INCLUDED */
