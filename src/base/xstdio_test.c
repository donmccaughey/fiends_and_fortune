#include "xstdio.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>


static int wrap_xvasprintf(char **string, const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int result = xvasprintf(string, format, arguments);
    va_end(arguments);
    return result;
}


static void
xvasprintf_test(void)
{
    char *string = NULL;
    int result = wrap_xvasprintf(&string, "%s", "Hello, World!");
    assert(13 == result);
    assert(string);
    assert(0 == strcmp(string, "Hello, World!"));
    free(string);
}


void
xstdio_test(void)
{
    xvasprintf_test();
}
