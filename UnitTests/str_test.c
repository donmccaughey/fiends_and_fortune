#include <assert.h>
#include <math.h>

#include "alloc_or_die.h"
#include "str.h"


void
str_test(void);


static void
str_realloc_format_test(void)
{
    char *str = NULL;
    
    str_realloc_append_formatted(&str, "foo");
    assert(0 == strcmp("foo", str));
    
    str_realloc_append_formatted(&str, "%s", "bar");
    assert(0 == strcmp("foobar", str));
    
    str_realloc_append_formatted(&str, "%c%c%c", 'b', 'a', 'z');
    assert(0 == strcmp("foobarbaz", str));
    
    str_realloc_append_formatted(&str, " %i", 42);
    assert(0 == strcmp("foobarbaz 42", str));
    
    str_realloc_append_formatted(&str, " x %.2f", M_PI);
    assert(0 == strcmp("foobarbaz 42 x 3.14", str));
    
    free_or_die(str);
}


void
str_test(void)
{
    str_realloc_format_test();
}
