#include <stdlib.h>
#include <base/base.h>
#include <mechanics/mechanics.h>


void
dice_test(void);


int
main(int argc, char *argv[])
{
    dice_test();
    alloc_count_is_zero_or_die();
    return EXIT_SUCCESS;
}
