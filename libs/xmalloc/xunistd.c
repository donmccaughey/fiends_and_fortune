#include "xunistd.h"

#include <stdlib.h>
#include <unistd.h>


char *
xgetcwd(void)
{
    char *cwd = getcwd(NULL, 0);
    if (!cwd) abort();
    return cwd;
}
