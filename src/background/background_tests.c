#include <stdlib.h>
#include <base/base.h>


void
direction_test(void);

void
language_test(void);

void
money_test(void);

void
orientation_test(void);


int
main(int argc, char *argv[])
{
    direction_test();
    language_test();
    money_test();
    orientation_test();
    alloc_count_is_zero_or_die();
    return EXIT_SUCCESS;
}
