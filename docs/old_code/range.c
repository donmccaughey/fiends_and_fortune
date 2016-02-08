#include "range.h"

#include <assert.h>
#include <limits.h>


bool
range_contains(struct range range, int value)
{
    return value >= range.begin && value < range.end;
}


bool
range_equals(struct range range, struct range other)
{
    assert(range.begin <= range.end);
    assert(other.begin <= other.end);
    
    if (range_is_empty(range)) {
        return range_is_empty(other);
    } else {
        return range.begin == other.begin && range.end == other.end;
    }
}


struct range
range_expand(struct range range, int amount)
{
    assert(range.begin <= range.end);
    return range_make(range.begin - amount, range.end + amount);
}


struct range
range_extend_to_include_value(struct range range, int value)
{
    assert(range.begin <= range.end);
    assert(value < INT_MAX);
    
    if (range_is_empty(range)) {
        return range_make(value, value + 1);
    } else {
        struct range extended_range = range;
        if (value < range.begin) extended_range.begin = value;
        if (value >= range.end) extended_range.end = value + 1;
        return extended_range;
    }
}


bool
range_is_empty(struct range range)
{
    assert(range.begin <= range.end);
    return range.begin == range.end;
}


struct range
range_join(struct range range, struct range other)
{
    int begin = range.begin < other.begin ? range.begin : other.begin;
    int end = range.end > other.end ? range.end : other.end;
    return range_make(begin, end);
}


int
range_length(struct range range)
{
    assert(range.begin <= range.end);
    if (range.begin < 0 && range.end >= 0) {
        unsigned positive_segment = (unsigned)(range.end - 0);
        unsigned negative_segment = (unsigned)(0 - range.begin);
        assert(negative_segment <= (unsigned)INT_MAX - positive_segment);
        return positive_segment + negative_segment;
    } else {
        return range.end - range.begin;
    }
}


struct range
range_make(int begin, int end)
{
    assert(begin <= end);
    return (struct range){ .begin=begin, .end=end };
}
