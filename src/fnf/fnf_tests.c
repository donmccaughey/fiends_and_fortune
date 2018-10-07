#include <stdlib.h>
#include <base/base.h>


void
action_test(void);

void
options_test(void);


int
main(int argc, char *argv[])
{
    action_test();
    options_test();
    alloc_count_is_zero_or_die();
    return EXIT_SUCCESS;
}
