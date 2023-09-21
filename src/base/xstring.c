#include "xstring.h"
#include <stdlib.h>
#include <string.h>


char *
xstrdup(const char *string)
{
    if ( ! string) return NULL;
    char *dup = strdup(string);
    if ( ! dup) abort();
    return dup;
}
