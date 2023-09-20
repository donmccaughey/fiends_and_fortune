#include "fail.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void
vfail(char const *format, va_list arguments);


void
fail(char const *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    vfail(format, arguments);
    va_end(arguments);
}


void
vfail(char const *format, va_list arguments)
{
    vfprintf(stderr, format, arguments);
    fprintf(stderr, "\n");
    if (errno) perror(NULL);
    exit(EXIT_SUCCESS == errno ? EXIT_FAILURE : errno);
}
