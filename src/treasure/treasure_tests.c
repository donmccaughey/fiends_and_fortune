#include <stdlib.h>
#include <base/base.h>
#include <mechanics/mechanics.h>
#include <treasure/treasure.h>


void
coins_test(void);


int
main(int argc, char *argv[])
{
    coins_test();
    alloc_count_is_zero_or_die();
    return EXIT_SUCCESS;
}
