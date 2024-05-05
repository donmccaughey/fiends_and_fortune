#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "xstdio.h"


static void
xasprintf_tests(void)
{
    char *s = NULL;
    int bytes_printed = xasprintf(&s, "%s", "Hello, World!");
    assert(13 == bytes_printed);
    assert(s);
    assert(0 == strcmp(s, "Hello, World!"));
    free(s);
}


static int
xvasprintf_helper(char **s, const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int bytes_printed = xvasprintf(s, format, arguments);
    va_end(arguments);
    return bytes_printed;
}


static void
xvasprintf_tests(void)
{
    char *s = NULL;
    int bytes_printed = xvasprintf_helper(&s, "%s", "Hello, World!");
    assert(13 == bytes_printed);
    assert(s);
    assert(0 == strcmp(s, "Hello, World!"));
    free(s);
}


int
main(int argc, char *argv[])
{
    xasprintf_tests();
    xvasprintf_tests();
    return EXIT_SUCCESS;
}
