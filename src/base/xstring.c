#include "xstring.h"
#include "fail.h"
#include <string.h>


char *
xstrdup(const char *string)
{
    if ( ! string) return NULL;
    char *dup = strdup(string);
    if ( ! dup) fail("strdup(%p) failed", string);
    return dup;
}
