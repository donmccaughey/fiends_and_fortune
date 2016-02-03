#include "range.h"

#include <assert.h>


bool
range_contains(struct range range, int32_t value)
{
    return value >= range.begin && value < range.end;
}


bool
range_is_equal(struct range range1, struct range range2)
{
    assert(range1.begin <= range1.end);
    assert(range2.begin <= range2.end);
    
    if (range_is_empty(range1)) {
        return range_is_empty(range2);
    } else {
        return range1.begin == range2.begin && range1.end == range2.end;
    }
}


struct range
range_expand(struct range range, int32_t amount)
{
    // TODO: add test coverage for these asserts
    assert(range.begin <= range.end);
    assert(amount < 0
           ? (range.end - range.begin) / 2 >= -amount
           :    INT32_MIN - range.begin <= -amount
             && INT32_MAX - range.end >= amount
           );
    return range_make(range.begin - amount, range.end + amount);
}


struct range
range_extend_to_include_value(struct range range, int32_t value)
{
    assert(range.begin <= range.end);
    assert(value < INT32_MAX);
    
    if (range_is_empty(range)) {
        return range_make(value, value + 1);
    } else {
        struct range extendedRange = range;
        if (value < range.begin) extendedRange.begin = value;
        if (value >= range.end) extendedRange.end = value + 1;
        return extendedRange;
    }
}


bool
range_is_empty(struct range range)
{
    assert(range.begin <= range.end);
    return range.begin == range.end;
}


uint32_t
range_length(struct range range)
{
    assert(range.begin <= range.end);
    if (range.begin < 0 && range.end > 0) {
        return (uint32_t)(range.end - 0) + (uint32_t)(0 - range.begin);
    } else {
        return (uint32_t)(range.end - range.begin);
    }
}


struct range
range_make(int32_t begin, int32_t end)
{
    assert(begin <= end);
    return (struct range){ .begin=begin, .end=end };
}
