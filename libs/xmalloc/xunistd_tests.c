#include <assert.h>
#include <stdlib.h>

#include "xunistd.h"


static void
xgetcwd_tests(void)
{
    char *cwd = xgetcwd();
    assert(cwd);
    assert(cwd[0] == '/');
    free(cwd);
}


int
main(int argc, char *argv[])
{
    xgetcwd_tests();
    return EXIT_SUCCESS;
}
