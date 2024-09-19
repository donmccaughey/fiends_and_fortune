#include "unreachable.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void
vunreachable(char const *format, va_list arguments) XMALLOC_PRINTF(1, 0);


static void
vunreachable(char const *format, va_list arguments)
{
    vfprintf(stderr, format, arguments);
    fprintf(stderr, "\n");
    if (errno) perror(NULL);
    exit(EXIT_SUCCESS == errno ? EXIT_FAILURE : errno);
}


void
unreachable(char const *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    vunreachable(format, arguments);
    va_end(arguments);
}
