#include <stdlib.h>
#include <base/base.h>
#include <character/character.h>


void
characteristics_test(void);

void
characteristic_generation_method_test(void);


int
main(int argc, char *argv[])
{
    characteristics_test();
    characteristic_generation_method_test();
    alloc_count_is_zero_or_die();
    return EXIT_SUCCESS;
}
