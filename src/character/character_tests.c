#include <stdlib.h>
#include <base/base.h>
#include <character/character.h>


void
method_test(void);


int
main(int argc, char *argv[])
{
    method_test();
    alloc_count_is_zero_or_die();
    return EXIT_SUCCESS;
}
