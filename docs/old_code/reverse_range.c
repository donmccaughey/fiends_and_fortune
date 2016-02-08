#include "reverse_range.h"

#include <assert.h>
#include <limits.h>


bool
reverse_range_equals(struct reverse_range reverse_range,
                     struct reverse_range other)
{
    assert(reverse_range.top >= reverse_range.bottom);
    assert(other.top >= other.bottom);
    
    if (reverse_range_is_empty(reverse_range)) {
        return reverse_range_is_empty(other);
    } else {
        return reverse_range.top == other.top
            && reverse_range.bottom == other.bottom;
    }
}


struct reverse_range
reverse_range_expand(struct reverse_range reverse_range, int amount)
{
    assert(reverse_range.top >= reverse_range.bottom);
    return reverse_range_make(reverse_range.top + amount,
                              reverse_range.bottom - amount);
}


struct reverse_range
reverse_range_extend_to_include_value(struct reverse_range reverse_range,
                                      int value)
{
    assert(reverse_range.top >= reverse_range.bottom);
    assert(value > INT_MIN);
    
    if (reverse_range_is_empty(reverse_range)) {
        return reverse_range_make(value, value - 1);
    } else {
        struct reverse_range extended_reverse_range = reverse_range;
        if (value > reverse_range.top) extended_reverse_range.top = value;
        if (value <= reverse_range.bottom) {
            extended_reverse_range.bottom = value - 1;
        }
        return extended_reverse_range;
    }
}


bool
reverse_range_is_empty(struct reverse_range reverse_range)
{
    assert(reverse_range.top >= reverse_range.bottom);
    return reverse_range.top == reverse_range.bottom;
}


int
reverse_range_length(struct reverse_range reverse_range)
{
    assert(reverse_range.top >= reverse_range.bottom);
    if (reverse_range.top >= 0 && reverse_range.bottom < 0) {
        unsigned positive_segment = (unsigned)(reverse_range.top - 0);
        unsigned negative_segment = (unsigned)(0 - reverse_range.bottom);
        assert(negative_segment <= (unsigned)INT_MAX - positive_segment);
        return positive_segment + negative_segment;
    } else {
        return reverse_range.top - reverse_range.bottom;
    }
}


struct range
range_from_reverse_range(struct reverse_range reverse_range)
{
    assert(reverse_range.top >= reverse_range.bottom);
    assert(reverse_range.top < INT_MAX);
    return range_make(reverse_range.bottom + 1, reverse_range.top + 1);
}


struct reverse_range
reverse_range_make(int top, int bottom)
{
    assert(top >= bottom);
    return (struct reverse_range){ .top=top, .bottom=bottom };
}


struct reverse_range
reverse_range_from_range(struct range range)
{
    assert(range.begin <= range.end);
    assert(range.begin > INT_MIN);
    return reverse_range_make(range.end - 1, range.begin - 1);
}
