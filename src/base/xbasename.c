#include "xbasename.h"
#include "xstring.h"
#include <libgen.h>
#include <stdlib.h>


char *
xbasename(char const *path)
{
    char *path_dup = xstrdup(path);
    char *name = xstrdup(basename(path_dup));
    free(path_dup);
    return name;
}
