#include <stdlib.h>
#include <base/base.h>
#include <background/background.h>


void
language_test(void);


int
main(int argc, char *argv[])
{
    language_test();
    alloc_count_is_zero_or_die();
    return EXIT_SUCCESS;
}
