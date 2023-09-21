#include "xbasename.h"
#include "xstring.h"
#include <libgen.h>


char *
xbasename(char *path)
{
    return xstrdup(basename(path));
}
