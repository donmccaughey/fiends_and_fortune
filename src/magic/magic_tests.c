#include <stdlib.h>
#include <base/base.h>
#include <magic/magic.h>


void
spell_test(void);


int
main(int argc, char *argv[])
{
    spell_test();
    alloc_count_is_zero_or_die();
    return EXIT_SUCCESS;
}
