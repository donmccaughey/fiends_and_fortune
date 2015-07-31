#include <assert.h>

#include "range.h"


void range_test(void);
static void range_is_equal_test(void);
static void reverse_range_is_equal_test(void);
static void range_expand_test(void);
static void reverse_range_expand_test(void);
static void range_extend_to_include_value_test(void);
static void reverse_range_extend_to_include_value_test(void);
static void range_length_test(void);
static void reverse_range_length_test(void);


void range_test(void)
{
    range_is_equal_test();
    reverse_range_is_equal_test();
    range_expand_test();
    reverse_range_expand_test();
    range_extend_to_include_value_test();
    reverse_range_extend_to_include_value_test();
    range_length_test();
    reverse_range_length_test();
}


static void range_is_equal_test(void)
{
    assert(range_is_equal(range_make(0, 0), range_make(0, 0)));
    assert(range_is_equal(range_make(42, 42), range_make(0, 0)));
    
    assert(range_is_equal(range_make(-10, 10), range_make(-10, 10)));
    assert(range_is_equal(range_make(-10, -1), range_make(-10, -1)));
    assert(range_is_equal(range_make(1, 10), range_make(1, 10)));
    
    assert(!range_is_equal(range_make(-10, 10), range_make(-10, 11)));
    assert(!range_is_equal(range_make(-10, -1), range_make(-11, -1)));
    assert(!range_is_equal(range_make(1, 10), range_make(1, 11)));
}


static void reverse_range_is_equal_test(void)
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


static void range_expand_test(void)
{
    struct range range = range_make(0, 0);
    
    range = range_expand(range, 1);
    
    assert(range_is_equal(range_make(-1, 1), range));
    
    range = range_expand(range, 2);
    
    assert(range_is_equal(range_make(-3, 3), range));
    
    range = range_expand(range, -3);
    
    assert(range_is_empty(range));
}


static void reverse_range_expand_test(void)
{
    struct reverse_range reverseRange = reverse_range_make(0, 0);
    
    reverseRange = reverse_range_expand(reverseRange, 1);
    
    assert(reverse_range_is_equal(reverse_range_make(1, -1), reverseRange));
    
    reverseRange = reverse_range_expand(reverseRange, 2);
    
    assert(reverse_range_is_equal(reverse_range_make(3, -3), reverseRange));
    
    reverseRange = reverse_range_expand(reverseRange, -3);
    
    assert(reverse_range_is_empty(reverseRange));
}


static void range_extend_to_include_value_test(void)
{
    struct range range = range_make(0, 0);
    
    range = range_extend_to_include_value(range, 1);
    
    assert(range_is_equal(range_make(1, 2), range));
    
    range = range_extend_to_include_value(range, -1);
    
    assert(range_is_equal(range_make(-1, 2), range));
    
    range = range_extend_to_include_value(range, 42);
    
    assert(range_is_equal(range_make(-1, 43), range));
}


static void reverse_range_extend_to_include_value_test(void)
{
    struct reverse_range reverseRange = reverse_range_make(0, 0);
    
    reverseRange = reverse_range_extend_to_include_value(reverseRange, 1);
    
    assert(reverse_range_is_equal(reverse_range_make(1, 0), reverseRange));
    
    reverseRange = reverse_range_extend_to_include_value(reverseRange, -1);
    
    assert(reverse_range_is_equal(reverse_range_make(1, -2), reverseRange));
    
    reverseRange = reverse_range_extend_to_include_value(reverseRange, 42);
    
    assert(reverse_range_is_equal(reverse_range_make(42, -2), reverseRange));
}


static void range_length_test(void)
{
    assert(0u == range_length(range_make(0, 0)));
    assert(0u == range_length(range_make(42, 42)));
    
    assert(1u == range_length(range_make(0, 1)));
    assert(1u == range_length(range_make(-2, -1)));
    assert(2u == range_length(range_make(10, 12)));
    assert(2u == range_length(range_make(-1, 1)));
    
    assert(UINT32_MAX == range_length(range_make(INT32_MIN, INT32_MAX)));
}


static void reverse_range_length_test(void)
{
    assert(0u == reverse_range_length(reverse_range_make(0, 0)));
    assert(0u == reverse_range_length(reverse_range_make(42, 42)));
    
    assert(1u == reverse_range_length(reverse_range_make(1, 0)));
    assert(1u == reverse_range_length(reverse_range_make(-1, -2)));
    assert(2u == reverse_range_length(reverse_range_make(12, 10)));
    assert(2u == reverse_range_length(reverse_range_make(1, -1)));
    
    assert(UINT32_MAX == reverse_range_length(reverse_range_make(INT32_MAX, INT32_MIN)));
}
