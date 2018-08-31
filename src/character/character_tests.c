#include <stdlib.h>
#include <base/base.h>
#include <character/character.h>


void
characteristics_test(void);

void
characteristic_generation_method_test(void);

void
characteristic_rolls_test(void);


int
main(int argc, char *argv[])
{
    characteristic_generation_method_test();
    characteristic_rolls_test();
    characteristics_test();
    alloc_count_is_zero_or_die();
    return EXIT_SUCCESS;
}
