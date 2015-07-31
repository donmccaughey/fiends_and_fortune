#include "reverse_range.h"

#include <assert.h>


bool
reverse_range_is_equal(struct reverse_range reverse_range1,
                       struct reverse_range reverse_range2)
{
    assert(reverse_range1.top >= reverse_range1.bottom);
    assert(reverse_range2.top >= reverse_range2.bottom);
    
    if (reverse_range_is_empty(reverse_range1)) {
        return reverse_range_is_empty(reverse_range2);
    } else {
        return reverse_range1.top == reverse_range2.top
        && reverse_range1.bottom == reverse_range2.bottom;
    }
}


struct reverse_range
reverse_range_expand(struct reverse_range reverse_range, int32_t amount)
{
    // TODO: add test coverage for these asserts
    assert(reverse_range.top >= reverse_range.bottom);
    assert(amount < 0
           ? (reverse_range.top - reverse_range.bottom) / 2 >= -amount
           :    INT32_MIN - reverse_range.bottom <= -amount
             && INT32_MAX - reverse_range.top >= amount
           );
    return reverse_range_make(reverse_range.top + amount,
                              reverse_range.bottom - amount);
}


struct reverse_range
reverse_range_extend_to_include_value(struct reverse_range reverse_range,
                                      int32_t value)
{
    assert(reverse_range.top >= reverse_range.bottom);
    assert(value > INT32_MIN);
    
    if (reverse_range_is_empty(reverse_range)) {
        return reverse_range_make(value, value - 1);
    } else {
        struct reverse_range extendedReverseRange = reverse_range;
        if (value > reverse_range.top) extendedReverseRange.top = value;
        if (value <= reverse_range.bottom) {
            extendedReverseRange.bottom = value - 1;
        }
        return extendedReverseRange;
    }
}


bool
reverse_range_is_empty(struct reverse_range reverse_range)
{
    assert(reverse_range.top >= reverse_range.bottom);
    return reverse_range.top == reverse_range.bottom;
}


uint32_t
reverse_range_length(struct reverse_range reverse_range)
{
    assert(reverse_range.top >= reverse_range.bottom);
    if (reverse_range.top > 0 && reverse_range.bottom < 0) {
        return (uint32_t) (reverse_range.top - 0) + (uint32_t) (0 - reverse_range.bottom);
    } else {
        return (uint32_t) (reverse_range.top - reverse_range.bottom);
    }
}


struct range
range_from_reverse_range(struct reverse_range reverse_range)
{
    assert(reverse_range.top >= reverse_range.bottom);
    assert(reverse_range.top < INT32_MAX);
    return range_make(reverse_range.bottom + 1, reverse_range.top + 1);
}


struct reverse_range
reverse_range_make(int32_t top, int32_t bottom)
{
    assert(top >= bottom);
    return (struct reverse_range) { .top = top, .bottom = bottom };
}


struct reverse_range
reverse_range_from_range(struct range range)
{
    assert(range.begin <= range.end);
    assert(range.begin > INT32_MIN);
    return reverse_range_make(range.end - 1, range.begin - 1);
}
