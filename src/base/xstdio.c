#include "xstdio.h"


#include <stdio.h>
#include <stdlib.h>


int
xvasprintf(char **string, const char *format, va_list arguments)
{
    int result = vasprintf(string, format, arguments);
    if (-1 == result) abort();
    return result;
}
