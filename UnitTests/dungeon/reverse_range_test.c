#include "reverse_range.h"

#include <assert.h>
#include <limits.h>


void
reverse_range_test(void);


static void
reverse_range_is_equal_test(void)
{
    assert(reverse_range_is_equal(reverse_range_make(0, 0),
                                  reverse_range_make(0, 0)));
    assert(reverse_range_is_equal(reverse_range_make(42, 42),
                                  reverse_range_make(0, 0)));
    
    assert(reverse_range_is_equal(reverse_range_make(10, -10),
                                  reverse_range_make(10, -10)));
    assert(reverse_range_is_equal(reverse_range_make(-1, -10),
                                  reverse_range_make(-1, -10)));
    assert(reverse_range_is_equal(reverse_range_make(10, 1),
                                  reverse_range_make(10, 1)));
    
    assert(!reverse_range_is_equal(reverse_range_make(10, -10),
                                   reverse_range_make(10, -11)));
    assert(!reverse_range_is_equal(reverse_range_make(-1, -10),
                                   reverse_range_make(-1, -11)));
    assert(!reverse_range_is_equal(reverse_range_make(10, 1),
                                   reverse_range_make(10, 0)));
}


static void
reverse_range_expand_test(void)
{
    struct reverse_range reverseRange = reverse_range_make(0, 0);
    
    reverseRange = reverse_range_expand(reverseRange, 1);
    
    assert(reverse_range_is_equal(reverse_range_make(1, -1), reverseRange));
    
    reverseRange = reverse_range_expand(reverseRange, 2);
    
    assert(reverse_range_is_equal(reverse_range_make(3, -3), reverseRange));
    
    reverseRange = reverse_range_expand(reverseRange, -3);
    
    assert(reverse_range_is_empty(reverseRange));
}


static void
reverse_range_extend_to_include_value_test(void)
{
    struct reverse_range reverseRange = reverse_range_make(0, 0);
    
    reverseRange = reverse_range_extend_to_include_value(reverseRange, 1);
    
    assert(reverse_range_is_equal(reverse_range_make(1, 0), reverseRange));
    
    reverseRange = reverse_range_extend_to_include_value(reverseRange, -1);
    
    assert(reverse_range_is_equal(reverse_range_make(1, -2), reverseRange));
    
    reverseRange = reverse_range_extend_to_include_value(reverseRange, 42);
    
    assert(reverse_range_is_equal(reverse_range_make(42, -2), reverseRange));
}


static void
reverse_range_length_test(void)
{
    assert(0 == reverse_range_length(reverse_range_make(0, 0)));
    assert(0 == reverse_range_length(reverse_range_make(42, 42)));
    
    assert(1 == reverse_range_length(reverse_range_make(1, 0)));
    assert(1 == reverse_range_length(reverse_range_make(-1, -2)));
    assert(2 == reverse_range_length(reverse_range_make(12, 10)));
    assert(2 == reverse_range_length(reverse_range_make(1, -1)));
    
    assert(INT_MAX == reverse_range_length(reverse_range_make(-1, INT_MIN)));
    assert(INT_MAX == reverse_range_length(reverse_range_make(0, INT_MIN + 1)));
    assert(INT_MAX == reverse_range_length(reverse_range_make(1, INT_MIN + 2)));
    
    assert(INT_MAX == reverse_range_length(reverse_range_make(INT_MAX, 0)));
    assert(INT_MAX == reverse_range_length(reverse_range_make(INT_MAX - 1, -1)));
}


void
reverse_range_test(void)
{
    reverse_range_is_equal_test();
    reverse_range_expand_test();
    reverse_range_extend_to_include_value_test();
    reverse_range_length_test();
}
